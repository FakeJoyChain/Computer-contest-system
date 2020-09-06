#include "competitor.h"

/**
 * @brief 获取一个已经初始化的竞赛者对象
 * @return 一个所有字段都是0的竞赛者对象
 * @author fakejoychain
 * @date 2020年9月6日
 */
struct sim_comp get_competitor() {
    struct sim_comp competitor = {0, {0}, {0}, {0}, {0}};
    return competitor;
}

/**
 * @brief 从固定文件中读取参赛者的信息
 * @param out_comps_array 为struct sim_comp*类型变量的地址, 用于改变指针指向,
 * 获取所有对象集合, 若没有对象或是发生错误则将会将此变量置NULL
 * @return 获取成功则返回获取到的对象集合的个数, 失败或没有对象时则返回0
 * @author fakejoychain
 * @date 2020年9月6日
 */
int read_competitors(struct sim_comp** out_comps_array) {
    int cnt = 0;
    FILE* file = NULL;

    if ((file = fopen(SFILE, "rb+")) == NULL) {
        /** 如果文件打开失败则表示文件不存在或读取错误 */
        printf("%s:%d\n", "文件打开错误, 错误编号", errno);
        cnt = 0;
        *out_comps_array = NULL;
    } else {
        struct sim_comp* cptr = NULL;
        struct sim_comp competitor = get_competitor();

        /** 若是文件打开成功那么则开始尝试读取内容 */
        while (fread(&competitor, sizeof(competitors), 1, file) == 1) {
            /** 记录对象个数 */
            cnt++;
        }

        if (cnt > 0) {
            rewind(file);
            /** 如果有对象, 那么尝试分配内存 */
            cptr = (struct sim_comp*)calloc(cnt, sizeof(competitors));

            if (cptr == NULL) {
                printf("%s\n", "对象内存分配失败!");
                cnt = 0;
                fclose(file);
                *out_comps_array = NULL;
                return cnt;
            }

            if (fread(cptr, sizeof(competitors), cnt, file) != cnt) {
                printf("%s\n", "对象读取失败!");
                cnt = 0;

                /** 防止内存泄露 */
                free(cptr);
                fclose(file);
                cptr = NULL;
                *out_comps_array = NULL;
                return cnt;
            }
            /** 修改指针指向 */
            *out_comps_array = cptr;
        } else {
            printf("%s\n", "无对象数据!");
        }
        fclose(file);
    }
    return cnt;
}

