#include "competitor.h"

/**
 * @brief 获取一个已经初始化的竞赛者对象
 * @return 一个所有字段都是0的竞赛者对象
 * @author fakejoychain
 * @date 2020年9月6日
 * @version 1.0.0
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
 * @version 1.0.0
 */
int read_competitors(struct sim_comp** out_comps_array) {
    if (out_comps_array == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return 0;
    }

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

/**
 * @brief 自行根据id增长规则设置引用对象的id
 * @param ref_comp 为struct sim_comp类型变量的地址, 用于初始化对象的id信息
 * @author fakejoychain
 * @date 2020年9月6日
 * @version 1.0.0
 */
void set_next_comp_id(struct sim_comp* ref_comp) {
    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return;
    }

    int cnt = 0;
    struct sim_comp* comps_array = NULL;

    cnt = read_competitors(&comps_array);
    if (cnt == 0) {
        /** 设置默认的id值为10001, 以后依次递增 */
        ref_comp->id = 10001;
    } else {
        ref_comp->id = comps_array[cnt - 1].id + 1;
    }

    if (comps_array != NULL) {
        /** 防止内存泄漏 */
        free(comps_array);
        comps_array == NULL;
    }
}

/**
 * @brief 将指针引用对象(并且已经初始化)的信息填入文件尾部
 * @param ref_comp 为struct sim_comp类型变量的地址, 用于读取信息等内容
 * @return 若是引用信息添加成功则返回true,
 * 失败或由于文件读取等信息错误则返回false
 * @author fakejoychain
 * @date 2020年9月7日
 * @version 1.0.0
 */
bool _append_competitor_ref(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return ret &= false;
    }

    FILE* file = fopen(SFILE, "ab+");
    if (file == NULL) {
        /** 文件打开失败! */
        printf("%s\n", "文件打开错误!");
        return ret &= false;
    } else {
        if (fwrite(ref_comp, sizeof(competitors), 1, file) != 1) {
            printf("%s\n", "对象写入文件失败!");
            fclose(file);
            return ret &= false;
        }
    }

    if (file != NULL) {
        fclose(file);
        file == NULL;
    }

    return ret;
}

/**
 * @brief 单独添加一个学生对象信息填入文件尾部
 * @return 若是单独的学生对象添加成功则返回true,
 * 失败或由于文件读取等信息错误则返回false
 * @author fakejoychain
 * @date 2020年9月7日
 * @version 1.0.0
 */
bool _append_competitor_none() {
    bool ret = true;
    struct sim_comp competitor = get_competitor();
    set_next_comp_id(&competitor);

    ret &= adjust_name(&competitor);
    ret &= adjust_class(&competitor);
    ret &= adjust_scores(&competitor);
    ret &= adjust_topic_name(&competitor);
    if (ret == false) {
        printf("%s:>>>\n", "在进行对象赋值时产生过错误, 是否继续保存该对象?");
    }
    ret &= _append_competitor_ref(&competitor);
    return ret;
}

/**
 * @brief 修改指针引用对象的姓名
 * @param ref_comp 为struct sim_comp类型变量的地址
 * @return 修改姓名成功返回true, 修改姓名失败,
 * 用户取消操作或是缓冲区数据读写错误都返回false
 * @author fakejoychain
 * @date 2020年9月8日
 * @version 1.0.0
 */
bool adjust_name(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return ret &= false;
    }

    char buf[32] = {0};
    printf("%s\n", "请输入用户姓名(16个字符内):>>>");

    if (scanf("%31[^\n]s", buf) != 1) {
        printf("%s\n", "读取缓冲区错误!");
        return ret &= false;
    }

    /** 吃掉缓冲区的残留的回车 */
    getchar();

    size_t str_len = strlen(buf);
    if (str_len > 15) {
        printf("%s\n", "字符串过长, 截断处理!");
    }

    printf("%s:>>\n", "请确认姓名信息");
    printf("姓名: %s\n", buf);
    printf("%s", "[Y/N (other or default is N)]:>>>");
    if ((buf[31] = getchar()) != 'Y' || buf[31] != 'y') {
        printf("%s\n", "用户取消输入!");
        return ret &= false;
    }
    memcpy(ref_comp->name, buf, str_len > 15 ? 15 : str_len);
    return ret;
}

/**
 * @brief 修改指针引用对象的班级信息
 * @param ref_comp 为struct sim_comp类型变量的地址
 * @return 修改班级成功返回true, 修改班级失败,
 * 用户取消操作或是缓冲区数据读写错误都返回false
 * @author fakejoychain
 * @date 2020年9月8日
 * @version 1.0.0
 */
