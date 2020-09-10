#include "competitor.h"

/**
 * @brief ��ȡһ���Ѿ���ʼ���ľ����߶���
 * @return һ�������ֶζ���0�ľ����߶���
 * @author fakejoychain
 * @date 2020��9��6��
 * @version 1.0.0
 */
struct sim_comp get_competitor() {
    struct sim_comp competitor = {0, {0}, {0}, {0}, {0}};
    return competitor;
}

/**
 * @brief �ӹ̶��ļ��ж�ȡ�����ߵ���Ϣ
 * @param out_comps_array Ϊstruct sim_comp*���ͱ����ĵ�ַ, ���ڸı�ָ��ָ��,
 * ��ȡ���ж��󼯺�, ��û�ж�����Ƿ��������򽫻Ὣ�˱�����NULL
 * @return ��ȡ�ɹ��򷵻ػ�ȡ���Ķ��󼯺ϵĸ���, ʧ�ܻ�û�ж���ʱ�򷵻�0
 * @author fakejoychain
 * @date 2020��9��6��
 * @version 1.0.0
 */
int read_competitors(struct sim_comp** out_comps_array) {
    if (out_comps_array == NULL) {
        printf("%s\n", "��ָ����������!");
        return 0;
    }

    int cnt = 0;
    FILE* file = NULL;

    if ((file = fopen(SFILE, "rb+")) == NULL) {
        /** ����ļ���ʧ�����ʾ�ļ������ڻ��ȡ���� */
        printf("%s:%d\n", "�ļ��򿪴���, ������", errno);
        cnt = 0;
        *out_comps_array = NULL;
    } else {
        struct sim_comp* cptr = NULL;
        struct sim_comp competitor = get_competitor();

        /** �����ļ��򿪳ɹ���ô��ʼ���Զ�ȡ���� */
        while (fread(&competitor, sizeof(competitors), 1, file) == 1) {
            /** ��¼������� */
            cnt++;
        }

        if (cnt > 0) {
            rewind(file);
            /** ����ж���, ��ô���Է����ڴ� */
            cptr = (struct sim_comp*)calloc(cnt, sizeof(competitors));

            if (cptr == NULL) {
                printf("%s\n", "�����ڴ����ʧ��!");
                cnt = 0;
                fclose(file);
                *out_comps_array = NULL;
                return cnt;
            }

            if (fread(cptr, sizeof(competitors), cnt, file) != cnt) {
                printf("%s\n", "�����ȡʧ��!");
                cnt = 0;

                /** ��ֹ�ڴ�й¶ */
                free(cptr);
                fclose(file);
                cptr = NULL;
                *out_comps_array = NULL;
                return cnt;
            }
            /** �޸�ָ��ָ�� */
            *out_comps_array = cptr;
        } else {
            printf("%s\n", "�޶�������!");
        }
        fclose(file);
    }
    return cnt;
}

/**
 * @brief ���и���id���������������ö����id
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ, ���ڳ�ʼ�������id��Ϣ
 * @author fakejoychain
 * @date 2020��9��6��
 * @version 1.0.0
 */
void set_next_comp_id(struct sim_comp* ref_comp) {
    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return;
    }

    int cnt = 0;
    struct sim_comp* comps_array = NULL;

    cnt = read_competitors(&comps_array);
    if (cnt == 0) {
        /** ����Ĭ�ϵ�idֵΪ10001, �Ժ����ε��� */
        ref_comp->id = 10001;
    } else {
        ref_comp->id = comps_array[cnt - 1].id + 1;
    }

    if (comps_array != NULL) {
        /** ��ֹ�ڴ�й© */
        free(comps_array);
        comps_array == NULL;
    }
}

/**
 * @brief ��ָ�����ö���(�����Ѿ���ʼ��)����Ϣ�����ļ�β��
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ, ���ڶ�ȡ��Ϣ������
 * @return ����������Ϣ��ӳɹ��򷵻�true,
 * ʧ�ܻ������ļ���ȡ����Ϣ�����򷵻�false
 * @author fakejoychain
 * @date 2020��9��7��
 * @version 1.0.0
 */
bool _append_competitor_ref(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return ret &= false;
    }

    FILE* file = fopen(SFILE, "ab+");
    if (file == NULL) {
        /** �ļ���ʧ��! */
        printf("%s\n", "�ļ��򿪴���!");
        return ret &= false;
    } else {
        if (fwrite(ref_comp, sizeof(competitors), 1, file) != 1) {
            printf("%s\n", "����д���ļ�ʧ��!");
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
 * @brief �������һ��ѧ��������Ϣ�����ļ�β��
 * @return ���ǵ�����ѧ��������ӳɹ��򷵻�true,
 * ʧ�ܻ������ļ���ȡ����Ϣ�����򷵻�false
 * @author fakejoychain
 * @date 2020��9��7��
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
        printf("%s:>>>\n", "�ڽ��ж���ֵʱ����������, �Ƿ��������ö���?");
    }
    ret &= _append_competitor_ref(&competitor);
    return ret;
}

/**
 * @brief �޸�ָ�����ö��������
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ
 * @return �޸������ɹ�����true, �޸�����ʧ��,
 * �û�ȡ���������ǻ��������ݶ�д���󶼷���false
 * @author fakejoychain
 * @date 2020��9��8��
 * @version 1.0.0
 */
