#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "mtm_ex3.h"
#include "GradeSheet.h"
#include "CleanCourse.h"
#include "SemesterGrades.h"
#include "CourseGrades.h"
#include "Student.h"
#include "Student.h"
#include "CourseManager.h"

#define MTM_SUCCESS -1
/**
 * error: terminate program run and frees all space allocated
 * happens if command line arguments, opening files or allocation failure occures
 * @return
 */
void error(MtmErrorCode errorCode, CourseManager courseManager){
    mtmPrintErrorMessage(stderr,errorCode);
    courseManagerDestroy(courseManager);
}

/**
 * checkCommandLine: checks command line arguments and puts the relevant files in place
 * receives command line, number of argument, input file and output file
 * @return
 * 1 if line correct, 0 otherwise
 */
int checkCommandLine(int argc,char **argv,int *input_flag, int *output_flag){
    int counter = 1;//will go over command line arguments
    if(argc > 5){
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;//there're more variables then there should be
    }
    if(argc == 1){
        *input_flag = 0;
        *output_flag = 0;
        return MTM_SUCCESS;
    }
    if(strcmp(argv[counter],"-i") != 0 && strcmp(argv[counter],"-o") != 0){
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
    //will check if there wasn't any input/output file declared
    while(counter < argc){
        //iterating through command line
        if (strcmp(argv[counter],"-i") == 0){//input
            counter++;
            if(!*(argv+counter)){
                return MTM_INVALID_COMMAND_LINE_PARAMETERS;
            }
            if(strcmp(argv[counter],"-o") == 0){
                return MTM_INVALID_COMMAND_LINE_PARAMETERS;//needs to be a file
            }
            *input_flag = counter;//place in array
        }
        if (strcmp(argv[counter],"-o") == 0){//output
            counter++;
            if(!*(argv+counter)){
                return MTM_INVALID_COMMAND_LINE_PARAMETERS;
            }
            if(strcmp(argv[counter],"-i") == 0){
                return MTM_INVALID_COMMAND_LINE_PARAMETERS;//needs to be a file
            }
            *output_flag = counter;//output file
        }
        counter++;
    }
    return MTM_SUCCESS;
}

/**
 * freeNexit: frees all memory allocated and closes open files
 * receives courseManager and input/output files
 * @return
 */
void freeNexit(CourseManager courseManager){
    courseManagerDestroy(courseManager);
}

//get rid of spaces and tabs
char* delete_spaces(char *string){
    if(string == NULL){
        return NULL;
    }
    while(*string && (*string == ' ' || *string == '\t')){
        string++;
    }
    return string;
}


//get rid of spaces and tabs
char* advance_word(char *string){
    if(string == NULL){
        return NULL;
    }
    while(*string && *string != ' ' && *string != '\t'){//we're in a word
        string++;
    }
    while(*string && (*string == ' ' || *string == '\t')){
        string++;
    }
    return string;
}

//counts length until next space or tab
int count_till_space(char *string){
    if(string == NULL){
        return 0;
    }
    int counter = 0;
    while(*string && *string != ' ' && *string != '\t' && *string != '\n'){//we're in a word
        string++;
        counter++;
    }
    return counter;
}

/**
 * callStudentAdd: calls the function student add by the line received
 * @return
 */
void callStudentAdd(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    char *param2 = malloc(MAX_LEN + 1);
    char *param3 = malloc(MAX_LEN + 1);
    param2[0] = '\0';param3[0] = '\0';
    int counter = 0, id = 0;//will count words length
    token = advance_word(token);//advance to next word
    id = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    counter = count_till_space(token);
    strncat(param2,token,counter);//get second param from line
    param2[counter] = '\0';
    token = advance_word(token);//advance to next word
    counter = count_till_space(token);
    strncat(param3,token,counter);//get third param from line
    param3[counter] = '\0';
    MtmErrorCode error = addStudent(courseManager,id,param2,param3);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
    free(param2);
    free(param3);
}

/**
* callStudentRemove: calls the function student remove by the line received
* @return
*/
void callStudentRemove(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int id = 0;//will count words length
    token = advance_word(token);//advance to next word
    id = strtol(token,&token,10);
    MtmErrorCode error = removeStudent(courseManager,id);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
* callStudentLogIn: calls the function student login by the line received
* @return
*/
void callStudentLogIn(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int id = 0;//will count words length
    token = advance_word(token);//advance to next word
    id = strtol(token,&token,10);
    MtmErrorCode error = studentLogIn(courseManager,id);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
* callStudentLogOut: calls the function student logout by the line received
* @return
*/
void callStudentLogOut(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    MtmErrorCode error = studentLogOut(courseManager);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
* callStudentFriendRequest: calls the function student friend request by the line received
* @return
*/
void callStudentFriendRequest(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int id = 0;//will count words length
    token = advance_word(token);//advance to next word
    id = strtol(token,&token,10);
    MtmErrorCode error = studentFriendRequest(courseManager,id);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * callStudentHandleFriendRequest: calls the function handle request by the line received
 * @return
 */
void callStudentHandleFriendRequest(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    char *param2 = malloc(MAX_LEN + 1);
    param2[0] = '\0';
    int counter = 0, other_id = 0;//will count words length
    token = advance_word(token);//advance to next word
    other_id = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    counter = count_till_space(token);
    strncat(param2,token,counter);//get second param from line
    param2[counter] = '\0';
    MtmErrorCode error = studentHandleRequest(courseManager,other_id,param2);
    free(param2);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
* callStudentUnFriend: calls the function student unfriend by the line received
* @return
*/
void callStudentUnFriend(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int id = 0;//will count words length
    token = advance_word(token);//advance to next word
    id = strtol(token,&token,10);
    MtmErrorCode error = studentUnFriend(courseManager,id);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * executestudent: executes student commands
 * receives courseManager, line and output file
 * @return
 */
void executeStudent(CourseManager courseManager,char* token,FILE* output_file){
    switch(*token){
        case 'a'://student add
        {
            callStudentAdd(courseManager,token,output_file);
            return;
        }
        case 'r'://student remove
        {
            callStudentRemove(courseManager,token,output_file);
            return;
        }
        case 'l'://student login/logout
        {
            if(*(token+3) == 'i'){
                callStudentLogIn(courseManager,token,output_file);
            }
            else {
                callStudentLogOut(courseManager,token,output_file);
            }
            return;
        }
        case 'f'://student friend request
        {
            callStudentFriendRequest(courseManager,token,output_file);
            return;
        }
        case 'h'://student handle friend request
        {
            callStudentHandleFriendRequest(courseManager,token,output_file);
            return;
        }
        case 'u'://student unfriend
        {
            callStudentUnFriend(courseManager,token,output_file);
            return;
        }
    }
}

/**
 * callGradeSheetAdd: calls the function grade sheet add by the line received
 * @return
 */
void callGradeSheetAdd(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    char *param2 = malloc(MAX_LEN + 1);
    param2[0] = '\0';
    int counter = 0, semester = -1, course_id = -1, grade = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    semester = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    course_id = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    counter = count_till_space(token);
    strncat(param2,token,counter);//get points param from line
    param2[counter] = '\0';
    token = advance_word(token);//advance to next word
    grade = strtol(token,&token,10);
    MtmErrorCode error = grade_sheet_add(courseManager,semester,course_id,param2,grade);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
    free(param2);
}


/**
 * callGradeSheetRemove: calls the function grade sheet remove by the line received
 * @return
 */
void callGradeSheetRemove(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int semester = -1, course_id = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    semester = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    course_id = strtol(token,&token,10);
    MtmErrorCode error = grade_sheet_remove(courseManager,semester,course_id);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * callGradeSheetUpdate: calls the function grade sheet update by the line received
 * @return
 */
void callGradeSheetUpdate(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int new_grade = -1, course_id = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    course_id = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    new_grade = strtol(token,&token,10);
    MtmErrorCode error = grade_sheet_update(courseManager,course_id,new_grade);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}


/**
 * executeGradeSheet: executes grade sheet commands
 * receives courseManager, line and output file
 * @return
 */
void executeGradeSheet(CourseManager courseManager,char* token,FILE* output_file){
    switch(*token){
        case 'a'://grade sheet add
        {
            callGradeSheetAdd(courseManager,token,output_file);
            return;
        }
        case 'r'://grade sheet remove
        {
            callGradeSheetRemove(courseManager,token,output_file);
            return;
        }
        case 'u'://grade sheet update
        {
            callGradeSheetUpdate(courseManager,token,output_file);
            return;
        }
    }
}

/**
* callReportFull: calls the function report full by the line received
* @return
*/
void callReportFull(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    MtmErrorCode error = reportFull(courseManager,output_file);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
* callReportClean: calls the function report clean by the line received
* @return
*/
void callReportClean(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    MtmErrorCode error = reportClean(courseManager,output_file);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * callReportBest: calls the function report best by the line received
 * @return
 */
void callReportBest(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int amount = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    amount = strtol(token,&token,10);
    MtmErrorCode error = reportBest(courseManager,output_file,amount);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * callReportWorst: calls the function report worst by the line received
 * @return
 */
void callReportWorst(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int amount = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    amount = strtol(token,&token,10);
    MtmErrorCode error = reportWorst(courseManager,output_file,amount);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * callReportReference: calls the function report reference by the line received
 * @return
 */
void callReportReference(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    int course_id = -1, amount = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    course_id = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    amount = strtol(token,&token,10);
    MtmErrorCode error = reportReference(courseManager,output_file,course_id,amount);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
}

/**
 * callReportFacultyRequest: calls the function report faculty request by the line received
 * @return
 */
void callReportFacultyRequest(CourseManager courseManager,char* token,FILE* output_file) {
    //space for parameters to functions in line (max 4)
    char *param2 = malloc(MAX_LEN + 1);
    param2[0] = '\0';
    int counter = 0,course_id = -1;//gets parameters for function
    token = advance_word(token);//advance to next word
    course_id = strtol(token,&token,10);
    token = advance_word(token);//advance to next word
    counter = count_till_space(token);
    strncat(param2,token,counter);//get points param from line
    param2[counter] = '\0';
    MtmErrorCode error = reportFacultyRequest(courseManager,output_file,course_id,param2);
    if(error != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,error);
    }
    free(param2);
}

/**
 * executeReport: executes report commands
 * receives courseManager, line and output file
 * @return
 */
void executeReport(CourseManager courseManager,char* token,FILE* output_file){
    switch(*token){
        case 'f'://report full/faculty request
        {
            if(*(token+1) == 'u'){
                callReportFull(courseManager,token,output_file);
            }
            else{
                callReportFacultyRequest(courseManager,token,output_file);
            }
            return;
        }
        case 'c'://report clean
        {
            callReportClean(courseManager,token,output_file);
            return;
        }
        case 'b'://report best
        {
            callReportBest(courseManager,token,output_file);
            return;
        }
        case 'w'://report worst
        {
            callReportWorst(courseManager,token,output_file);
            return;
        }
        case 'r'://report reference
        {
            callReportReference(courseManager,token,output_file);
            return;
        }
    }
}

/**
 * executeLine: receives a line and executes relevant functions
 * receives courseManager, line and output file
 * @return
 */
void executeLine(CourseManager courseManager,char* line,FILE* output_file){
    //const char space[2] = " ";//delimiter for line
    char *token;//will iterate through words
    token = delete_spaces(line);
    //token now points to the first char (that's not space or tab) in line
    if(*token == '#'){
        return;//this line is a remark
    }
    if(*token == 's'){//student functions
        token = advance_word(token);//advance to next word
        executeStudent(courseManager,token,output_file);
    }
    if(*token == 'g'){//grade_sheet functions
        token = advance_word(token);//advance to next word
        executeGradeSheet(courseManager,token,output_file);
    }
    if(*token == 'r'){//report functions
        token = advance_word(token);//advance to next word
        executeReport(courseManager,token,output_file);
    }
}

/**
 * readAndExecuteFile: reads lines from file and executes relevant functions
 * receives courseManager and input/output file
 * @return
 */
MtmErrorCode readAndExecuteFile(CourseManager courseManager,FILE* input_file,FILE* output_file){
    char *line = malloc(MAX_LEN+1);//will get line of command
    if(line == NULL){
        mtmPrintErrorMessage(stderr,MTM_OUT_OF_MEMORY);
        freeNexit(courseManager);//memory problem - exit
        return MTM_OUT_OF_MEMORY;
    }
    while(fgets(line,MAX_LEN+1,input_file) != NULL){
        executeLine(courseManager,line,output_file);
        strcpy(line,"");//clear line for next line to receive
    }
    free(line);
    return MTM_SUCCESS;
}

int main(int argc,char **argv){
    FILE* input_file;
    FILE* output_file;//will receive the relevant output and input files
    MtmErrorCode errorCode;//will receive all error codes from program run
    CourseManager courseManager;//will create course manager
    int input_flag = 0, output_flag = 0;
    courseManagerCreate(&courseManager);
    if(courseManager == NULL){
        mtmPrintErrorMessage(stderr,MTM_OUT_OF_MEMORY);
        freeNexit(courseManager);
        return 0;
    }
    errorCode = checkCommandLine(argc,argv,&input_flag,&output_flag);
    if(errorCode != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,errorCode);
        freeNexit(courseManager);
        return 0;//exit program
    }
    if(input_flag != 0) {
        input_file = fopen(argv[input_flag], "r");
        if (input_file == NULL) {
            return MTM_CANNOT_OPEN_FILE;
        }
    }
    if(output_flag != 0) {
        output_file = fopen(argv[output_flag], "w");
        if (output_file == NULL) {
            return MTM_CANNOT_OPEN_FILE;
        }
    }
    if(input_flag != 0){
        if(output_flag != 0){//both are actuall files
            if(readAndExecuteFile(courseManager,input_file,output_file)!=MTM_SUCCESS){
                freeNexit(courseManager);
                fclose(input_file);
                fclose(output_file);
                return 0;
            }
            fclose(input_file);
            fclose(output_file);
        }
        else{//input is a file and output stdout
            if(readAndExecuteFile(courseManager,input_file,stdout)!=MTM_SUCCESS){
                freeNexit(courseManager);
                fclose(input_file);
                return 0;
            }
            fclose(input_file);
        }
    }
    else{
        if(output_flag != 0){//input file is stdin and output file is a file
            if(readAndExecuteFile(courseManager,stdin,output_file)!=MTM_SUCCESS){
                freeNexit(courseManager);
                fclose(output_file);
                return 0;
            }
            fclose(output_file);
        }
            if(readAndExecuteFile(courseManager,stdin,stdout)!=MTM_SUCCESS){
                freeNexit(courseManager);
                return 0;
            }
    }
    freeNexit(courseManager);
    return 0;
}
