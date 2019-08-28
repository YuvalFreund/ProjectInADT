#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "mtm_ex3.h"
#include "SemesterGrades.h"
#include "CourseGrades.h"

/**
 * SemesterGrades structure
 *
 * semester grades will hold courses and their data for a relevant semester
 * The following functions are available:
 * semesterGradesCreate: Allocates a new empty SemesterGrades.
 * semesterGradesAddGrade: insert a grade to the semester
 * semesterGradesRemoveGrade: removes a grade from the semester
 * semesterGradesUpdateGrade: updates a grade in semester
 * semesterGradesPrintSemester: prints the full semester
 * semesterGradesPrintCourseInSemester: prints a specific course in a semester
 * semesterGradesPrintCourseInSemester: prints a specific course in a semester
 * semesterGradesDestroy: De-allocates a GradeSheet.
 */

/**
 * definition of SemesterGrades struct
 */
struct SemesterGrades_t{
    int semester;
    List courseGrades;
};


static ListElement copyCourseGrade(ListElement element) {
    if (element == NULL) {
        return NULL;
    }
    CourseGrade new = courseGradeCopy(element);
    return new ? new : NULL;
}
static void freeCourseGrade(ListElement element) {
    courseGradeDestroy(element);
}


/**
 * compareCourseGrade
 * a cmopare function for worst and best grades function - creating a fictive semester with
 * all the clean courses and sorting it as required
 */

static int compareCourseGrade(ListElement element1, ListElement element2, ListSortKey key){
    assert(element1 != NULL || element2 != NULL);
    //check points distance for courses
    int distance = courseGradeReturnCourseGrade(element1) -
                   courseGradeReturnCourseGrade(element2);
    distance = (distance*(*(int *)key));
    //if points are equal, check semester
    if(distance == 0) {
        distance = courseGradeGetHelper(element1) - courseGradeGetHelper(element2);
    }
    //if semesters are equal, check id
    if(distance == 0){
        distance = courseGradeReturnCourseid(element1) - courseGradeReturnCourseid(element2);
    }
    return distance;
}

/**
 * semesterGradesCreate: Allocates a new empty SemesterGrades.
 *
 * @return
 * SEMESTER_GRADES_SUCCESS - new grade sheet created
 * SEMESTER_GRADES_OUT_OF_MEMORY - memory problem occurred
 */
SemesterGradesResult semesterGradesCreate(int semester, SemesterGrades *semesterGrades){
    if(semester < 0){
        return SEMESTER_GRADES_INVALID_PARAMETERS;
    }
    *semesterGrades = malloc(sizeof(struct SemesterGrades_t));
    if((*semesterGrades) == NULL){
        return SEMESTER_GRADES_OUT_OF_MEMORY;
    }
    (*semesterGrades)->courseGrades = listCreate(copyCourseGrade,freeCourseGrade);
    if((*semesterGrades)->courseGrades == NULL){
        return SEMESTER_GRADES_OUT_OF_MEMORY;
    }
    (*semesterGrades)->semester = semester;
    return SEMESTER_GRADES_SUCCESS;
}

/**
 * semesterGradesCopy: copies a semester.
 * @return
 * SemesterGrades - if succeed
 * NULL - otherwise
 */
SemesterGrades semesterGradesCopy(SemesterGrades semesterGrade){
    if(semesterGrade == NULL){
        return NULL;
    }
    SemesterGrades new_semester;
    semesterGradesCreate(semesterGrade->semester,&new_semester);
    listDestroy(new_semester->courseGrades);
    new_semester->courseGrades = listCopy(semesterGrade->courseGrades);
    return new_semester;
}

//this function returns pints but as a string and not int
char* CHpointsLastCourseInSemester(SemesterGrades semester,int course_id){
    CourseGrade latest = NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if (courseGradeReturnCourseid(iterator)==course_id){
            latest = iterator;
        }
    }
    if (latest==NULL) {//course didn't appear in semester at all
        return NULL;
    }
    return courseGradeReturnCoursePoints(latest);
}

/*
 * semesterGradesInsertHelper: prints a specific course in a semester
 * @param semeter, helper
 * @return
 * SEMESTER_GRADES_SUCCESS - helper inserted
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 */

