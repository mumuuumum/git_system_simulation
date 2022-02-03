#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define len_cmt_name 140
#define num_cmt_file 10000
#define len_file_name 140
#define len_file_content 150
#define num_newest_point 5000
#define num_collect_files 10000
#define len_command 200
#define ll long long
//提交的结构体
struct Commit{
    struct Commit *Commit_one;
    struct Commit *Commit_two;
    char Commit_name [len_cmt_name];
    struct File *(Content [num_cmt_file]);
    int new_point_id;
    int top;
    int flag;
};
//文件的结构体
struct File{
    int Time;
    char Name [len_file_name];
    int start_point;
    char Content [len_file_content];
};

struct Newest_point {
    struct Commit *(point [num_newest_point]);
    int top;
};

struct Content {
    struct File *(file_address [num_collect_files]);
    int top;
};

struct Newest_point Newest_point;
struct Commit *Head = NULL;
struct Commit Staging_area;
char command_str [len_command];
int timex;
int in_stage;
int acnt = 0;

void Initial_Newest_point ();
void Initial_Commit (struct Commit *Commit);
void Initial_File (struct File *File);
struct File *Search_File (char *Name_searched);
struct File *Search_Staging (char *Name_searched);
struct File *Search_Common (char *Name_searched,struct Commit *Commit);
void Part_strcpy (char *str_one,const char *str_two,int Start_point,int End_point);
int str_to_num (const char *str,int Start_point,int End_point);
void Write (char *filename,int offset,int len);
void Read (char *filename,int offset,int len);
void Unlink (char *filename);
void Ls ();
void Collect_file (struct Content *content,struct Commit *commit);
void Init_flag (struct Commit *commit);
void Commit (char *commit_name);
struct Commit *Search_Commit (char *Name_searched);
struct Commit *Search_Point (struct Commit *Commit, char *Name_searched);
int Staging_is_empty ();
void Checkout (char *commit_name);
void Merge (char *mergee,char *commit_name);
int File_is_deleted (int dark_loca,struct File *Address,struct Content *content,int *last_loca,int *arr);