bool adjust_class(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return ret &= false;
    }

    char buf[48] = {0};
    printf("%s:>>>\n", "请输入班级信息");

    if (scanf("%47[^\n]s", buf) != 1) {
        printf("%s\n", "读取缓冲区数据错误!");
        return ret &= false;
    }

    /** 清除掉缓冲区的回车 */
    getchar();

    size_t str_len = strlen(buf);
    if (str_len > 31) {
        printf("%s\n", "字符串过长将发生截断处理!");
    }

    printf("%s\n", "请确认班级信息:>>");
    printf("班级: %s\n", buf);
    printf("%s", "[Y/N (other or default is N)]:>>>");

    if ((buf[47] = getchar()) != 'Y' || buf[47] != 'y') {
        printf("%s\n", "用户取消输入!");
        return ret &= false;
    }

    memcpy(ref_comp->_class, buf, str_len > 32 ? 32 : str_len);
    return ret;
}

/**
 * @brief 修改指针所指向对象的课题名称
 * @param ref_comp 为struct sim_comp类型变量的地址
 * @return 修改课题成功返回true, 修改课题失败,
 * 用户取消操作或是缓冲区数据读写错误都返回false
 * @author fakejoychain
 * @date 2020年9月8日
 * @version 1.0.0
 */
bool adjust_topic_name(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return ret & false;
    }

    char buf[80] = {0};
    printf("%s", "请输入课题信息:>>>");
    if (scanf("%79[^\n]s", buf) != 1) {
        printf("%s\n", "读取缓冲区数据出错!");
        return ret &= false;
    }

    size_t str_len = strlen(buf);
    if (str_len > 63) {
        printf("%s\n", "字符串过长将发生截断处理!");
    }

    printf("%s\n", "请确认班级信息:>>");
    printf("课题名称:%s\n", buf);
    printf("%s", "[Y/N (other or default is N)]:>>>");

    if ((buf[80] = getchar()) != 'Y' || buf[80] != 'y') {
        printf("%s\n", "用户取消输入!");
        return ret &= false;
    }

    memcpy(ref_comp->topic_name, buf, str_len > 63 ? 63 : str_len);
    return ret;
}

/**
 * @brief 随机产生一个适合的分数数值填充到分数区
 * @param ref_comp 为struct sim_comp类型变量的地址, 用于修改分数
 * @param srand 函数指针, 传入原本的srand函数地址, 若不使用随机数则可传入NULL
 * @return 返回函数中用于处理分数值的静态变量的地址, 通过对该地址的
 * 访问和修改可以手动修改下次的修改分数的位置
 * @author fakejoychain
 * @date 2020年9月9日
 * @version 1.0.0
 */
void* _adjust_score_sig(struct sim_comp* ref_comp,
                        void (*srand)(unsigned int)) {
    static size_t index;

    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return &index;
    }

    /** 这里只传入原本的srand函数来保证每次运行的随机性 */
    if (srand != NULL) {
        (*srand)((unsigned)time(NULL));
    }

    int score = 0;
    /** 利用循环来产生一个合理的数值 */
    while ((score = (unsigned)rand() % 100) < 50) {
        continue;
    }
    ref_comp->scores[abs(index) % 6] = (uint8_t)score;
    index++;
    return &index;
}

/**
 * @brief 修改指针所指向对象的课题名称
 * @param ref_comp 为struct sim_comp类型变量的地址
 * @return 修改分数成功返回true, 修改分数失败, 或是内部错误都返回false
 * @author fakejoychain
 * @date 2020年9月8日
 * @version 1.0.0
 */
bool adjust_scores(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "空指针对象的引用!");
        return ret & false;
    }

    int cnt = 0;
    for (cnt = 0; cnt < 6; cnt++) {
        _adjust_score_sig(ref_comp, NULL);
    }
    return ret;
}

/**
 * @brief 修改指针引用对象信息内容
 * @param ref_comp 为struct sim_comp类型变量的地址, 用于修改其内部信息
 * @return 若是引用对象的信息发生修改并修改成功则返回true,
 * 若是用户取消修改或者修改失败则返回false
 * @author fakejoychain
 * @date 2020年9月8日
 * @version 1.0.0
 */
bool adjust_competitors_information(struct sim_comp* ref_comp) {
    bool ret = true;
    ret &= adjust_name(ref_comp);

    return ret;
}
