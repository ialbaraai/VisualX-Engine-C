#include <stdio.h>
#include <core.h>
#include <string.h>

void print_names(const size_t index, void* value)
{
	if (value) printf("\t\t- %zu. \"%s\"\n", index, ((string_t*)value)->data);
}

void add_name(vector_t* names_vector);
void adjust_name(vector_t* names_vector);

int main(void)
{
	file_t names_file = {0};
	core_file_init(&names_file, "names.txt", 16, 32);

	vector_t names_vector = {0};
	core_vector_init(&names_vector, 16, sizeof(string_t), core_string_copy_callback, core_string_destroy_callback);

	if (core_file_read(&names_file))
	{
		for (size_t i = 0; i < names_file.content.size; ++i)
		{
			core_vector_push_back(&names_vector, (string_t*)core_vector_get(&names_file.content, i));
		}
	}

	string_t option = {0};
	core_string_init(&option, 8);

	printf("-------- Names Manager --------\n");

	do
	{
		printf("\t1. List names\n\t2. Add name\n\t3. Adjust name\n\t4. Quit\nChoose an option: ");
		core_string_input_strict(&option, 4);

		if (strcmp(option.data, "1") == 0)
		{
			core_vector_foreach(&names_vector, print_names);
		}
		else if (strcmp(option.data, "2") == 0)
		{
			add_name(&names_vector);
		}
		else if (strcmp(option.data, "3") == 0)
		{
			adjust_name(&names_vector);
		}
		else if (strcmp(option.data, "4") == 0)
		{
			break;
		}
	} while (1);

	core_vector_clear(&names_file.content);

	for (size_t i = 0; i < names_vector.size; ++i)
	{
		core_vector_push_back(&names_file.content, (string_t*)core_vector_get(&names_vector, i));
	}

	core_file_write(&names_file);

	core_string_destroy(&option);
	core_vector_destroy(&names_vector);
	core_file_destroy(&names_file);

	return 0;
}

void add_name(vector_t* names_vector)
{
	if (names_vector)
	{
		string_t new_name = {0};
		core_string_init(&new_name, 32);

		printf("-------- Add Name --------\n");

		do
		{
			printf("\t\t- Enter new name: ");
			core_string_input(&new_name);
		} while (new_name.size < 1);
		
		core_vector_push_back(names_vector, &new_name);

		core_string_destroy(&new_name);
	}
}
void adjust_name(vector_t* names_vector)
{
	if (names_vector)
	{
		string_t target_name = {0};
		string_t new_name = {0};

		core_string_init(&target_name, 32);
		core_string_init(&new_name, 32);

		printf("-------- Adjust Name --------\n");

		do
		{
			printf("\t\t- Enter name to adjust: ");
			core_string_input(&target_name);	
		} while (target_name.size < 1);

		int index = -1;

		for (size_t i = 0; i < names_vector->size; ++i)
		{
			if (core_string_compare(&target_name, (string_t*)core_vector_get(names_vector, i)))
			{
				index = i;
				break;
			}
		}

		if (index != -1)
		{
			do
			{
				printf("\t\t- Enter new name: ");
				core_string_input(&new_name);
			} while (new_name.size < 1);
			
			core_vector_set(names_vector, index, &new_name);
		}

		core_string_destroy(&target_name);
		core_string_destroy(&new_name);
	}
}