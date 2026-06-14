#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SDL3 + Vulkan
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

// cglm
#include "cglm.h"

// LuaJIT
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// tinyfiledialogs
#include "tinyfiledialogs.h"

// Nuklear
#include "include/backend/nuklear_config.h"
#include "nuklear_sdl_vulkan.h"

// ------------------------------------------------------------------ config
#define WIDTH  900
#define HEIGHT 600
#define MAX_FRAMES 2

// ------------------------------------------------------------------ embedded triangle SPIR-V
// vert: passthrough with hardcoded positions, frag: red output
static const uint32_t tri_vert_spv[] = {
    0x07230203,0x00010000,0x0008000b,0x00000018,0x00000000,0x00020011,0x00000001,
    0x0006000b,0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,
    0x00000000,0x00000001,0x0007000f,0x00000000,0x00000004,0x6e69616d,0x00000000,
    0x00000009,0x0000000b,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,
    0x6e69616d,0x00000000,0x00060005,0x00000007,0x505f6c67,0x65567265,0x78657472,
    0x00000000,0x00060006,0x00000007,0x00000000,0x505f6c67,0x7469736f,0x006e6f69,
    0x00030005,0x00000009,0x00000000,0x00040005,0x0000000b,0x565f6c67,0x78657472,
    0x00030047,0x00000007,0x00000002,0x00050048,0x00000007,0x00000000,0x0000000b,
    0x00000000,0x00040047,0x0000000b,0x0000000b,0x00000000,0x00020013,0x00000002,
    0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,
    0x00000010,0x00000006,0x00000004,0x0003001e,0x00000007,0x00000010,0x00040020,
    0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,
    0x00040015,0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,
    0x00000000,0x00040015,0x0000000e,0x00000020,0x00000000,0x00040020,0x0000000f,
    0x00000001,0x0000000e,0x0004003b,0x0000000f,0x0000000b,0x00000001,0x00040020,
    0x00000011,0x00000003,0x00000010,0x0004002b,0x00000006,0x00000012,0x00000000,
    0x0004002b,0x00000006,0x00000013,0x3f800000,
    // positions array: (-0.5,-0.5), (0.5,-0.5), (0.0, 0.5)
    // We'll use a simpler approach: gl_VertexIndex to pick position
    0x00090032,0x00000010,0x00000014,
    0xbf000000,0xbf000000,0x00000000,0x3f800000,
    0x00090032,0x00000010,0x00000015,
    0x3f000000,0xbf000000,0x00000000,0x3f800000,
    0x00090032,0x00000010,0x00000016,
    0x00000000,0x3f000000,0x00000000,0x3f800000,
    0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,
    0x000200f8,0x00000005,
    0x0004003d,0x0000000e,0x00000017,0x0000000b,
    0x000600c7,0x0000000e,0x00000018,0x00000017,0x00000000,0x00000000,
    0x000500c4,0x00000010,0x00000019,0x00000018,0x00000000,
    0x00050041,0x00000011,0x0000001a,0x00000009,0x0000000d,
    0x0003003e,0x0000001a,0x00000019,
    0x000100fd,0x00010038
};

