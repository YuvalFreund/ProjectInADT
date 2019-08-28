#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "CleanCourse.h"


/**
 * CleanCourse structure
 *
 * CleanCourse holds a list of courses with the last semesters the course was done by the student.

 * The following functions are available:
 * ff
 */

/**
 * definition of CleanCourse struct - a helper struct for printing clean sheets
 */
struct CleanCourse_t{
    int course_id;
    List last_semester;
};

static ListElement lastSemesterCopy(ListElement element) {
    if(element == NULL){
        return NULL;
    }
    int *new = malloc(sizeof(int));
    *new = *(int *)element;
    return new? new : NULL;
}

static void freeLastSemester(ListElement element) {
    free(element);
}

static int compareLastSemester(ListElement element1,ListElement element2,ListSortKey key) {
    return *(int *)element1 - *(int *)element2;
}

CleanCourseResult cleanCourseCreate(int course_id,CleanCourse *course) {
    if(course_id < 0 || course_id > 1000000){
        return CLEAN_COURSE_INVALID_PARAMETERS;
    }
    *course = malloc(sizeof(struct CleanCourse_t));
    if((*course) == NULL){
        return CLEAN_COURSE_OUT_OF_MEMORY;
    }
    (*course)->last_semester = listCreate(lastSemesterCopy,freeLastSemester);
    (*course)->course_id = course_id;
    return CLEAN_COURSE_SUCCESS;
}

CleanCourse cleanCourseCopy(CleanCourse course){
    if(course == NULL){
        return NULL;
    }
    CleanCourse new;
    cleanCourseCreate(course->course_id,&new);
    listDestroy(new->last_semester);
    new->last_semester = listCopy(course->last_semester);
    return new;
}


int getCleanCourseId(CleanCourse course){
    if(course==NULL){
        return -1;
    }
    return course->course_id;
}

CleanCourseResult cleanCourseAdd(CleanCourse course, int semester) {
    if(course==NULL){
        return CLEAN_COURSE_INVALID_PARAMETERS;
    }
    int *temp = malloc(sizeof(int));
    if(temp == NULL){
        return CLEAN_COURSE_OUT_OF_MEMORY;
    }
    *temp = semester;
    listInsertLast(course->last_semester,temp);
    listSort(course->last_semester,compareLastSemester,0);
    free(temp);
    return CLEAN_COURSE_SUCCESS;
}

CleanCourseResult cleanCourseRemove(CleanCourse course, int semester) {
    if(course == NULL || semester<0){
        return CLEAN_COURSE_INVALID_PARAMETERS;
    }
    LIST_FOREACH(int*,iterator,course->last_semester){
        if(semester == *iterator){
            listRemoveCurrent(course->last_semester);
            return CLEAN_COURSE_SUCCESS;
        }
    }
    return CLEAN_COURSE_SEMESTER_NOT_EXISTS;
}

void print_check(CleanCourse course){
    if(course==NULL){
        return;
    }
    LIST_FOREACH(int*,iterator,course->last_semester){
        printf("%d,",*iterator);
    }
    printf("\n");
}
/**
* cleanCourseGetLastSemester-
 * @param- clean course
 * @return- last semester that course was done as int
*/
int cleanCourseGetLastSemester(CleanCourse course){
    int last_semester = -1;
    LIST_FOREACH(int*,iterator,course->last_semester){
        last_semester=*iterator;
    }
    return last_semester;
}

void destroyCleanCourse(CleanCourse course) {
    if(course != NULL){
        listDestroy(course->last_semester);
    }
    free(course);
}
