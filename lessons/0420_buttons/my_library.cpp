
#include "my_library.h"

int is_pressed_once(int key)
{
    static int flag = 0;
    if (flag == 1) flag++;
    if (is_pressed(key) && flag == 0){
        flag = 1;
    }
    if (!is_pressed(key) && flag != 0)
        flag = 0;
    if (flag == 1)
        return 1;
    return 0;
}