// Simpler approach: use hardcoded SPIR-V from glslangValidator
// vert shader: uses gl_VertexIndex to pick from 3 hardcoded positions
static const uint32_t vert_spv[] = {
    0x07230203,0x00010000,0x0008000b,0x00000020,0x00000000,0x00020011,0x00000001,
    0x0006000b,0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,
    0x00000000,0x00000001,0x0007000f,0x00000000,0x00000004,0x6e69616d,0x00000000,
    0x0000000d,0x00000013,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,
    0x6e69616d,0x00000000,0x00060005,0x0000000b,0x505f6c67,0x65567265,0x78657274,
    0x00000000,0x00060006,0x0000000b,0x00000000,0x505f6c67,0x7469736f,0x006e6f69,
    0x00030005,0x0000000d,0x00000000,0x00040005,0x00000013,0x565f6c67,0x78657274,
    0x00657865,0x00030047,0x0000000b,0x00000002,0x00050048,0x0000000b,0x00000000,
    0x0000000b,0x00000000,0x00040047,0x00000013,0x0000000b,0x00000000,0x00020013,
    0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,
    0x00040017,0x00000007,0x00000006,0x00000004,0x00040015,0x00000008,0x00000020,
    0x00000000,0x0004002b,0x00000008,0x00000009,0x00000003,0x0004001c,0x0000000a,
    0x00000007,0x00000009,0x0003001e,0x0000000b,0x00000007,0x00040020,0x0000000c,
    0x00000003,0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000003,0x00040015,
    0x0000000e,0x00000020,0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,
    0x00040020,0x00000010,0x00000003,0x00000007,0x00040020,0x00000012,0x00000001,
    0x0000000e,0x0004003b,0x00000012,0x00000013,0x00000001,
    // vec4 positions[3]
    0x0004002b,0x00000006,0x00000014,0xbf000000, // -0.5
    0x0004002b,0x00000006,0x00000015,0xbf000000, // -0.5
    0x0004002b,0x00000006,0x00000016,0x00000000, //  0.0
    0x0004002b,0x00000006,0x00000017,0x3f800000, //  1.0
    0x00070050,0x00000007,0x00000018,0x00000014,0x00000015,0x00000016,0x00000017,
    0x0004002b,0x00000006,0x00000019,0x3f000000, //  0.5
    0x00070050,0x00000007,0x0000001a,0x00000019,0x00000015,0x00000016,0x00000017,
    0x0004002b,0x00000006,0x0000001b,0x00000000, //  0.0
    0x0004002b,0x00000006,0x0000001c,0x3f000000, //  0.5
    0x00070050,0x00000007,0x0000001d,0x0000001b,0x0000001c,0x00000016,0x00000017,
    0x0006002c,0x0000000a,0x0000001e,0x00000018,0x0000001a,0x0000001d,
    0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,
    0x000200f8,0x00000005,
    0x0004003d,0x0000000e,0x0000001f,0x00000013,
    0x00050041,0x00000010,0x00000020,0x0000000d,0x0000000f,
    0x00060041,0x00000010,0x00000021,0x0000001e,0x0000001f,
    0x0004003d,0x00000007,0x00000022,0x00000021,
    0x0003003e,0x00000020,0x00000022,
    0x000100fd,0x00010038
};
static const size_t vert_spv_size = sizeof(vert_spv);

// frag: output solid green
static const uint32_t frag_spv[] = {
    0x07230203,0x00010000,0x0008000b,0x0000000d,0x00000000,0x00020011,0x00000001,
    0x0006000b,0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,
    0x00000000,0x00000001,0x0006000f,0x00000004,0x00000004,0x6e69616d,0x00000000,
    0x00000009,0x00030010,0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,
    0x00040005,0x00000004,0x6e69616d,0x00000000,0x00040005,0x00000009,0x6c6f6366,
    0x0000726f,0x00040047,0x00000009,0x0000001e,0x00000000,0x00020013,0x00000002,
    0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,
    0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,0x00000007,
    0x0004003b,0x00000008,0x00000009,0x00000003,0x0004002b,0x00000006,0x0000000a,
    0x3f800000,0x0004002b,0x00000006,0x0000000b,0x00000000,
    0x00070050,0x00000007,0x0000000c,0x0000000b,0x0000000a,0x0000000b,0x0000000a,
    0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,
    0x000200f8,0x00000005,
    0x0003003e,0x00000009,0x0000000c,
    0x000100fd,0x00010038
};
static const size_t frag_spv_size = sizeof(frag_spv);

