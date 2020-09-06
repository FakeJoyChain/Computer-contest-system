#include "competitor.h"

/**
 * @brief ��ȡһ���Ѿ���ʼ���ľ����߶���
 * @return һ�������ֶζ���0�ľ����߶���
 * @author fakejoychain
 * @date 2020��9��6��
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
 */
int read_competitors(struct sim_comp** out_comps_array) {
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