SemesterGradesResult semesterGradesInsertHelper(SemesterGrades semester, int helper){
    if(semester==NULL){
        return  SEMESTER_GRADES_INVALID_PARAMETERS;
    }
    CourseGrade latest= NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades) {
        latest = iterator;
    }
    courseGradeInsertHelper(latest,helper);
    return SEMESTER_GRADES_SUCCESS;
}
//check function for insert helper
void semesterGradesPrintHelper(SemesterGrades semester){
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        printf("%d,",courseGradeGetHelper(iterator));
    }
}


/**
 * semesterGradesGetSemester: returns the semester of the semester.
 *
 * @return
 * -1 - if NULL
 * semester - otherwise
 */
int semesterGradesGetSemester(SemesterGrades semesterGrade){
    if(semesterGrade == NULL){
        return -1;
    }
    return semesterGrade->semester;
}

/**
 * semesterGradesAddGrade: insert a grade to the semester
 * @param semester
 * @param course_id
 * @param points
 * @param grade
 * @return
 * SEMESTER_GRADES_SUCCESS - grade added
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 */
SemesterGradesResult semesterGradesAddGrade(SemesterGrades semester, int course_id, char* points, int grade){
    CourseGrade new;
    if(courseGradeCreate(course_id,points,grade,&new) != COURSE_GRADE_SUCCESS || semester == NULL){
        return SEMESTER_GRADES_INVALID_PARAMETERS;
    }
    if(new == NULL){
        return SEMESTER_GRADES_OUT_OF_MEMORY;
    }
    listInsertLast(semester->courseGrades,new);
    courseGradeDestroy(new);
    return SEMESTER_GRADES_SUCCESS;
}

/**
 * semesterGradesIsEmpty: check if semester is empty.
 *
 * @return 0 of emtpy, else 1
 */
int semesterGradesIsEmpty(SemesterGrades semesterGrade){
    if(semesterGrade==NULL){
        return 0;
    }
    if (listGetSize(semesterGrade->courseGrades)==0){
        return 0;
    }
    return 1;
}

/**
 * semesterGradesRemoveGrade: removes a grade from the semester
 * @param course_id
 * @param semester
 * @return
 * SEMESTER_GRADES_SUCCESS - grade removed
 * SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
 */
SemesterGradesResult semesterGradesRemoveGrade(SemesterGrades semester, int course_id){
    bool exists = false;
    CourseGrade latest = listGetFirst(semester->courseGrades);
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if(courseGradeReturnCourseid(iterator) == course_id){
            latest = iterator;
            exists = true;
        }
    }
    if(exists == false){
        return SEMESTER_GRADES_COURSE_NOT_EXIST;
    }
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if(iterator == latest){
            listRemoveCurrent(semester->courseGrades);
            break;
        }
    }
    return SEMESTER_GRADES_SUCCESS;
}

/**
 * semesterGradesUpdateGrade: updates a grade in semester
 * @param grade
 * @param course_id
 * @return
 * SEMESTER_GRADES_SUCCESS - grade updated
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 * SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
 */

SemesterGradesResult semesterGradesUpdateGrade(SemesterGrades semester,int course_id, int grade) {
    CourseGrade latest = NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades) {
        if (courseGradeReturnCourseid(iterator) == course_id) {
            latest = iterator;
        }
    }
    if(latest == NULL){
        return SEMESTER_GRADES_COURSE_NOT_EXIST;
    }
    if (grade < 0 || grade > 100) {
        return SEMESTER_GRADES_INVALID_PARAMETERS;
    }
    courseGradeUpdateGrade(latest, grade);
    return SEMESTER_GRADES_SUCCESS;
}

/**
 * DoesGradeExists - checks if a grade exists in the semester
 * @param semester
 * @param course_id
 * @return
 * -1 - the semester doesn't hold the course
 * grade otherwise
 */

int DoesGradeExists(SemesterGrades semester,int course_id){
    CourseGrade latest = NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if (courseGradeReturnCourseid(iterator)==course_id){
            latest = iterator;
        }
    }
    if (latest==NULL) {//course didn't appear in semester at all
        return -1;
    }
    return courseGradeReturnCourseGrade(latest);
}

/**
 * the next 2 functions extract the points or grade
 * of the lest time that a course appeard in a semester
 * points- return as doubled because of function! do not forget!
 * semesterGradesUpdateGrade: updates a grade in semester
 * @param semester
 * @param course_id
 * @return
 * SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
 */