// ------------------------------------------------------------------ globals
static SDL_Window        *g_win;
static VkInstance         g_inst;
static VkSurfaceKHR       g_surf;
static VkPhysicalDevice   g_phys = VK_NULL_HANDLE;
static VkDevice           g_dev;
static VkQueue            g_queue;
static uint32_t           g_qfam;
static VkSwapchainKHR     g_swap;
static VkImage           *g_imgs;
static VkImageView       *g_views;
static uint32_t           g_img_count;
static VkFormat           g_fmt;
static VkExtent2D         g_ext;
static VkRenderPass       g_rp;
static VkFramebuffer     *g_fbs;
static VkCommandPool      g_pool;
static VkCommandBuffer    g_cmds[MAX_FRAMES];
static VkSemaphore        g_img_sem[MAX_FRAMES];
static VkSemaphore        g_ren_sem[MAX_FRAMES];
static VkFence            g_fence[MAX_FRAMES];
static uint32_t           g_frame = 0;
static VkPipelineLayout   g_pl;
static VkPipeline         g_pipe;

static char               g_lua_result[256] = "No script run yet";
static char               g_lua_file[512]   = "No file selected";

#define VK_CHECK(x) do { VkResult _r=(x); if(_r!=VK_SUCCESS){fprintf(stderr,"VK error %d line %d\n",_r,__LINE__);exit(1);}} while(0)

// ------------------------------------------------------------------ Vulkan helpers
static VkShaderModule make_shader(const uint32_t *code, size_t size) {
    VkShaderModuleCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = size, .pCode = code,
    };
    VkShaderModule m;
    VK_CHECK(vkCreateShaderModule(g_dev, &ci, NULL, &m));
    return m;
}

static void create_instance(void) {
    VkApplicationInfo ai = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "VisualX",
        .apiVersion = VK_API_VERSION_1_0,
    };
    uint32_t n = 0;
    const char * const *exts = SDL_Vulkan_GetInstanceExtensions(&n);
    VkInstanceCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &ai,
        .enabledExtensionCount = n,
        .ppEnabledExtensionNames = exts,
    };
    VK_CHECK(vkCreateInstance(&ci, NULL, &g_inst));
}

static void pick_device(void) {
    uint32_t n = 0;
    vkEnumeratePhysicalDevices(g_inst, &n, NULL);
    VkPhysicalDevice *devs = malloc(n * sizeof *devs);
    vkEnumeratePhysicalDevices(g_inst, &n, devs);
    g_phys = devs[0];
    free(devs);

    uint32_t qn = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(g_phys, &qn, NULL);
    VkQueueFamilyProperties *qp = malloc(qn * sizeof *qp);
    vkGetPhysicalDeviceQueueFamilyProperties(g_phys, &qn, qp);
    for (uint32_t i = 0; i < qn; i++) {
        VkBool32 pres = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(g_phys, i, g_surf, &pres);
        if ((qp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && pres) { g_qfam = i; break; }
    }
    free(qp);

    float prio = 1.0f;
    VkDeviceQueueCreateInfo qci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = g_qfam, .queueCount = 1, .pQueuePriorities = &prio,
    };
    const char *ext = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    VkDeviceCreateInfo dci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1, .pQueueCreateInfos = &qci,
        .enabledExtensionCount = 1, .ppEnabledExtensionNames = &ext,
    };
    VK_CHECK(vkCreateDevice(g_phys, &dci, NULL, &g_dev));
    vkGetDeviceQueue(g_dev, g_qfam, 0, &g_queue);
}

