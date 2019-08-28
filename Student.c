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

/**
 * Student structure
 *
 * Student structure represents students in the system.
 * Each student structure has it's first name, sur name and id.
 * Also, each student has a transcript (GradeSheet) and a list of friends.

 * The following functions are available:
 * studentDestroy:                 De-allocates a student.
 * studentPrintFriendsTookCourse:  prints the friends who took the same course before
 * studentPrintWorst:              prints the worst grades (that counts) for the student
 * studentPrintBest:               prints the best grades (that counts) for the student
 * studentPrintClean:              prints the student clean transcript
 * studentPrintGrades:             prints the student full transcript
 * studentUpdateGrade:             updates a grade from the students transcript (from the specific semester
 * studentRemoveGrade:             removes a grade from the students transcript (from the specific semester
 * studentAddGrade:                insert a grade to the students transcript (to the specific semester
 * studentRemoveFriend:            Removes a friend from the students friend list
 * studentAddFriend:               Adds a friend to the students friend list
 * studentCreate:                  Allocates a new empty student.
 */

/** Type for defining the student */
struct Student_t {
    int student_id;
    char* first_name;
    char* last_name;
    GradeSheet gradeSheet;
    List friendsList;
    int helper;//for reference print in course manager
};

static ListElement friendsListCopy(ListElement element) {
    if(element == NULL){
        return NULL;
    }
    int *new = malloc(sizeof(int));
    *new = *(int *)element;
    return new? new : NULL;
}

static void freeFriendsList(ListElement element) {
    free(element);
}
/**
 * studentCreate: Allocates a new empty student.
 *
 * @param id
 * @param first_name
 * @param last_name
 * @param student - pointer to student created
 * @return
 * STUDENT_SUCCESS - new student created
 * STUDENT_OUT_OF_MEMORY - memory problem occurred
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 */
StudentResult studentCreate(int id,char* first_name,char* last_name,Student *student){
    *student = malloc(sizeof(struct Student_t));
    if((*student)==NULL){
        return STUDENT_OUT_OF_MEMORY;
    }
    (*student)->first_name = malloc(sizeof(char)*((strlen(first_name))+1));
    if((*student)->first_name == NULL){
        return STUDENT_OUT_OF_MEMORY;
    }
    (*student)->last_name = malloc(sizeof(char)*((strlen(last_name))+1));
    if((*student)->last_name == NULL){
        return STUDENT_OUT_OF_MEMORY;
    }
    (*student)->friendsList = listCreate(friendsListCopy,freeFriendsList);
    if((*student)->friendsList == NULL){
        return STUDENT_OUT_OF_MEMORY;
    }
    if(gradeSheetCreate(&(*student)->gradeSheet)!=GRADE_SHEET_SUCCESS){
        return STUDENT_OUT_OF_MEMORY;
    }
    strcpy((*student)->last_name,last_name);
    strcpy((*student)->first_name,first_name);
    (*student)->student_id = id;
    (*student)->helper = -1;
    return STUDENT_SUCCESS;
}

/**
 * studentPrintcheck: prints all the ID of the student's friends
 * @param Student
 * @return
*/
void studentPrintCheck(Student student){
    LIST_FOREACH(int*,iterator,student->friendsList){
        printf("%d\n",*iterator);
    }
}

/**
 * studentAddFriend: Adds a friend to the students friend list
 *
 * @param id - id of student added
 * @return
 * STUDENT_SUCCESS - new friend added
 * STUDENT_OUT_OF_MEMORY - memory problem occurred
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 * STUDENT_ALREADY_FRIEND - the student is in the list already
 */
StudentResult studentAddFriend(Student student, int id){
    if(student==NULL){
        return STUDENT_INVALID_PARAMETERS;
    }
    if(id<0 || id >1000000000){
        return STUDENT_INVALID_PARAMETERS;
    }
    LIST_FOREACH(int*,iterator,student->friendsList) {
        if(*iterator==id){
            return  STUDENT_ALREADY_FRIENDS;
        }
    }
    int *temp = malloc(sizeof(int));
    if(temp == NULL){
        return STUDENT_INVALID_PARAMETERS;
    }
    *temp = id;
    listInsertLast(student->friendsList,temp);
    free(temp);
    return STUDENT_SUCCESS;
}

