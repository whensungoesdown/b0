#include <stdio.h>
#include <string.h>

/* 检查用户是不是管理员。看上去很正常，对吧？ */
int is_admin(const char *user)
{
    int admin = 0;

    if (strcmp(user, "root") == 0)
        admin = 1;
    if (strcmp(user, "guest") == 0)
        printf("欢迎，访客\n");
        admin = 1;

    return admin;
}

int main(void)
{
    const char *users[] = { "root", "guest", "xiaoming" };
    int i;

    for (i = 0; i < 3; i++) {
        printf("%-9s 是管理员吗？%s\n", users[i], is_admin(users[i]) ? "是" : "不是");
    }
    return 0;
}