static void create_swapchain(void) {
    VkSurfaceCapabilitiesKHR caps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(g_phys, g_surf, &caps);
    uint32_t fn = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(g_phys, g_surf, &fn, NULL);
    VkSurfaceFormatKHR *fmts = malloc(fn * sizeof *fmts);
    vkGetPhysicalDeviceSurfaceFormatsKHR(g_phys, g_surf, &fn, fmts);
    g_fmt = fmts[0].format;
    VkColorSpaceKHR cs = fmts[0].colorSpace;
    free(fmts);
    g_ext = caps.currentExtent.width != UINT32_MAX ? caps.currentExtent : (VkExtent2D){WIDTH,HEIGHT};

    VkSwapchainCreateInfoKHR sci = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = g_surf, .minImageCount = caps.minImageCount + 1,
        .imageFormat = g_fmt, .imageColorSpace = cs,
        .imageExtent = g_ext, .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = caps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR, .clipped = VK_TRUE,
    };
    VK_CHECK(vkCreateSwapchainKHR(g_dev, &sci, NULL, &g_swap));
    vkGetSwapchainImagesKHR(g_dev, g_swap, &g_img_count, NULL);
    g_imgs = malloc(g_img_count * sizeof *g_imgs);
    vkGetSwapchainImagesKHR(g_dev, g_swap, &g_img_count, g_imgs);
    g_views = malloc(g_img_count * sizeof *g_views);
    for (uint32_t i = 0; i < g_img_count; i++) {
        VkImageViewCreateInfo iv = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = g_imgs[i], .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = g_fmt,
            .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0,1,0,1 },
        };
        VK_CHECK(vkCreateImageView(g_dev, &iv, NULL, &g_views[i]));
    }
}

static void create_render_pass(void) {
    VkAttachmentDescription att = {
        .format = g_fmt, .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };
    VkAttachmentReference ref = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkSubpassDescription sub = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1, .pColorAttachments = &ref,
    };
    VkSubpassDependency dep = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };
    VkRenderPassCreateInfo rci = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1, .pAttachments = &att,
        .subpassCount = 1, .pSubpasses = &sub,
        .dependencyCount = 1, .pDependencies = &dep,
    };
    VK_CHECK(vkCreateRenderPass(g_dev, &rci, NULL, &g_rp));
}

static void create_framebuffers(void) {
    g_fbs = malloc(g_img_count * sizeof *g_fbs);
    for (uint32_t i = 0; i < g_img_count; i++) {
        VkFramebufferCreateInfo fci = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = g_rp, .attachmentCount = 1, .pAttachments = &g_views[i],
            .width = g_ext.width, .height = g_ext.height, .layers = 1,
        };
        VK_CHECK(vkCreateFramebuffer(g_dev, &fci, NULL, &g_fbs[i]));
    }
}

static void create_pipeline(void) {
    VkShaderModule vs = make_shader(vert_spv, vert_spv_size);
    VkShaderModule fs = make_shader(frag_spv, frag_spv_size);

    VkPipelineShaderStageCreateInfo stages[2] = {
        { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .stage = VK_SHADER_STAGE_VERTEX_BIT, .module = vs, .pName = "main" },
        { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .stage = VK_SHADER_STAGE_FRAGMENT_BIT, .module = fs, .pName = "main" },
    };
    VkPipelineVertexInputStateCreateInfo vi = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    };
    VkPipelineInputAssemblyStateCreateInfo ia = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };
    VkViewport vp = { 0,0,(float)g_ext.width,(float)g_ext.height,0,1 };
    VkRect2D sc = { {0,0}, g_ext };
    VkPipelineViewportStateCreateInfo vps = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1, .pViewports = &vp,
        .scissorCount = 1, .pScissors = &sc,
    };
    VkPipelineRasterizationStateCreateInfo rs = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .lineWidth = 1.0f,
    };
    VkPipelineMultisampleStateCreateInfo ms = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };
    VkPipelineColorBlendAttachmentState cba = {
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|
                          VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT,
    };
    VkPipelineColorBlendStateCreateInfo cb = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .attachmentCount = 1, .pAttachments = &cba,
    };
    VkPipelineLayoutCreateInfo plci = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    };
    VK_CHECK(vkCreatePipelineLayout(g_dev, &plci, NULL, &g_pl));

    VkGraphicsPipelineCreateInfo gpci = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2, .pStages = stages,
        .pVertexInputState = &vi, .pInputAssemblyState = &ia,
        .pViewportState = &vps, .pRasterizationState = &rs,
        .pMultisampleState = &ms, .pColorBlendState = &cb,
        .layout = g_pl, .renderPass = g_rp,
    };
    VK_CHECK(vkCreateGraphicsPipelines(g_dev, VK_NULL_HANDLE, 1, &gpci, NULL, &g_pipe));
    vkDestroyShaderModule(g_dev, vs, NULL);
    vkDestroyShaderModule(g_dev, fs, NULL);
}