int main () {
/*#ifndef ONLINE_JUDGE // 只有在本地需要提示程序运行时间、通过文件输入输出
    freopen("input.txt", "r", stdin); //输入写入 "input.txt" 文件中
    freopen("output.txt", "w", stdout);//输出到 "output.txt" 文件中
    ll _begin_time = clock(); //记录开始时间
#endif*/
    Initial_Commit(&Staging_area);
    Initial_Newest_point ();

    int command_number = 0;
    scanf("%d", &command_number);

    getchar();
    for (int i = 0; i < command_number ; i++,acnt++) {
        memset(command_str,0,len_command * sizeof(char));
        gets(command_str);
        /*if (strcmp(command_str,"read filename22 143538 39") == 0) {
            printf("\n%d\n",i);
            exit(0);
        }*/
        char filename [len_file_name];
        char cmtname [len_cmt_name];
        int offset;
        int len;
        int Start_point;
        int End_point;
        memset(filename,0,len_file_name * sizeof(char));
        memset(cmtname,0,len_cmt_name * sizeof(char));
        switch (command_str[0]) {
            case 'w':
                //write
                for (Start_point = 6,End_point = 6; command_str [++End_point] != ' ' ;);
                End_point--;
                Part_strcpy(filename,command_str,Start_point,End_point);
                for (Start_point = End_point + 2,End_point += 2; command_str [++End_point] != ' ';);
                End_point--;
                offset = str_to_num(command_str,Start_point,End_point);
                for (Start_point = End_point + 2,End_point += 2; command_str [++End_point] != '\0';);
                End_point--;
                len = str_to_num(command_str,Start_point,End_point);
                Write(filename,offset,len);
                break;
            case 'r':
                //cnt++;
                //read
                for (Start_point = 5,End_point = 7; command_str [++End_point] != ' ' ;);
                End_point--;
                Part_strcpy(filename,command_str,Start_point,End_point);
                for (Start_point = End_point + 2,End_point += 2; command_str [++End_point] != ' ';);
                End_point--;
                offset = str_to_num(command_str,Start_point,End_point);
                for (Start_point = End_point + 2,End_point += 2; command_str [++End_point] != '\0';);
                End_point--;
                len = str_to_num(command_str,Start_point,End_point);
                Read(filename,offset,len);
                break;
            case 'u':
                //unlink
                for (Start_point = 7,End_point = 7;command_str [++End_point] != '\0' ;);
                End_point--;
                Part_strcpy(filename,command_str,Start_point,End_point);
                Unlink(filename);
                break;
            case 'l':
                //cnt++;
                Staging_area.Commit_one = Head;
                Ls();
                Staging_area.Commit_one = NULL;
                break;
            case 'c':
                switch (command_str[1]) {
                    case 'o':
                        for (Start_point = 7,End_point = 7;command_str [++End_point] != '\0' ;);
                        End_point--;
                        Part_strcpy(filename,command_str,Start_point,End_point);
                        Commit(filename);
                        break;
                    case 'h':
                        for (Start_point = 9,End_point = 9;command_str [++End_point] != '\0' ;);
                        End_point--;
                        Part_strcpy(filename,command_str,Start_point,End_point);
                        Checkout(filename);
                }
                break;
            case 'm':
                for (Start_point = 6,End_point = 6;command_str [++End_point] != ' ' ;);
                End_point--;
                Part_strcpy(filename,command_str,Start_point,End_point);
                for (Start_point = End_point + 2,End_point += 2; command_str [++End_point] != '\0';);
                End_point--;
                Part_strcpy(cmtname,command_str,Start_point,End_point);
                Merge(filename,cmtname);
        }
    }
/*#ifndef ONLINE_JUDGE
    ll _end_time = clock();
    printf("time = %ld ms\n", _end_time - _begin_time);
#endif*/
    return 0;
}
void Initial_Newest_point () {
    for (int i = 0; i < num_newest_point ; i++) {
        Newest_point.point [i] = NULL;
    }
    Newest_point.top = -1;
}
void Initial_Commit (struct Commit *Commit) {
    Commit->Commit_one = NULL;
    Commit->Commit_two = NULL;
    memset(Commit->Commit_name,0,len_cmt_name * sizeof(char));
    for (int i = 0; i < num_cmt_file ; i++) {
        Commit->Content [i] = NULL;
    }
    Commit->top = -1;
    Commit->new_point_id = -1;
    Commit->flag = 0;
}
void Initial_File (struct File *File) {
    File->Time = 0;
    File->start_point = -1;
    memset(File->Name,0,len_file_name);
    memset(File->Content,0,len_file_content);
}
struct File *Search_File (char *Name_searched) {
    struct File *Address = NULL;
    Address = Search_Staging (Name_searched);
    if (Address != NULL) {
        in_stage = 1;
        return Address;
    }
    if (Head == NULL) {
        return NULL;
    }
    Address = Search_Common(Name_searched,Head);
    if (Address != NULL) {
        in_stage = 0;
        return Address;
    }
    return NULL;
}
struct File *Search_Staging (char *Name_searched) {
    for (int i = 0; i <= Staging_area.top ; i++) {
        if (strcmp((Staging_area.Content [i])->Name,Name_searched) == 0) {
            return Staging_area.Content [i];
        }
        if ((Staging_area.Content [i])->Name [0] == '-') {
            for (int j = 1;(Staging_area.Content [i])->Name [j] != 0;j++) {
                if ((Staging_area.Content [i])->Name [j] != Name_searched [j - 1]) {
                    break;
                }
                if (j == strlen(Name_searched) && (Staging_area.Content [i])->Name [j + 1] == 0) {
                    return Staging_area.Content [i];
                }
            }
        }
    }
    return NULL;
}
struct File *Search_Common (char *Name_searched,struct Commit *Commit) {
    for (int i = 0; i <= Commit->top ; i++) {
        if (strcmp((Commit->Content [i])->Name,Name_searched) == 0) {
            return Commit->Content [i];
        }
        if ((Commit->Content [i])->Name [0] == '-') {
            for (int j = 1;(Commit->Content [i])->Name [j] != 0;j++) {
                if ((Commit->Content [i])->Name [j] != Name_searched [j - 1]) {
                    break;
                }
                if (j == strlen(Name_searched) && (Commit->Content [i])->Name [j + 1] == 0) {
                    return Commit->Content [i];
                }
            }
        }
    }
    struct File *Address_searched_one = NULL;
    if (Commit->Commit_one != NULL) {
        Address_searched_one = Search_Common(Name_searched,Commit->Commit_one);
    }
    struct File *Address_searched_two = NULL;
    if (Commit->Commit_two != NULL) {
        Address_searched_two = Search_Common(Name_searched,Commit->Commit_two);
    }
    if (Address_searched_one != NULL && Address_searched_two != NULL) {
        if (Address_searched_one->Time > Address_searched_two->Time) {
            return Address_searched_one;
        } else {
            return Address_searched_two;
        }
    }
    if (Address_searched_one == NULL && Address_searched_two != NULL) {
        return Address_searched_two;
    }
    if (Address_searched_two == NULL && Address_searched_one != NULL) {
        return Address_searched_one;
    }
    if (Address_searched_one == NULL && Address_searched_two == NULL) {
        return NULL;
    }
}
void Part_strcpy (char *str_one,const char *str_two,int Start_point,int End_point) {
    for (int i = Start_point,j = 0 ; i <= End_point ; i++,j++) {
        str_one [j] = str_two [i];
    }
}
int str_to_num (const char *str,int Start_point,int End_point) {
    int result = 0;
    for (int i = End_point, j = 1;i >= Start_point;i--,j *= 10) {
        result += (str [i] - ('0' - 0)) * j;
    }
    return result;
}
void Write (char *filename,int offset,int len) {
    struct File *Address = Search_File(filename);
    if (Address != NULL) {
        if (in_stage == 1) {
            if (Address->Name [0] == '-') {
                for (int i = 0;;i++) {
                    if (Staging_area.Content [i] == Address) {
                        memset(Address->Name,0,len_file_name * sizeof(char));
                        Part_strcpy(Address->Name,filename,0,len_file_name - 1);
                        break;
                    }
                }
            }
        } else {
            Staging_area.Content [++Staging_area.top] = malloc(sizeof(struct File));
            Initial_File(Staging_area.Content [Staging_area.top]);
            Part_strcpy(Staging_area.Content [Staging_area.top]->Content,Address->Content,0, strlen(Address->Content) - 1);
            Part_strcpy(Staging_area.Content [Staging_area.top]->Name,filename,0, strlen(filename) - 1);
            Address = Staging_area.Content [Staging_area.top];
        }
    } else {
        Address = malloc(sizeof(struct File));
        Staging_area.Content [++Staging_area.top] = Address;
        Initial_File(Address);
        strcpy(Address->Name,filename);
    }
    /*if (offset > strlen(Address->Content)) {
        for (int i = strlen(Address->Content); i < offset;i++) {
            Address->Content [i] = '.';
        }
    }*/
    Address->start_point = offset;
    char *tmp_str = malloc((len + 1) * sizeof(char));
    memset(tmp_str,0,(len + 1) * sizeof(char));
    gets(tmp_str);
    for (int i = 0 , j = 0 ; i < strlen(tmp_str) ; i++ , j++) {
        Address->Content [j] = tmp_str [i];
    }
    free(tmp_str);
    Address->Time = timex;
}
void Read (char *filename,int offset,int len) {
    struct File *Address = Search_File(filename);
    if (Address != NULL && Address->Name [0] != '-') {
        int i;
        int j = 0;
        for (i = 0 ; (i < Address->start_point - offset) && (i < len); i++) {
            printf(".");
        }
        if (Address->start_point - offset < 1) {
            for (j = offset - Address->start_point ;i < len && j < strlen(Address->Content);i++,j++) {
                printf("%c",Address->Content [j]);
            }
        } else {
            for (;i < len && j < strlen(Address->Content);i++,j++) {
                printf("%c",Address->Content [j]);
            }
        }
        for (;i < len;i++) {
            printf(".");
        }
    } else {
        for (int i = 0; i < len ; i++) {
            printf(".");
        }
    }
    printf("\n");
}
void Unlink (char *filename) {
    struct File *Address = Search_File(filename);
    if (Address != NULL && Address->Name [0] != '-') {
        if (in_stage == 1) {
            memset(Address->Name,0,len_file_name * sizeof(char));
            Address->Name [0] = '-';
            strcat(Address->Name,filename);
            memset(Address->Content,0, strlen(Address->Content) * sizeof(char));
        } else {
            Staging_area.Content [++Staging_area.top] = malloc(sizeof(struct File));
            Initial_File(Staging_area.Content [Staging_area.top]);
            Staging_area.Content [Staging_area.top]->Name [0] = '-';
            strcat(Staging_area.Content [Staging_area.top]->Name,filename);
            Staging_area.Content [Staging_area.top]->Time = timex;
        }
    }
}
void Ls () {
    struct Content *content = malloc(sizeof(struct Content));
    content->top = -1;
    for (int i = 0 ; i < num_collect_files ; i++) {
        content->file_address [i] = NULL;
    }
    int content_num = 0;

    Collect_file(content,&Staging_area);
    int *arr = malloc((content->top + 1) * sizeof(int));
    for (int i = 0 ; i <= content->top ; i++) {
        arr [i] = i;
    }
    for (int i = 0; i < content->top; i++) {
        int min = i;
        for (int j = i + 1; j <= content->top ; j++) {
            if (strcmp(content->file_address [arr [min]]->Name, content->file_address [arr [j]]->Name) > 0) {
                min = j;
            }
        }
        int tmp = arr [min];
        arr [min] = arr [i];
        arr [i] = tmp;
    }

    int dark_loca = -1;
    int last_loca = 0;

    if (content->top >= 1) {
        for (int i = 0 ; i < content->top ; i++) {
            if (content->file_address [arr [i]]->Name [0] == '-' && content->file_address [arr [i + 1]]->Name [0] != '-') {
                dark_loca = i;
                break;
            }
        }
        if (content->file_address [arr [content->top]]->Name [0] == '-') {
            dark_loca = content->top;
        }
    } else if (content->top == 0) {
        if (content->file_address [arr [content->top]]->Name [0] == '-') {
            dark_loca = content->top;
        }
    }

    int low = -1,high = -1;
    if (content->top > 0) {
        if (dark_loca == content->top - 1) {
            if (File_is_deleted(dark_loca,content->file_address [arr [content->top]],content,&last_loca,arr) == 0) {
                content_num++;
                low = content->top;
                high = content->top;
            }
        } else if (dark_loca == content->top - 2) {
            int tmp_arr [2] = {-2, -2};
            if (File_is_deleted(dark_loca,content->file_address [arr [content->top - 1]],content,&last_loca,arr) == 0) {
                tmp_arr [0] = content->top - 1;
                content_num++;
            }
            if (File_is_deleted(dark_loca,content->file_address [arr [content->top]],content,&last_loca,arr) == 0) {
                tmp_arr [1] = content->top;
                content_num++;
            }
            if (tmp_arr [0] != -2) {
                low = tmp_arr [0];
            } else if (tmp_arr [1] != -2) {
                low = tmp_arr [1];
            }
            if (tmp_arr [1] != -2) {
                high = tmp_arr [1];
            } else if (tmp_arr [0] != -2) {
                high = tmp_arr [0];
            }
        } else {
            int tmp = -1;
            for (int i = dark_loca + 1 ; i < content->top - 1;i++) {
                if (File_is_deleted(dark_loca,content->file_address [arr [i]],content,&last_loca,arr) == 1) {
                    continue;
                } else {
                    if (low == -1) {
                        low = i;
                    }
                    tmp = i;
                    content_num++;
                }
            }
            int tmp_arr [2] = {-2,-2};
            if (File_is_deleted(dark_loca,content->file_address [arr [content->top - 1]],content,&last_loca,arr) == 0) {
                tmp_arr [0] = content->top - 1;
                content_num++;
            }
            if (File_is_deleted(dark_loca,content->file_address [arr [content->top]],content,&last_loca,arr) == 0) {
                tmp_arr [1] = content->top;
                content_num++;
            }
            if (low == -1) {
                if (tmp_arr [0] != -2) {
                    low = tmp_arr [0];
                } else if (tmp_arr [1] != -2) {
                    low = tmp_arr [1];
                }
            }
            if (tmp_arr [1] != -2) {
                high = tmp_arr [1];
            } else if (tmp_arr [0] != -2) {
                high = tmp_arr [0];
            } else {
                high = tmp;
            }
        }
    } else if (content->top == 0) {
        if (File_is_deleted(dark_loca,content->file_address [arr [content->top]],content,&last_loca,arr) == 0) {
            content_num++;
            low = content->top;
            high = content->top;
        }
    }
    if (command_str [2] == 'a') {
        for (int i = 0 ; i <= content->top ; i++) {
            printf("%s %d %c\n",content->file_address [arr [i]]->Name,content->file_address [arr [i]]->Time,Search_File(content->file_address [arr [i]]->Name)->Name [0]);
        }
    } else {
        if (content_num == 0) {
            printf("0");
        } else {
            printf("%d %s %s", content_num, content->file_address[arr[low]]->Name,
                   content->file_address[arr[high]]->Name);
        }
        printf("\n");
        free(arr);
        free(content);
    }
    Init_flag(&Staging_area);
}
void Collect_file (struct Content *content,struct Commit *commit) {
    for (int i = 0 ; i <= commit->top ; i++) {
        for (int j = 0 ; j <= content->top ;j++) {
            if (content->file_address [j] ==  commit->Content [i] || strcmp(content->file_address [j]->Name,commit->Content [i]->Name) == 0) {
                goto here;
            }
        }
        content->file_address [++(content->top)] = commit->Content [i];
        here:;
    }
    commit->flag = 1;
    if (commit->Commit_one != NULL && commit->Commit_one->flag != 1) {
        Collect_file (content,commit->Commit_one);
    }
    if (commit->Commit_two != NULL && commit->Commit_two->flag != 1) {
        Collect_file(content,commit->Commit_two);
    }
}
void Init_flag (struct Commit *commit) {
    commit->flag = 0;
    if (commit->Commit_one != NULL && commit->Commit_one->flag == 1) {
        Init_flag(commit->Commit_one);
    }
    if (commit->Commit_two != NULL && commit->Commit_two->flag == 1) {
        Init_flag(commit->Commit_two);
    }
}
void Commit (char *commit_name) {
    if (Search_Commit(commit_name) != NULL || Staging_is_empty() == 1) {
        return;
    }

    struct Commit *new_commit = malloc(sizeof(struct Commit));
    Initial_Commit(new_commit);

    strcpy(Staging_area.Commit_name,commit_name);
    Staging_area.Commit_one = Head;

    *new_commit = Staging_area;
    new_commit->new_point_id = ++Newest_point.top;
    Newest_point.point [Newest_point.top] = new_commit;
    Initial_Commit(&Staging_area);
    Head = new_commit;
    timex++;
}
struct Commit *Search_Commit (char *Name_searched) {
    for (int i = 0; i <= Newest_point.top ; i++) {
        struct Commit *Commit_searched = NULL;
        Commit_searched = Search_Point(Newest_point.point[i],Name_searched);
        if (Commit_searched != NULL) {
            for (int j = 0 ; j <= i ; j++) {
                Init_flag(Newest_point.point [j]);
            }
            return Commit_searched;
        }
    }
    for (int i = 0 ; i <= Newest_point.top ; i++) {
        Init_flag(Newest_point.point [i]);
    }
    return NULL;
}
struct Commit *Search_Point (struct Commit *Commit, char *Name_searched) {
    Commit->flag = 1;
    if (strcmp(Commit->Commit_name, Name_searched) == 0) {
        return Commit;
    }