int gradeLastTimeOfCourseInSemester(SemesterGrades semester,int course_id){
    CourseGrade latest = NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if (courseGradeReturnCourseid(iterator)==course_id){
            latest = iterator;
        }
    }
    if (latest==NULL) {//course didn't appear in semester at all
        return 0;
    }
    return courseGradeReturnCourseGrade(latest);
}

int convertStringToInt(char* string){
    while(*string == '0'){
        string++;
    }
    char *temp = string;//will iterate to get number of digits that's not 0
    int counter = 1;
    temp++;
    while(*temp != '\0' && *temp != '.'){
        counter*=10;
        temp++;
    }
    double num = 0;
    while(*string != '\0' && *string != '.'){
        num+= (double)(*string - '0')*counter;
        string++;
        counter/=10;
    }
    if(*string!='\0'){
        string++;
        num+= (double)((*string)-'0')/10;
    }
    return (int)(num*2);
}

int pointsLastTimeOfCourseInSemester (SemesterGrades semester,int course_id){
    CourseGrade latest = NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if (courseGradeReturnCourseid(iterator)==course_id){
            latest = iterator;
        }
    }
    if (latest==NULL) {//course didn't appear in semester at all
        return 0;
    }
    return convertStringToInt(courseGradeReturnCoursePoints(latest));
}

/**
 * functions for printing a semester:
 * convertStringToInt - converts a string into int.
 * findTotalPoints - finds the total points done in a semester.
 * findTotalFailedPoints - finds the points sum of all failed classes in the semester.
 * arrayToSumGrade - sums the elements of the grades array
 * findEffectivePoints - finds the sum of all effective points in the semester.
 * findEffectiveGradeSum - finds the sum of of the effective course grades, when
 *  each grade is multiplied by the number of course points.
 */

 int findTotalPoints (SemesterGrades semester){
    int sum=0;
    LIST_FOREACH(CourseGrade, iterator, semester->courseGrades){
        sum+=convertStringToInt(courseGradeReturnCoursePoints(iterator));
    }
    return sum;
}
int findTotalFailedPoints (SemesterGrades semester){
    int sum=0;
    int tempgrade=0;
    LIST_FOREACH(CourseGrade, iterator, semester->courseGrades){
        tempgrade= courseGradeReturnCourseGrade(iterator);
        if(tempgrade < 55){
            sum+=convertStringToInt(courseGradeReturnCoursePoints(iterator));
        }
    }
    return sum;
}

int arrayToSumGrade (SemesterGrades semester, int* course_flags,int size, int sum_flag){
    int i=0,sum=0;
    CourseGrade latest;
    while(i<size && *(course_flags+i)!=0) {// for each course that appeared in semester
        latest = listGetFirst(semester->courseGrades);
        LIST_FOREACH(CourseGrade, iterator, semester->courseGrades) {
            int tempcourse = courseGradeReturnCourseid(iterator);
            if (*(course_flags + i) == tempcourse) {
                latest = iterator;
            }
        }
        int temp_grade=1;
        if (sum_flag==1){// sum flags sets use of function- for sum or points.
            temp_grade = courseGradeReturnCourseGrade(latest);
        }
        sum+= temp_grade*
               convertStringToInt(courseGradeReturnCoursePoints(latest));
        i++;
    }
    return sum;
}

int findEffectivePoints (SemesterGrades semester){
    int sum=0;
    int size= listGetSize(semester->courseGrades);
    int *course_flags= malloc(sizeof(int)*size);//flags for if course already appeared
    if (course_flags==NULL){
        return -1;
    }
    for (int i=0; i<size; i++) {
        *(course_flags+i)=0;
    }
    int tempcourse, index = 0;
    LIST_FOREACH(CourseGrade, iterator, semester->courseGrades){
        bool appeard = false;
        tempcourse=courseGradeReturnCourseid(iterator);
        int i;
        for (i=0; i<size; i++){
            if (*(course_flags+i)==tempcourse){
                appeard = true;
                break;
            }
        }
        if (appeard==false) {
            *(course_flags + index) = tempcourse;
            index++;
        }
    }//until here we find what courses already appeared;
    sum = arrayToSumGrade(semester,course_flags,size,0);
    free(course_flags);
    return sum;
}