/**
 * studentRemoveFriend: Removes a friend from the students friend list
 *
 * @param id - id of student added
 * @return
 * STUDENT_SUCCESS - student removed
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 * STUDENT_NOT_FRIEND - the student isn't a friend in the list
 */
StudentResult studentRemoveFriend(Student student, int id) {
    if (student == NULL) {
        return STUDENT_INVALID_PARAMETERS;
    }
    LIST_FOREACH(int*, iterator, student->friendsList) {
        if (*iterator == id) {
            listRemoveCurrent(student->friendsList);
            return STUDENT_SUCCESS;
        }
    }
    return STUDENT_NOT_FRIEND;
}
/**
 * studentAddGrade: insert a grade to the students transcript (to the specific semester
 * and update the latest semester in course list)
 * @param semester
 * @param course_id
 * @param points
 * @param grade
 * @return
 * STUDENT_SUCCESS - grade added
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 */
StudentResult studentAddGrade(Student student, int semester, int course_id, char* points, int grade){
    if(student==NULL || semester <= 0 || course_id >= 1000000 || course_id <=0
            || grade < 0 || grade > 100){
        return STUDENT_INVALID_PARAMETERS;
    }
    if(gradeSheetAddGrade(student->gradeSheet,semester,course_id,points,grade)!=GRADE_SHEET_SUCCESS){
        return STUDENT_INVALID_PARAMETERS;
    }
    return STUDENT_SUCCESS;
}

/**
 * studentRemoveGrade: removes a grade from the students transcript (from the specific semester
 * and update the latest semester in course list)
 * @param semester
 * @param course_id
 * @return
 * STUDENT_INVALID_PARAMETERS- student is NULL
 * STUDENT_SUCCESS - grade removed
 * STUDENT_COURSE_NOT_EXIST - the student didn't take the course
 */
StudentResult studentRemoveGrade(Student student, int semester, int course_id){
    if(student==NULL){
        return STUDENT_INVALID_PARAMETERS;
    }
    if (gradeSheetRemoveGrade(student->gradeSheet,semester,course_id)!=GRADE_SHEET_SUCCESS){
        return STUDENT_COURSE_NOT_EXIST;
    }
    return STUDENT_SUCCESS;
}

/**
 * studentUpdateGrade: updates a grade from the students transcript (from the specific semester
 * and update the latest semester in course list)
 * @param grade
 * @param course_id
 * @return
 * STUDENT_SUCCESS - grade updated
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 * STUDENT_COURSE_NOT_EXIST - the student didn't take the course
 */
StudentResult studentUpdateGrade(Student student, int course_id, int grade){
    if(student==NULL){
        return STUDENT_INVALID_PARAMETERS;
    }
    if (gradeSheetUpdateGrade(student->gradeSheet,course_id,grade) == GRADE_SHEET_COURSE_NOT_EXIST){
        return STUDENT_COURSE_NOT_EXIST;
    }
    if(grade < 0 || grade > 100){
        return MTM_INVALID_PARAMETERS;
    }
    return STUDENT_SUCCESS;
}

/**
 * studentPrintGrades: prints the student full transcript
 * @return
 */
void studentPrintGradesFull(Student student,FILE* file){
printf("%s",student->last_name);
    mtmPrintStudentInfo(file,student->student_id,student->first_name,student->last_name);
 gradeSheetPrintFull(student->gradeSheet,file);
}

/**
 * studentPrintClean: prints the student clean transcript
 * @return
 */
void studentPrintClean(Student student,FILE* file){
    mtmPrintStudentInfo(file,student->student_id,student->first_name,student->last_name);
    gradeSheetPrintClean(student->gradeSheet,file);
}

