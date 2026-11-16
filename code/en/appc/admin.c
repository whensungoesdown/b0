#include <stdio.h>
#include <string.h>

/* Check whether a user is an administrator. Looks fine, right? */
int is_admin(const char *user)
{
    int admin = 0;

    if (strcmp(user, "root") == 0)
        admin = 1;
    if (strcmp(user, "guest") == 0)
        printf("Welcome, guest\n");
        admin = 1;

    return admin;
}

int main(void)
{
    const char *users[] = { "root", "guest", "alice" };
    int i;

    for (i = 0; i < 3; i++) {
        printf("%-9s admin? %s\n", users[i], is_admin(users[i]) ? "yes" : "no");
    }
    return 0;
}
