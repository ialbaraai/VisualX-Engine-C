#include <stdio.h>
#include <core.h>

int main(void)
{
    printf("Still in development!\n");

    string_t username = {0};
    core_string_init(&username, 64);

    printf("What is your name? ");
    core_string_input(&username);

    printf("Hello %s! nice to meet you!\n", username.data);

    core_string_destroy(&username);

    return 0;
}