/**
 * studentPrintBest: prints the best grades (that counts) for the student
 * @param amount
 * @return
 * STUDENT_SUCCESS - grade printed
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 */
StudentResult studentPrintBest(Student student,FILE* file, int amount){
    if(gradeSheetPrintBest(student->gradeSheet,file,amount)!=GRADE_SHEET_SUCCESS){
        return STUDENT_INVALID_PARAMETERS;
    }
    return STUDENT_SUCCESS;
}

/**
 * studentPrintWorst: prints the worst grades (that counts) for the student
 * @param amount
 * @return
 * STUDENT_SUCCESS - grades printed
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 */
StudentResult studentPrintWorst(Student student,FILE* file, int amount){
    if(gradeSheetPrintWorst(student->gradeSheet,file,amount)!=GRADE_SHEET_SUCCESS){
        return STUDENT_INVALID_PARAMETERS;
    }
    return STUDENT_SUCCESS;
}
/**
 * studentGetStudentId: returns student's id
 * @param student
 * @return
 * -1 if student is NULL
 * student's id- else
 */
int studentGetStudentId (Student student){
    if (student==NULL){
        return -1;
    }
    return student->student_id;
}
/**
 * studentCheckIfFriends: check if a student appears
 * in the connected student friends list
 * f
 * @return
 * -1 if student is NULL
 * 1 if they are friends
 * 0 else
 */
int studentCheckIfFriends(Student student, int friend_id){
    if (student==NULL){
        return -1;
    }
    LIST_FOREACH(int*,iterator,student->friendsList){
        if (*iterator==friend_id){
            return 1;
        }
    }
    return 0;
}

 /**
 * studentCopy: copies a student.
 *
 * @return
 * NULL - if student to be copied is NULL
 * student otherwise
 */
Student studentCopy (Student student){
    if (student==NULL){
        return NULL;
    }
    Student new_student;
    if(studentCreate(student->student_id,student->first_name,
                     student->last_name,&new_student)!=STUDENT_SUCCESS){
        return NULL;
    }
    listDestroy(new_student->friendsList);
    gradeSheetDestroy(new_student->gradeSheet);
    new_student->friendsList = listCopy(student->friendsList);
    new_student->gradeSheet = copyGradeSheet(student->gradeSheet);
     new_student->helper =student->helper;
    return new_student;
}

/**
 * studentGetHelper- takes the helper from the student
 *
 * @return
 */
int studentGetHelper(Student student){
    return student->helper;
}

/**
 * studenInsertHelper- insersts  helper from the student
 *
 * @return
 */
void studentInsertHelper(Student student, int helper1){
    student->helper= helper1;
}

/**
 * studentGetBestGrade- takes the best grade of student in a course
 * @param- student, course_id
 * @return
 */
int studentGetBestGrade(Student student, int course_id){
    return gradeSheetGetBestGrade(student->gradeSheet,course_id);
}

/**
 * studentReturnFriends- returns the friends list of student
 * @param- student
 * @return list
 */
List studentReturnFriends(Student student){
    return student->friendsList;
}

/**
 * studentPrintStudentName: prints the student name only
 * @return
 */
void studentPrintStudentName(Student student,FILE* file){
    mtmPrintStudentName(file,student->first_name,student->last_name);
}

/**
 * studentDoesGradeExists - checks if a grade is in the student's sheet
 * @param- gradesheet, course_id
  * @return
 * 1 if the grade is there, 0 otherwise
 */
int studentDoesGradeExists(Student student, int course_id){
    assert(student!=NULL);
    if(gradeSheetDoesGradeExists(student->gradeSheet,course_id) == 0){
        return 0;
    }
    return 1;
}

/**
 * studentDestroy: De-allocates a student.
 *
 * @return
 */
void studentDestroy(Student student){
    free(student->last_name);
    free(student->first_name);
    gradeSheetDestroy(student->gradeSheet);
    listDestroy(student->friendsList);
    free(student);
}

