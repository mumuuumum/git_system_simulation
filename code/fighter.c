#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//提交的结构体
struct Commit{
    struct Commit *Commit_one;
    struct Commit *Commit_two;
    char Commit_name [132];
    struct File *(Content [132]);
    int new_point_id;
    int top;
};
//文件的结构体
struct File{
    int Time;
    char Name [132];
    char Content [132];
};

struct Newest_point {
    struct Commit *(point [132]);
    int top;
};

struct Newest_point Newest_point;
struct Commit *Head = NULL;
struct Commit Staging_area;
char command_str [132];
int time;
int in_stage;

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
void Commit (char *commit_name);
struct Commit *Search_Commit (char *Name_searched);
struct Commit *Search_Point (struct Commit *Commit, char *Name_searched);
int Staging_is_empty ();
void Checkout (char *commit_name);
void Merge (char *mergee,char *commit_name);

int main () {
    Initial_Commit(&Staging_area);
    Initial_Newest_point ();

    int command_number = 0;
    scanf("%d", &command_number);
    
    getchar();
    for (int i = 0; i < command_number ; i++) {
        memset(command_str,0,132 * sizeof(char));
        gets(command_str);
        char filename [132];
        char cmtname [132];
        int offset;
        int len;
        int Start_point;
        int End_point;
        memset(filename,0,132 * sizeof(char));
        memset(cmtname,0,132 * sizeof(char));
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
                Ls();
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

    return 0;
}
void Initial_Newest_point () {
    for (int i = 0; i < 132 ; i++) {
        Newest_point.point [i] = NULL;
    }
    Newest_point.top = -1;
}
void Initial_Commit (struct Commit *Commit) {
    Commit->Commit_one = NULL;
    Commit->Commit_two = NULL;
    memset(Commit->Commit_name,0,132 * sizeof(char));
    for (int i = 0; i < 132 ; i++) {
        Commit->Content [i] = NULL;
    }
    Commit->top = -1;
    Commit->new_point_id = -1;
}
void Initial_File (struct File *File) {
    File->Time = 0;
    memset(File->Name,0,132);
    memset(File->Content,0,132);
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
                if (j == strlen(Name_searched)) {
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
                if (j == strlen(Name_searched)) {
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
        if (Address_searched_one->Name [0] == '-') {
            return Address_searched_two;
        } else if (Address_searched_two->Name [0] == '-') {
            return Address_searched_one;
        } else {
            if (Address_searched_one->Time > Address_searched_two->Time) {
                return Address_searched_one;
            } else {
                return Address_searched_two;
            }
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
                        memset(Address->Name,0,132 * sizeof(char));
                        Part_strcpy(Address->Name,filename,0,131);
                        break;
                    }
                }
            }
        } else {
            Staging_area.Content [++Staging_area.top] = malloc(sizeof(struct File));
            Initial_File(Staging_area.Content [Staging_area.top]);
            Part_strcpy(Staging_area.Content [Staging_area.top]->Content,Address->Content,0,131);
            Part_strcpy(Staging_area.Content [Staging_area.top]->Name,filename,0,131);
            Address = Staging_area.Content [Staging_area.top];
        }
    } else {
        Address = malloc(sizeof(struct File));
        Staging_area.Content [++Staging_area.top] = Address;
        Initial_File(Address);
        strcpy((Address)->Name,filename);
    }
    if (offset > strlen(Address->Content)) {
        for (int i = strlen(Address->Content); i < offset;i++) {
            Address->Content [i] = '.';
        }
    }
    char *tmp_str = malloc((len + 1) * sizeof(char));
    memset(tmp_str,0,(len + 1) * sizeof(char));
    gets(tmp_str);
    for (int i = 0 , j = offset ; i < strlen(tmp_str) ; i++ , j++) {
        Address->Content [j] = tmp_str [i];
    }
    free(tmp_str);
    Address->Time = ++time;
}
void Read (char *filename,int offset,int len) {
    struct File *Address = Search_File(filename);
    if (Address != NULL && Address->Name [0] != '-') {
        int i;
        int j = 0;
        for (i = offset ; Address->Content [i] != '\0' && j < len ; i++,j++) {
            printf("%c", Address->Content [i]);
        }
        for (;j < len;j++) {
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
            memset(Address->Name,0,132 * sizeof(char));
            Address->Name [0] = '-';
            strcat(Address->Name,filename);
            memset(Address->Content,0,132 * sizeof(char));
        } else {
            Staging_area.Content [++Staging_area.top] = malloc(sizeof(struct File));
            Initial_File(Staging_area.Content [Staging_area.top]);
            Staging_area.Content [Staging_area.top]->Name [0] = '-';
            strcat(Staging_area.Content [Staging_area.top]->Name,filename);
        }
    }
}
void Ls () {
    struct File *(Content [132 * 2]);
    for (int i = 0 ; i < 132 * 2 ; i++) {
        Content [i] = NULL;
    }
    int Content_num = 0;
    for (int i = 0 ; i <= Staging_area.top ; i++) {
        if (Staging_area.Content [i]->Name [0] != '-') {
            Content [Content_num] = Staging_area.Content [i];
            Content_num++;
        } else {
            Content_num--;
        }
    }

    if (Head != NULL) {
        if (Head->Commit_one != NULL && Head->Commit_two != NULL) {
            for (int i = 0 ; i <= Head->Commit_one->top ; i++) {
                if (Head->Commit_one->Content [i]->Name [0] != '-') {
                    Content [Content_num] = Head->Commit_one->Content [i];
                    Content_num++;
                }
            }
            for (int i = 0 ; i <= Head->Commit_two->top ; i++) {
                if (Head->Commit_two->Content [i]->Name [0] != '-') {
                    Content [Content_num] = Head->Commit_two->Content [i];
                    Content_num++;
                }
            }
        } else {
            for (int i = 0 ; i <= Head->top ; i++) {
                if (Head->Content [i]->Name [0] != '-') {
                    Content [Content_num] = Head->Content [i];
                    Content_num++;
                }
            }
        }
    }

    int *arr = malloc(Content_num * sizeof(int));
    for (int i = 0 ; i < Content_num ; i++) {
        arr [i] = i;
    }

    for (int i = 0; i < Content_num - 1; i++) {
        int max = i;

        for (int j = i + 1; j < Content_num ; j++) {
            if (strcmp(Content [arr [max]]->Name,Content [j]->Name) < 0) {
                max = j;
            }
        }
        int tmp = arr [max];
        arr [max] = arr [i];
        arr [i] = tmp;
    }

    int file_number = 0;
    if (Content_num == 1) {
        file_number = Content_num;
    } else {
        for (int i = 0; i < Content_num - 1; i++) {
            if (i == Content_num - 2) {
                if (strcmp(Content [arr [i]]->Name,Content [arr [i + 1]]->Name) != 0) {
                    file_number += 2;
                } else {
                    file_number++;
                }
                break;
            }
            if (strcmp(Content [arr [i]]->Name,Content [arr [i + 1]]->Name) != 0) {
                file_number++;
            }
        }
    }
    if (file_number == 0) {
        printf("0\n");
    } else {
        printf("%d %s %s\n", file_number, Content [arr [Content_num - 1]]->Name, Content [arr [0]]->Name);
    }
    free(arr);
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
}
struct Commit *Search_Commit (char *Name_searched) {
    for (int i = 0; i <= Newest_point.top ; i++) {
        struct Commit *Commit_searched = NULL;
        Commit_searched = Search_Point(Newest_point.point[i],Name_searched);
        if (Commit_searched != NULL) {
            return Commit_searched;
        }
    }
    return NULL;
}

struct Commit *Search_Point (struct Commit *Commit, char *Name_searched) {
    if (strcmp(Commit->Commit_name, Name_searched) == 0) {
        return Commit;
    }

    struct Commit *Address_searched_one = NULL;
    if (Commit->Commit_one != NULL) {
        Address_searched_one = Search_Point(Commit->Commit_one,Name_searched);
    }
    if (Address_searched_one != NULL) {
        return Address_searched_one;
    }
    struct Commit *Address_searched_two = NULL;
    if (Commit->Commit_two != NULL) {
        Address_searched_two = Search_Point(Commit->Commit_two,Name_searched);
    }
    if (Address_searched_two != NULL) {
        return Address_searched_two;
    }

    return NULL;
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
}