static void create_commands(void) {
    VkCommandPoolCreateInfo cpci = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = g_qfam,
    };
    VK_CHECK(vkCreateCommandPool(g_dev, &cpci, NULL, &g_pool));
    VkCommandBufferAllocateInfo cbai = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = g_pool, .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = MAX_FRAMES,
    };
    VK_CHECK(vkAllocateCommandBuffers(g_dev, &cbai, g_cmds));
    VkSemaphoreCreateInfo sci = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    VkFenceCreateInfo fci = { .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                               .flags = VK_FENCE_CREATE_SIGNALED_BIT };
    for (int i = 0; i < MAX_FRAMES; i++) {
        VK_CHECK(vkCreateSemaphore(g_dev, &sci, NULL, &g_img_sem[i]));
        VK_CHECK(vkCreateSemaphore(g_dev, &sci, NULL, &g_ren_sem[i]));
        VK_CHECK(vkCreateFence(g_dev, &fci, NULL, &g_fence[i]));
    }
}

// ------------------------------------------------------------------ draw frame
static VkSemaphore draw_triangle(VkSemaphore wait_sem) {
    vkWaitForFences(g_dev, 1, &g_fence[g_frame], VK_TRUE, UINT64_MAX);
    vkResetFences(g_dev, 1, &g_fence[g_frame]);

    uint32_t img;
    vkAcquireNextImageKHR(g_dev, g_swap, UINT64_MAX, g_img_sem[g_frame], VK_NULL_HANDLE, &img);

    VkCommandBuffer cmd = g_cmds[g_frame];
    vkResetCommandBuffer(cmd, 0);
    VkCommandBufferBeginInfo bi = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    vkBeginCommandBuffer(cmd, &bi);

    VkClearValue clear = { .color = { .float32 = {0.08f, 0.08f, 0.12f, 1.0f} } };
    VkRenderPassBeginInfo rpbi = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = g_rp, .framebuffer = g_fbs[img],
        .renderArea = { .extent = g_ext },
        .clearValueCount = 1, .pClearValues = &clear,
    };
    vkCmdBeginRenderPass(cmd, &rpbi, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, g_pipe);
    vkCmdDraw(cmd, 3, 1, 0, 0);
    vkCmdEndRenderPass(cmd);
    vkEndCommandBuffer(cmd);

    VkPipelineStageFlags wait = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    uint32_t wait_count = wait_sem ? 1 : 0;
    VkSemaphore wait_sems[2] = { g_img_sem[g_frame], wait_sem };
    VkPipelineStageFlags wait_stages[2] = { wait, wait };

    VkSubmitInfo si = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = wait_count + 1,
        .pWaitSemaphores = wait_sems,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1, .pCommandBuffers = &cmd,
        .signalSemaphoreCount = 1, .pSignalSemaphores = &g_ren_sem[g_frame],
    };
    VK_CHECK(vkQueueSubmit(g_queue, 1, &si, g_fence[g_frame]));

    VkPresentInfoKHR pi = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1, .pWaitSemaphores = &g_ren_sem[g_frame],
        .swapchainCount = 1, .pSwapchains = &g_swap, .pImageIndices = &img,
    };
    vkQueuePresentKHR(g_queue, &pi);
    g_frame = (g_frame + 1) % MAX_FRAMES;
    return g_ren_sem[g_frame];
}

