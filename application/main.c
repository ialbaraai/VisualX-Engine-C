#include <stdio.h>
#include <core.h>

int main(void)
{
	string_t name = {0};
	core_string_init(&name, 64);

	printf("Still in progress! What is your name? ");
	core_string_input(&name);

	printf("Hello %s, nice name!\n", name.data);

	core_string_destroy(&name);
}