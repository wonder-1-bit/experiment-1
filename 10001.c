#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义学生节点结构体
typedef struct Student {
    char name[50];  // 学生姓名
    int score;      // 学生成绩
    struct Student *next; // 指向下一个节点的指针
    struct Student *pre; //指向上一个节点的指针
} Student;

// 从文件加载学生信息并构建链表
Student *loadFromFile(const char *filename) {
    Student *head = NULL;
     /*1.尝试以“只读”方式打开文件，如文件不存在输出提示信息并返回*/
    FILE *fp;
    char tmp_name[50];
    int tmp_score;
    if((fp = fopen(filename,"r")) == NULL)
    {
        printf("Failure to open the file!\n");
        exit(EXIT_FAILURE);
    }
   while(fscanf(fp, "%s %d", tmp_name, &tmp_score) == 2)
   {
        Student *p = NULL, *pr = head;
        p = (Student*)malloc(sizeof(Student));
        if(p == NULL)
        {
            printf("No enough memory to allocate!\n");
            exit(EXIT_FAILURE);
        }
        strcpy(p->name,tmp_name);
        p->score = tmp_score;
        //2.从文件中逐行读取学生信息并创建链表节点，并将链表节点挂载到链表
       //        - 如果链表为空，将新节点设置为链表头
      //       - 如果链表非空，将新节点挂到链表尾部
        if(head == NULL)
        {
            head = p;
        }
        else
        {
            while(pr->next != NULL)
            {
                pr->pre = pr;
                pr = pr->next;
            }
            pr->next = p;
        }
        p->next = NULL;
        p->pre = pr;
    }
    //3.关闭文件
    fclose(fp);
    //4.返回链表的头节点
    return head;
}

// 将链表中的学生信息保存到文件
void saveToFile(const char *filename, Student *head) {
    // 1.尝试以写入模式打开文件，如文件无法打开则提示信息并返回
    FILE *fp;
     if((fp = fopen(filename,"w")) == NULL)
    {
        printf("Failure to open the file!\n");
        exit(EXIT_FAILURE);
    }
    //2.遍历链表并写入文件
    Student *cur = head;
     if(head == NULL)
        {
            printf("No information in the system!");
        }
        else
        {
            fprintf(fp, "%s %d\n", cur->name, cur->score);
            while(cur->next != NULL)
            {
                cur->pre = cur;
                cur = cur->next;
                fprintf(fp, "%s %d\n", cur->name, cur->score);
            }
        }
        //3.关闭文件
        fclose(fp);
}

// 根据姓名查找学生成绩
Student *findStudent(Student *head, const char *name) {
    //1. 从链表头开始遍历，逐一检查每个节点：
    // - 如果找到匹配节点，返回该节点指针
    // - 如果遍历完成后未找到符合要求的节点，返回 NULL
    Student *p = head;
    while(p != NULL)
    {
        if(strcmp(p->name, name) == 0)
        return p;
        p->pre = p;
        p = p->next;
    }
    return NULL;
}

// 添加新学生到链表
void addStudent(Student **head, const char *name, int score) {
    //1. 创建一个新学生节点，初始化节点的姓名、成绩，并将其后续指针置为 NULL。
    Student *p = NULL, *pr = *head;
    p = (Student*)malloc(sizeof(Student));
    if(p == NULL)
     {
        printf("No enough memory to allocate!\n");
        exit(EXIT_FAILURE);
     }
    p->score = score;
    strcpy(p->name, name);
    p->next = NULL;
    p->pre = NULL;
    pr->pre = NULL;
    //2. 检查链表是否为空：
    //       - 如果为空，将新节点设置为头节点。
    //       - 如果链表不为空，遍历链表直到尾部，并将新节点连接到链表末尾。
   if(*head == NULL)
        {
            *head = p;
        }
        else
        {
            while(pr->next != NULL)
            {
                pr->pre = pr;
                pr = pr->next;
            }
            pr->next = p;
            p->pre = pr;
        }
}

// 删除学生节点
void deleteStudent(Student **head, const char *name) {
    //1. 从链表头开始遍历，寻找目标学生节点，若找到：
    //          - 如果目标节点是头节点，则调整头指针
    //          - 如果目标节点是中间或尾部节点，则将其后续节点链接到其前序节点
    //           - 释放该节点
    Student *tmp = *head;
        if(strcmp(name, (*head)->name) == 0)
        {
            *head = (*head)->next;
            free(tmp);
        }
        // 遍历链表寻找要删除的节点
       Student *cur = *head;
       cur->pre = NULL;
        while(cur != NULL && strcmp(cur->name,name) != 0){
            cur->pre = cur;
            cur = cur->next;
        }
        //如果找到了节点
        if(cur != NULL){
            cur->pre->next = cur->next;
            free(cur);
            printf("Student deleted");
        }else{
            //如果未找到目标节点，输出提示信息
            printf("Don't find the student!");
        }
}

int main() {
    const char *filename = "class_scores.txt";
    // 加载文件中的学生信息
    Student *head = loadFromFile(filename);

    int option;
    char name[50];
    int score;

    while (1) {
        printf("\nOptions:\n");
        printf("1. Find student score\n");
        printf("2. Add new students\n");
        printf("3. Delete a student\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Enter student name: ");
                scanf("%s", name);
                Student *tar_stu = findStudent(head, name);
                printf("%s's score: %d\n", name, tar_stu->score);
                /*
                *   TODO：
                *   调用findStudent获得指定学生的成绩，并显示
                */

                break;

            case 2: {
                int count;
                printf("How many students do you want to add? ");
                scanf("%d", &count);
                for (int i = 0; i < count; i++) {
                    printf("Enter student %d name and score (name score): ", i + 1);
                    scanf("%s %d", name, &score);
                    addStudent(&head, name, score);
                    /*
                    *   TODO：
                    *   调用addStudent新增学生成绩
                    */

                }
                printf("Students added successfully.\n");
                break;
            }

            case 3:
                printf("Enter student name to delete: ");
                scanf("%s", name);
                deleteStudent(&head, name);
                /*
                *   TODO：
                *   调用deleteStudent删除指定学生
                */
                printf("\n");
                break;

            case 4:
            saveToFile(filename, head);
                /*
                *   TODO：
                *   调用saveToFile将链表信息保存回文件
                *   并释放链表空间
                */

                printf("Data saved. Exiting program.\n");
                return 0;

            default:
                printf("Invalid option. Please try again.\n");
        }
    }
}