// ------------------------------------------------------------------ LuaJIT inline test
static void run_lua_inline(void) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    const char *script =
        "local result = 0\n"
        "for i = 1, 100 do result = result + i end\n"
        "return 'Sum 1-100 = ' .. result\n";

    if (luaL_dostring(L, script) == 0) {
        const char *res = lua_tostring(L, -1);
        if (res) snprintf(g_lua_result, sizeof(g_lua_result), "%s", res);
    } else {
        snprintf(g_lua_result, sizeof(g_lua_result), "Lua error: %s", lua_tostring(L, -1));
    }
    lua_close(L);
}

// ------------------------------------------------------------------ main
int main(void) {
    // SDL3 init
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    g_win = SDL_CreateWindow("VisualX Engine", WIDTH, HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (!g_win) { fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError()); return 1; }
    printf("[SDL3] Window created\n");

    // Vulkan
    create_instance();
    if (!SDL_Vulkan_CreateSurface(g_win, g_inst, NULL, &g_surf)) {
        fprintf(stderr, "SDL_Vulkan_CreateSurface failed\n"); return 1;
    }
    pick_device();
    create_swapchain();
    create_render_pass();
    create_framebuffers();
    create_pipeline();
    create_commands();
    printf("[Vulkan] Triangle pipeline ready\n");

    // LuaJIT inline test at startup
    run_lua_inline();
    printf("[LuaJIT] %s\n", g_lua_result);

    // Nuklear init
    struct nk_font_atlas *atlas;
    struct nk_context *nk = nk_sdl_init(g_win, g_dev, g_phys, g_qfam,
                                          g_views, g_img_count, g_fmt,
                                          NK_SDL_DEFAULT,
                                          512 * 1024, 128 * 1024);
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end(g_queue);
    printf("[Nuklear] UI ready\n");

    printf("\n[VisualX] All systems GO\n");

    int running = 1;
    SDL_Event evt;

    while (running) {
        nk_input_begin(nk);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_EVENT_QUIT) running = 0;
            if (evt.type == SDL_EVENT_KEY_DOWN && evt.key.key == SDLK_ESCAPE) running = 0;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(nk);

        // Nuklear UI
        if (nk_begin(nk, "VisualX Engine", nk_rect(10, 10, 320, 200),
                     NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE)) {

            nk_layout_row_dynamic(nk, 25, 1);
            nk_label(nk, g_lua_result, NK_TEXT_LEFT);

            nk_layout_row_dynamic(nk, 25, 1);
            nk_label(nk, g_lua_file, NK_TEXT_LEFT);

            nk_layout_row_dynamic(nk, 35, 2);

            if (nk_button_label(nk, "Run Lua")) {
                run_lua_inline();
            }

            if (nk_button_label(nk, "Open .lua file")) {
                const char *filters[] = { "*.lua" };
                const char *path = tinyfd_openFileDialog(
                    "Select a Lua script", "", 1, filters, "Lua scripts", 0);
                if (path) {
                    snprintf(g_lua_file, sizeof(g_lua_file), "%s", path);

                    lua_State *L = luaL_newstate();
                    luaL_openlibs(L);
                    if (luaL_dofile(L, path) == 0) {
                        const char *res = lua_tostring(L, -1);
                        snprintf(g_lua_result, sizeof(g_lua_result), "%s",
                                 res ? res : "Script ran OK");
                    } else {
                        snprintf(g_lua_result, sizeof(g_lua_result),
                                 "Error: %s", lua_tostring(L, -1));
                    }
                    lua_close(L);
                }
            }

            nk_layout_row_dynamic(nk, 35, 1);
            if (nk_button_label(nk, "Quit")) running = 0;
        }
        nk_end(nk);

        // Render: triangle first, then Nuklear on top
        VkSemaphore nk_sem = nk_sdl_render(g_queue, g_frame, VK_NULL_HANDLE, NK_ANTI_ALIASING_ON);
        draw_triangle(nk_sem);

        nk_sdl_handle_grab();
    }

    vkDeviceWaitIdle(g_dev);
    nk_sdl_shutdown();
    SDL_DestroyWindow(g_win);
    SDL_Quit();
    return 0;
}