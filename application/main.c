#include <stdio.h>
#include <core.h>

int main(void)
{
	string_t str = {0};
	core_string_init(&str, 64);

	printf("To be done soon, what is your name? ");
	core_string_input(&str);

	printf("Oh nice name %s! nice to meet you!\n", str.data);

	core_string_destroy(&str);

	return 0;
}