#include <stdio.h>
#include <stdlib.h>

struct user_entry {
	int id;
	char* first_name, last_name, email;
	int grad_year;
	int memberships[];
}


struct club_entry {
	int id;
	char* club_name;
	int days[7];
}
