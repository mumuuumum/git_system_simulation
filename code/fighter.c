#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//提交的结构体
struct Commit{
    struct Commit *Commit_one;
    struct Commit *Commit_two;
    char Commit_name [132];
    struct File *(Content [132]);
    int top;
};
//文件的结构体
struct File{
    int Time;
    char Name [132];
    char Content [132];
};


struct Commit *Head = NULL;
struct Commit Staging_area;
char command_str [132];
int time;
int in_stage;

void Initial_Commit (struct Commit *Commit);
void Initial_File (struct File *File);
struct File *Search (char *Name_searched);
struct File *Search_Staging (char *Name_searched);
struct File *Search_Common (char *Name_searched,struct Commit *Commit);
void Part_strcpy (char *str_one,const char *str_two,int Start_point,int End_point);
int str_to_num (const char *str,int Start_point,int End_point);
void Write (char *filename,int offset,int len);
void Read (char *filename,int offset,int len);
void Unlink (char *filename);

int main () {
    Initial_Commit(&Staging_area);

    int command_number = 0;
    scanf("%d", &command_number);
    
    getchar();
    for (int i = 0; i < command_number ; i++) {
        memset(command_str,0,132 * sizeof(char));
        gets(command_str);
        char filename [132];
        int offset;
        int len;
        int Start_point;
        int End_point;
        memset(filename,0,132 * sizeof(char));
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
                break;
            case 'c':
                switch (command_str[1]) {
                    case 'o':
                        break;
                    case 'h':
                        ;
                }
                break;
            case 'm':
                ;
        }
    }

    return 0;
}
void Initial_Commit (struct Commit *Commit) {
    Commit->Commit_one = NULL;
    Commit->Commit_two = NULL;
    memset(Commit->Commit_name,0,132 * sizeof(char));
    for (int i = 0; i < 132 ; i++) {
        Commit->Content [i] = NULL;
    }
    Commit->top = -1;
}
void Initial_File (struct File *File) {
    File->Time = 0;
    memset(File->Name,0,132);
    memset(File->Content,0,132);
}
struct File *Search (char *Name_searched) {
    struct File *Address = NULL;
    Address = Search_Staging (Name_searched);
    if (Address != NULL) {
        in_stage = 1;
        return Address;
    }
    if (Staging_area.Commit_one == NULL) {
        return NULL;
    }
    Address = Search_Common(Name_searched,Staging_area.Commit_one);
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
    for (int i = 0; i < Commit->top ; i++) {
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
    struct File *Address = Search(filename);
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
    strcat(Address->Content,tmp_str);
    free(tmp_str);
    Address->Time = ++time;
}
void Read (char *filename,int offset,int len) {
    struct File *Address = Search(filename);
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
    struct File *Address = Search(filename);
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
            strcat(Address->Name,filename);
        }
    }
}