int findEffectiveGradeSum (SemesterGrades semester){
    int sum=0;
    int size= listGetSize(semester->courseGrades);
    int *course_flags= malloc(sizeof(int)*size);//flags for if course already appeared
    if (course_flags==NULL){
        return -1;
    }
    for (int i=0; i<size; i++) {
        *(course_flags+i)=0;
    }
    int tempcourse, index = 0;
    LIST_FOREACH(CourseGrade, iterator, semester->courseGrades){
        bool appeard = false;
        tempcourse=courseGradeReturnCourseid(iterator);
        int i;
        for (i=0; i<size; i++){
            if (*(course_flags+i)==tempcourse){
                appeard = true;
                break;
            }
        }
        if (appeard==false) {
            *(course_flags + index) = tempcourse;
            index++;
        }
    }//until here we find what courses already appeared;
    sum =  arrayToSumGrade(semester,course_flags,size,1);
    free(course_flags);
    return sum;
}

/**
 * semesterGradesPrintSemester: prints the full semester
 * @return
 */
void semesterGradesPrintSemester(SemesterGrades semester,FILE* file) {
    assert(semester!=NULL);
    int total_points , failed_points, effective_points, effective_point_sum;
    total_points = findTotalPoints(semester);
    failed_points = findTotalFailedPoints(semester);
    effective_points = findEffectivePoints(semester);
    effective_point_sum = findEffectiveGradeSum(semester);
    LIST_FOREACH(CourseGrade, iterator, semester->courseGrades) {
        mtmPrintGradeInfo(file,courseGradeReturnCourseid(iterator),
                          convertStringToInt(courseGradeReturnCoursePoints(iterator)),
                          courseGradeReturnCourseGrade(iterator));
    }
    mtmPrintSemesterInfo(file,semester->semester,total_points,failed_points,
                         effective_points,effective_point_sum);
}

/**
 * semesterGradesPrintCourseInSemester: prints a specific course in a semester
 * @param course_id
 * @return
 */
void semesterGradesPrintCourseInSemester(SemesterGrades semester,FILE* file, int course_id){
    CourseGrade latest = NULL;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if (courseGradeReturnCourseid(iterator)==course_id){
            latest = iterator;
        }
    }
    if(latest == NULL){
        return;
    }
    mtmPrintGradeInfo(file, courseGradeReturnCourseid(latest),
                      convertStringToInt(courseGradeReturnCoursePoints(latest)),
                      courseGradeReturnCourseGrade(latest));
}

/**
 * semesterGradesSortByHelper: sorting courses as required for best/worst lists of courses.
 * @return
 * SEMESTER_GRADES_SUCCESS - list sorted
 */
SemesterGradesResult semesterGradesSortByHelper(SemesterGrades semesterGrade, ListSortKey key){
    if(semesterGrade == NULL){
        return SEMESTER_GRADES_INVALID_PARAMETERS;
    }
    listSort(semesterGrade->courseGrades,compareCourseGrade,key);
    return SEMESTER_GRADES_SUCCESS;
}


/**
 * semesterGradesPrintAmount: prints amount of courses from semster
 * @param semester
 * @param amount - amount of courses to print
 * @return
 */
void semesterGradesPrintAmount(SemesterGrades semester,FILE* file,int amount){
    int i = 1;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades) {
        if (i<=amount){
            courseGradePrintCourseGrade(iterator,file);
            i++;
        }else {
            break;
        }
    }
}
/**
 * bestGradeOfCourseInSemester- return the best grade the student
 * got in this course in the semester
* @param semester
* @param course_id
* @return
* SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
*/

int bestGradeOfCourseInSemester(SemesterGrades semester,int course_id){
    int grade=-1;
    int tempgrade;
    LIST_FOREACH(CourseGrade,iterator,semester->courseGrades){
        if (courseGradeReturnCourseid(iterator)==course_id) {
            tempgrade = courseGradeReturnCourseGrade(iterator);
            if (tempgrade > grade) {
                grade = tempgrade;
            }
        }
    }
    return grade;
}

/**
 * semesterGradesDestroy: De-allocates a GradeSheet.
 *
 * @return
 */
void semesterGradesDestroy(SemesterGrades semesterGrade){
    if(semesterGrade!=NULL){
        listDestroy(semesterGrade->courseGrades);
    }
    free(semesterGrade);
}