    struct Commit *Address_searched_one = NULL;
    if (Commit->Commit_one != NULL && Commit->Commit_one->flag != 1) {
        Address_searched_one = Search_Point(Commit->Commit_one,Name_searched);
    }
    struct Commit *Address_searched_two = NULL;
    if (Commit->Commit_two != NULL && Commit->Commit_two->flag != 1) {
        Address_searched_two = Search_Point(Commit->Commit_two,Name_searched);
    }
    if (Address_searched_one != NULL) {
        return Address_searched_one;
    } else {
        return Address_searched_two;
    }

}
int Staging_is_empty () {
    if (Staging_area.Commit_one == NULL) {
        if (Staging_area.Commit_two == NULL) {
            if (Staging_area.top == -1) {
                return 1;
            }
        }
    }
    return 0;
}
void Checkout (char *commit_name) {
    if (Staging_is_empty() != 1) {
        return;
    }
    struct Commit *tmp = Search_Commit(commit_name);
    if (tmp != NULL) {
        Head = tmp;
    }
}
void Merge (char *mergee,char *commit_name) {
    if (Staging_is_empty() == 0) {
        return;
    }
    if (strcmp(mergee, commit_name) == 0) {
        return;
    }
    struct Commit *commit_mergee = Search_Commit(mergee);
    if (commit_mergee == NULL) {
        return;
    }
    struct Commit *new_commit = malloc(sizeof(*new_commit));
    Initial_Commit(new_commit);
    new_commit->Commit_one = Head;
    new_commit->Commit_two = commit_mergee;
    strcpy(new_commit->Commit_name,commit_name);
    if (Head->new_point_id != -1 && commit_mergee->new_point_id != -1) {
        Newest_point.point [Head->new_point_id] = new_commit;
        new_commit->new_point_id = Head->new_point_id;
        Head->new_point_id = -1;

        if (commit_mergee->new_point_id != Newest_point.top) {
            Newest_point.point [commit_mergee->new_point_id] = Newest_point.point [Newest_point.top];
            Newest_point.point [Newest_point.top] = NULL;
            Newest_point.top--;
            Newest_point.point [commit_mergee->new_point_id] ->new_point_id = commit_mergee->new_point_id;
            commit_mergee->new_point_id = -1;
        } else {
            Newest_point.point [Newest_point.top--] = NULL;
            commit_mergee->new_point_id = -1;
        }

    } else if (Head->new_point_id != -1) {
        Newest_point.point [Head->new_point_id] = new_commit;
        new_commit->new_point_id = Head->new_point_id;
        Head->new_point_id = -1;
    } else if (commit_mergee->new_point_id != -1) {
        Newest_point.point [commit_mergee->new_point_id] = new_commit;
        new_commit->new_point_id = commit_mergee->new_point_id;
        commit_mergee->new_point_id = -1;
    } else {
        Newest_point.point [++Newest_point.top] = new_commit;
        new_commit->new_point_id = Newest_point.top;
    }
    Head = new_commit;
    timex++;
}
int File_is_deleted (int dark_loca,struct File *Address,struct Content *content,int *last_loca,int *arr) {
    if (Address->Name [0] == '-'){
        return 1;
    }
    for (int i = *last_loca ; i <= dark_loca ; i++) {
        for (int j = 1 ; ; j++) {
            if (content->file_address [arr [i]]->Name [j] != Address->Name [j - 1]) {
                break;
            }
            if (j == strlen(Address->Name) && content->file_address [arr [i]]->Name [j + 1] == 0) {
                *last_loca = i;
                if (content->file_address [arr [i]]->Time > Address->Time) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }
    return 0;
}