bool adjust_name(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return ret &= false;
    }

    char buf[32] = {0};
    printf("%s\n", "�������û�����(16���ַ���):>>>");

    if (scanf("%31[^\n]s", buf) != 1) {
        printf("%s\n", "��ȡ����������!");
        return ret &= false;
    }

    /** �Ե��������Ĳ����Ļس� */
    getchar();

    size_t str_len = strlen(buf);
    if (str_len > 15) {
        printf("%s\n", "�ַ�������, �ضϴ���!");
    }

    printf("%s:>>\n", "��ȷ��������Ϣ");
    printf("����: %s\n", buf);
    printf("%s", "[Y/N (other or default is N)]:>>>");
    if ((buf[31] = getchar()) != 'Y' || buf[31] != 'y') {
        printf("%s\n", "�û�ȡ������!");
        return ret &= false;
    }
    memcpy(ref_comp->name, buf, str_len > 15 ? 15 : str_len);
    return ret;
}

/**
 * @brief �޸�ָ�����ö���İ༶��Ϣ
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ
 * @return �޸İ༶�ɹ�����true, �޸İ༶ʧ��,
 * �û�ȡ���������ǻ��������ݶ�д���󶼷���false
 * @author fakejoychain
 * @date 2020��9��8��
 * @version 1.0.0
 */
bool adjust_class(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return ret &= false;
    }

    char buf[48] = {0};
    printf("%s:>>>\n", "������༶��Ϣ");

    if (scanf("%47[^\n]s", buf) != 1) {
        printf("%s\n", "��ȡ���������ݴ���!");
        return ret &= false;
    }

    /** ������������Ļس� */
    getchar();

    size_t str_len = strlen(buf);
    if (str_len > 31) {
        printf("%s\n", "�ַ��������������ضϴ���!");
    }

    printf("%s\n", "��ȷ�ϰ༶��Ϣ:>>");
    printf("�༶: %s\n", buf);
    printf("%s", "[Y/N (other or default is N)]:>>>");

    if ((buf[47] = getchar()) != 'Y' || buf[47] != 'y') {
        printf("%s\n", "�û�ȡ������!");
        return ret &= false;
    }

    memcpy(ref_comp->_class, buf, str_len > 32 ? 32 : str_len);
    return ret;
}

/**
 * @brief �޸�ָ����ָ�����Ŀ�������
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ
 * @return �޸Ŀ���ɹ�����true, �޸Ŀ���ʧ��,
 * �û�ȡ���������ǻ��������ݶ�д���󶼷���false
 * @author fakejoychain
 * @date 2020��9��8��
 * @version 1.0.0
 */
bool adjust_topic_name(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return ret & false;
    }

    char buf[80] = {0};
    printf("%s", "�����������Ϣ:>>>");
    if (scanf("%79[^\n]s", buf) != 1) {
        printf("%s\n", "��ȡ���������ݳ���!");
        return ret &= false;
    }

    size_t str_len = strlen(buf);
    if (str_len > 63) {
        printf("%s\n", "�ַ��������������ضϴ���!");
    }

    printf("%s\n", "��ȷ�ϰ༶��Ϣ:>>");
    printf("��������:%s\n", buf);
    printf("%s", "[Y/N (other or default is N)]:>>>");

    if ((buf[80] = getchar()) != 'Y' || buf[80] != 'y') {
        printf("%s\n", "�û�ȡ������!");
        return ret &= false;
    }

    memcpy(ref_comp->topic_name, buf, str_len > 63 ? 63 : str_len);
    return ret;
}

/**
 * @brief �������һ���ʺϵķ�����ֵ��䵽������
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ, �����޸ķ���
 * @param srand ����ָ��, ����ԭ����srand������ַ, ����ʹ���������ɴ���NULL
 * @return ���غ��������ڴ������ֵ�ľ�̬�����ĵ�ַ, ͨ���Ըõ�ַ��
 * ���ʺ��޸Ŀ����ֶ��޸��´ε��޸ķ�����λ��
 * @author fakejoychain
 * @date 2020��9��9��
 * @version 1.0.0
 */
void* _adjust_score_sig(struct sim_comp* ref_comp,
                        void (*srand)(unsigned int)) {
    static size_t index;

    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return &index;
    }

    /** ����ֻ����ԭ����srand��������֤ÿ�����е������ */
    if (srand != NULL) {
        (*srand)((unsigned)time(NULL));
    }

    int score = 0;
    /** ����ѭ��������һ���������ֵ */
    while ((score = (unsigned)rand() % 100) < 50) {
        continue;
    }
    ref_comp->scores[abs(index) % 6] = (uint8_t)score;
    index++;
    return &index;
}

/**
 * @brief �޸�ָ����ָ�����Ŀ�������
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ
 * @return �޸ķ����ɹ�����true, �޸ķ���ʧ��, �����ڲ����󶼷���false
 * @author fakejoychain
 * @date 2020��9��8��
 * @version 1.0.0
 */
bool adjust_scores(struct sim_comp* ref_comp) {
    bool ret = true;

    if (ref_comp == NULL) {
        printf("%s\n", "��ָ����������!");
        return ret & false;
    }

    int cnt = 0;
    for (cnt = 0; cnt < 6; cnt++) {
        _adjust_score_sig(ref_comp, NULL);
    }
    return ret;
}

/**
 * @brief �޸�ָ�����ö�����Ϣ����
 * @param ref_comp Ϊstruct sim_comp���ͱ����ĵ�ַ, �����޸����ڲ���Ϣ
 * @return �������ö������Ϣ�����޸Ĳ��޸ĳɹ��򷵻�true,
 * �����û�ȡ���޸Ļ����޸�ʧ���򷵻�false
 * @author fakejoychain
 * @date 2020��9��8��
 * @version 1.0.0
 */
bool adjust_competitors_information(struct sim_comp* ref_comp) {
    bool ret = true;
    ret &= adjust_name(ref_comp);

    return ret;
}
