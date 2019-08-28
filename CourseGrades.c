#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "mtm_ex3.h"
#include "CourseGrades.h"


/**
 * CourseGrade structure
 * Course grade will hold information about a specific course
 */

/**
 * definition of CourseGrade struct
 */
struct CourseGrade_t{
    int course_id;
    char* points;
    int grade;
    int helper;
};


static bool checkPoint(char* points){
    if(points == NULL){
        return false;
    }
    if(*points == '.'){
        return false;
    }
    while(*points != '\0' && *points != '.') {
        if (*points < '0' || *points > '9') {//not a number
            return false;
        }
        points++;
    }
    //first digit is a series of numbers
    if(*points == '\0') {
        return true;
    }
    if(*points != '.'){//second char is a decimal point
        return false;
    }
    points++;
    if(*points == '\0' || (*points != '5' && *points != '0')){//the number isn't "X.5"/"X.0"
        return false;
    }
    return true;
}
/**
 * courseGradeCreate: Allocates a new empty CourseGrade.
 * @param course_id
 * @param points
 * @param grades
 * @param courseGrade - a pointer to return the created courseGrade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_OUT_OF_MEMORY - memory problem occurred
 * COURSE_GRADE_INVALID_PARAMETERS
 */
CourseGradeResult courseGradeCreate(int course_id, char* points,
                                    int grade,CourseGrade *courseGrade){
    if(course_id >= 10000000 || course_id <= 0 ||grade < 0
       || grade > 100 || !checkPoint(points)){
        return COURSE_GRADE_INVALID_PARAMETERS;
    }
    *courseGrade = malloc(sizeof(struct CourseGrade_t));
    if((*courseGrade) == NULL){
        return COURSE_GRADE_OUT_OF_MEMORY;
    }
    (*courseGrade)->points = malloc(sizeof(char)*((strlen(points))+1));
    if((*courseGrade)->points == NULL){
        return COURSE_GRADE_OUT_OF_MEMORY;
    }
    strcpy((*courseGrade)->points,points);
    (*courseGrade)->course_id = course_id;
    (*courseGrade)->grade = grade;
    return COURSE_GRADE_SUCCESS;
}

/**
 * courseGradeUpdateGrade: updates a grade for CourseGrade.
 * @param courseGrade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_INVALID_PARAMETERS - if NULL
 */
CourseGradeResult courseGradeUpdateGrade(CourseGrade courseGrade, int grade){
    if(courseGrade == NULL){
        return COURSE_GRADE_INVALID_PARAMETERS;
    }
    if(grade < 0 || grade > 100){
        return COURSE_GRADE_INVALID_PARAMETERS;
    }
    courseGrade->grade = grade;
    return COURSE_GRADE_SUCCESS;
}
static int convertStringToInt(char* string){
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
/**
 * courseGradePrintCourseGrade: prints a course grade with the given output functions
 * @param courseGrade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_INVALID_PARAMETERS - if NULL
 */
CourseGradeResult courseGradePrintCourseGrade(CourseGrade courseGrade, FILE* file){
    if(courseGrade == NULL){
        return COURSE_GRADE_INVALID_PARAMETERS;
    }
    mtmPrintGradeInfo(file,courseGrade->course_id,
                      convertStringToInt(courseGrade->points),courseGrade->grade);
    return COURSE_GRADE_SUCCESS;
}

/**
 * courseGradeCompareGrade: compare the courses by Id
 * @param courseGrade1
 * @param courseGrade2
 * @return
 * int - if grade courseGrade1 > id courseGrade 2 value is bigger than 0, if 0 - equal, else
 * grade courseGrade2 is bigger
 */
int courseGradeCompareGrade(CourseGrade courseGrade1,CourseGrade courseGrade2){
    assert(courseGrade1 != NULL && courseGrade2 != NULL);
    return courseGrade1->grade-courseGrade2->grade;
}

/**
 * courseGradeCopy: compare the courses by Id
 * @param courseGrade1
 * @return
 * copies courseGrade 1 into a new courseGrade structure (with allocation of memory)
 */
CourseGrade courseGradeCopy(CourseGrade courseGrade1){
    if(courseGrade1 == NULL){
        return NULL;
    }
    CourseGrade new;
    courseGradeCreate(courseGrade1->course_id, courseGrade1->points,courseGrade1->grade,&new);
    return new;
}

/**
 * courseGradeReturnCourseid: gets a courseGrade and returns the id of the course
 * @param courseGrade
 * @return
 * course id or -1 if courseGrade is NULL
 */
int courseGradeReturnCourseid(CourseGrade courseGrade){
    if(courseGrade == NULL){
        return -1;
    }
    return  courseGrade->course_id;
}

/**
 * courseGradeReturnCourseGrade: gets a courseGrade and returns the grade of the course
 * @param courseGrade
 * @return
 * grade or -1 if courseGrade is NULL
 */
int courseGradeReturnCourseGrade(CourseGrade courseGrade){
    if(courseGrade == NULL){
        return -1;
    }
    return  courseGrade->grade;
}
/**
 * courseGradeReturnCoursePoints: gets a courseGrade and returns the Points of the course
 * @param courseGrade
 * @return
 * points or NULL if courseGrade if NULL
 */
char* courseGradeReturnCoursePoints(CourseGrade courseGrade){
    if(courseGrade == NULL){
        return NULL;
    }
    return  courseGrade->points;
}
/**
 * courseGradeInsertHelper: insert the helper in to course grade
 * @param courseGrade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_INVALID_PARAMETERS - if NULL
 */
CourseGradeResult courseGradeInsertHelper(CourseGrade courseGrade,int helper){
    if(courseGrade == NULL){
        return COURSE_GRADE_INVALID_PARAMETERS;
    }
    courseGrade->helper = helper;
    return COURSE_GRADE_SUCCESS;
}
/**
 * courseGradeGetHelper- get the helper from the coursegrade
 * @param courseGrade
 * @return
 * -1 if course is null
 * else - helper
 */
int courseGradeGetHelper(CourseGrade courseGrade){
    if(courseGrade == NULL){
        return -1;
    }
    return courseGrade->helper;
}
/**
 * courseGradeDestroy: De-allocates a new empty CourseGrade.
 * @param course_id
 * @return
 */
void courseGradeDestroy(CourseGrade courseGrade){
    free(courseGrade->points);
    free(courseGrade);
}



