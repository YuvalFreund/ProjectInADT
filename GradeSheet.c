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


/**
 * GradeSheet structure
 *
 * GradeSheet holds the transcript of a student devided by semesters.
 * Also holds a list of courses with the last semester the course was done by the student.

 * The following functions are available:
 * GradeSheetCreate: Allocates a new empty GradeSheet.
 * gradeSheetAddGrade: insert a grade to the transcript (to the specific semester
 * gradeSheetRemoveGrade: removes a grade from the transcript (from the specific semester
 * gradeSheetUpdateGrade: updates a grade in transcript (from the specific semester
 * gradeSheetPrintFull: prints the full transcript
 * gradeSheetPrintClean: prints the clean transcript
 * gradeSheetPrintBest: prints the best grades (that counts) int the transcript
 * gradeSheetPrintWorst: prints the worst grades (that counts) in the trancript
 * gradeSheetDestroy: De-allocates a GradeSheet.
 */

/**
 * definition of GradeSheet struct
 */
struct GradeSheet_t{
    List semesters_list;
    List clean_transcript;
};

/**
 * list cleanSemester function definitions -
 * copyCleanCourse - copy elements
 * freeCleanCourse - De-allocates elements
 * compareCleanCourse - compares elements
 */
static ListElement copyCleanCourse(ListElement element) {
    if (element == NULL) {
        return NULL;
    }
    CleanCourse new;
    new = cleanCourseCopy(element);
    return new ? new : NULL;
}
static void freeCleanCourse(ListElement element) {
    destroyCleanCourse(element);
}

int compareCleanCourse(ListElement element1,ListElement element2,ListSortKey key) {
    assert(element1 != NULL && element2 != NULL);
    int Id1 = getCleanCourseId(element1);
    int Id2 = getCleanCourseId(element2);
    int distance = Id1 - Id2;
    key = 0;//we dont need it
    return distance;
}

/**
 * list semesters function definitions -
 * copySemester - copy elements
 * freeSemester - De-allocates elements
 * compareSemester - compares elements
 */
static ListElement copySemester(ListElement element) {
    if (element == NULL) {
        return NULL;
    }
    SemesterGrades new = semesterGradesCopy(element);
    return new ? new : NULL;
}
static void freeSemester(ListElement element) {
    semesterGradesDestroy(element);
}

int compareSemester(ListElement element1,ListElement element2,ListSortKey key) {
    assert(element1 != NULL && element2 != NULL);
    int distance = semesterGradesGetSemester(element1) -
            semesterGradesGetSemester(element2);
    key = 0;//we dont need it
    return distance;
}

/**
 * GradeSheetCreate: Allocates a new empty GradeSheet.
 *
 * @return
 * GRADE_SHEET_SUCCESS - new grade sheet created
 * GRADE_SHEET_OUT_OF_MEMORY - memory problem occurred
 */
GradeSheetResult gradeSheetCreate(GradeSheet *gradeSheet){
    *gradeSheet = malloc(sizeof(struct GradeSheet_t));
    if((*gradeSheet) == NULL){
        return GRADE_SHEET_OUT_OF_MEMORY;
    }
    (*gradeSheet)->clean_transcript = listCreate(copyCleanCourse,freeCleanCourse);
    if((*gradeSheet)->clean_transcript == NULL){
        return GRADE_SHEET_OUT_OF_MEMORY;
    }
    (*gradeSheet)->semesters_list = listCreate(copySemester,freeSemester);
    if((*gradeSheet)->semesters_list == NULL){
        return GRADE_SHEET_OUT_OF_MEMORY;
    }
    return GRADE_SHEET_SUCCESS;
}

/**
 * gradeSheetAddGrade: insert a grade to the transcript (to the specific semester
 * and update the latest semester in course list)
 * @param semester
 * @param course_id
 * @param points
 * @param grade
 * @return
 * GRADE_SHEET_SUCCESS - grade added
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 */
GradeSheetResult gradeSheetAddGrade(GradeSheet gradeSheet, int semester, int course_id, char* points, int grade){
    if(gradeSheet == NULL || semester <= 0){
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    bool flag = false;
    //if clean_course exists - add semester to existing grade in clean transcript
    LIST_FOREACH(CleanCourse ,iterator,gradeSheet->clean_transcript){
        if(getCleanCourseId(iterator) == course_id){
            if(cleanCourseAdd(iterator,semester)!=CLEAN_COURSE_SUCCESS){
                return GRADE_SHEET_INVALID_PARAMETERS;
            }
            flag = true;break;
        }
    }
    //if clean grade doesn't exist - insert a new grade to clean transcript
    if(flag == false) {
        CleanCourse new_course;
        if (cleanCourseCreate(course_id, &new_course) != CLEAN_COURSE_SUCCESS) {
            return GRADE_SHEET_INVALID_PARAMETERS;
        }
        if (new_course == NULL) {
            return GRADE_SHEET_OUT_OF_MEMORY;
        }
        cleanCourseAdd(new_course, semester);
        listInsertLast(gradeSheet->clean_transcript,new_course);
        listSort(gradeSheet->clean_transcript,compareCleanCourse,0);
        destroyCleanCourse(new_course);
    }
    //insert grade to semesters list
    LIST_FOREACH(SemesterGrades ,iterator,gradeSheet->semesters_list){//if semester exists
        if(semesterGradesGetSemester(iterator) == semester){
            if(semesterGradesAddGrade(iterator,course_id,points,grade)!=SEMESTER_GRADES_SUCCESS){
                return GRADE_SHEET_INVALID_PARAMETERS;
            }
            return GRADE_SHEET_SUCCESS;
        }
    }
    //if semester doesn't exist
    SemesterGrades new_semester;
    if(semesterGradesCreate(semester,&new_semester) != SEMESTER_GRADES_SUCCESS){
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    if(new_semester == NULL){
        return GRADE_SHEET_OUT_OF_MEMORY;
    }
    if(semesterGradesAddGrade(new_semester,course_id,points,grade)!=SEMESTER_GRADES_SUCCESS){
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    listInsertLast(gradeSheet->semesters_list,new_semester);
    listSort(gradeSheet->semesters_list,compareSemester,0);
    semesterGradesDestroy(new_semester);
    return GRADE_SHEET_SUCCESS;
}

/**
 * gradeSheetRemoveGrade: removes a grade from the transcript (from the specific semester
 * and update the latest semester in course list)
 * @param semester
 * @param course_id
 * @return
 * GRADE_SHEET_SUCCESS - grade removed
 * GRADE_SHEET_COURSE_NOT_EXIST - the transcript doesn't hold the course
 */
GradeSheetResult gradeSheetRemoveGrade(GradeSheet gradeSheet, int semester, int course_id){
    if(gradeSheet == NULL){
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    //remove from clean_transcript
    LIST_FOREACH(CleanCourse ,iterator,gradeSheet->clean_transcript){
        if(getCleanCourseId(iterator) == course_id){
            if(cleanCourseRemove(iterator,semester)!=CLEAN_COURSE_SUCCESS){
                return GRADE_SHEET_INVALID_PARAMETERS;
            }
            break;
        }
    }
    //remove from semester
    LIST_FOREACH(SemesterGrades ,iterator,gradeSheet->semesters_list){
        if(semesterGradesGetSemester(iterator) == semester){
            if(semesterGradesRemoveGrade(iterator,course_id)!=SEMESTER_GRADES_SUCCESS){
                return GRADE_SHEET_INVALID_PARAMETERS;
            }
            break;
        }
    }
    LIST_FOREACH(SemesterGrades ,iterator,gradeSheet->semesters_list) {
        if (semesterGradesGetSemester(iterator) == semester) {
            if (semesterGradesIsEmpty(iterator) == 0) {
                listRemoveCurrent(gradeSheet->semesters_list);
            }
        }
    }
    return GRADE_SHEET_SUCCESS;
}

/**
 * gradeSheetUpdateGrade: updates a grade in transcript (from the specific semester
 * and update the latest semester in course list)
 * @param grade
 * @param course_id
 * @return
 * GRADE_SHEET_SUCCESS - grade updated
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 * GRADE_SHEET_COURSE_NOT_EXIST - the student didn't take the course
 */
GradeSheetResult gradeSheetUpdateGrade(GradeSheet gradeSheet, int course_id, int grade){
    if(gradeSheet == NULL){
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    SemesterGrades latest = NULL;
    //in clean transcript there's not need to save the grade hence it takes the grade from the semester
    //updates the grade in the semester
    LIST_FOREACH(SemesterGrades ,iterator,gradeSheet->semesters_list){
        if(DoesGradeExists(iterator,course_id) != -1){
            latest = iterator;
        }
    }
    if(latest ==NULL){
        return GRADE_SHEET_COURSE_NOT_EXIST;
    }
    if(grade < 0 || grade > 100){
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    semesterGradesUpdateGrade(latest,course_id,grade);
    return GRADE_SHEET_SUCCESS;
}

/**
 * gradeSheetPrintFull: prints the full transcript
 * @return
 */
void gradeSheetPrintFull(GradeSheet gradeSheet,FILE* file){
    int totalPoints = 0;
    int failedPoints = 0;
    int effectivePoints = 0;
    int effecitiveGradeSum = 0;
    LIST_FOREACH(SemesterGrades,iterator,gradeSheet->semesters_list){
        semesterGradesPrintSemester(iterator,file);
        totalPoints += findTotalPoints(iterator);
        failedPoints += findTotalFailedPoints(iterator);
    }// adds total points done and failed. effective for sheet:
    LIST_FOREACH(CleanCourse,iterator,gradeSheet->clean_transcript){
        int tempID = getCleanCourseId(iterator);
        if(tempID <= 399999 && tempID >= 300000) {//sport course
            //goes to all semesters, find last time course was done in each
            LIST_FOREACH(SemesterGrades,iterator2,gradeSheet->semesters_list){
                effectivePoints +=
                        pointsLastTimeOfCourseInSemester(iterator2,tempID);
                effecitiveGradeSum+=
                        pointsLastTimeOfCourseInSemester(iterator2,tempID)*
                        gradeLastTimeOfCourseInSemester(iterator2,tempID);
            }
        }
        else{//now for courses that are not sport
            int lastSemester = cleanCourseGetLastSemester(iterator);
            LIST_FOREACH(SemesterGrades,iterator2,gradeSheet->semesters_list){
                if (semesterGradesGetSemester(iterator2)==lastSemester){
                    effectivePoints+=
                            pointsLastTimeOfCourseInSemester(iterator2,tempID);
                    effecitiveGradeSum +=
                            pointsLastTimeOfCourseInSemester(iterator2,tempID)*
                    gradeLastTimeOfCourseInSemester(iterator2,tempID);
                }
            }
        }
    }
    mtmPrintSummary(file,totalPoints,failedPoints,
                    effectivePoints,effecitiveGradeSum);
}

/**
 * gradeSheetPrintClean: prints the clean transcript
 * @return
 */
void gradeSheetPrintClean(GradeSheet gradeSheet,FILE* file){
    if(gradeSheet == NULL){
        return;
    }
    int effective_points = 0, effective_grade = 0, last_semester = 0, course_number = 0;
    LIST_FOREACH(CleanCourse,iterator_clean,gradeSheet->clean_transcript){
        course_number = getCleanCourseId(iterator_clean);
        if(course_number >= 390000 && course_number <= 399999){
            LIST_FOREACH(SemesterGrades,iterator_semester,gradeSheet->semesters_list){
                semesterGradesPrintCourseInSemester(iterator_semester,file,course_number);//should print last
                effective_points+=pointsLastTimeOfCourseInSemester(iterator_semester,course_number);
                effective_grade+=gradeLastTimeOfCourseInSemester(iterator_semester,course_number)*
                        pointsLastTimeOfCourseInSemester(iterator_semester,course_number);
            }
        }
        else{
            last_semester = cleanCourseGetLastSemester(iterator_clean);
            LIST_FOREACH(SemesterGrades,iterator_semester,gradeSheet->semesters_list){
                if(semesterGradesGetSemester(iterator_semester) == last_semester){
                    semesterGradesPrintCourseInSemester(iterator_semester,file,course_number);//should print last
                    effective_points+=pointsLastTimeOfCourseInSemester(iterator_semester,course_number);
                    effective_grade+=gradeLastTimeOfCourseInSemester(iterator_semester,course_number)*
                                     pointsLastTimeOfCourseInSemester(iterator_semester,course_number);
                }
            }
        }
    }
    mtmPrintCleanSummary(file,effective_points,effective_grade);
}

SemesterGrades validList(SemesterGrades validCourses,GradeSheet gradeSheet){
    SemesterGrades new;
    semesterGradesCreate(0,&new);// creates a fake semester to hold data
    LIST_FOREACH(CleanCourse,iterator,gradeSheet->clean_transcript) {
        int tempID = getCleanCourseId(iterator);
        int tempLastSemester;
        if (tempID > 399999 || tempID < 390000) { //not a sport course
            tempLastSemester = cleanCourseGetLastSemester(iterator);
            LIST_FOREACH(SemesterGrades,iterator2,gradeSheet->semesters_list){
                if (semesterGradesGetSemester(iterator2) == tempLastSemester) {
                    semesterGradesAddGrade(new, tempID,
                                           CHpointsLastCourseInSemester(iterator2,tempID),
                                           gradeLastTimeOfCourseInSemester(iterator2,tempID));
                    semesterGradesInsertHelper(new,tempLastSemester);
                    break;//the valid grade of this course was added
                }
            }
        } else {// now for sport course get every time course was done
            LIST_FOREACH(SemesterGrades,iter2,gradeSheet->semesters_list){
                if(gradeLastTimeOfCourseInSemester(iter2,tempID)>0) {
                    semesterGradesAddGrade(new, tempID,
                                           CHpointsLastCourseInSemester(iter2, tempID),
                                           gradeLastTimeOfCourseInSemester(iter2, tempID));
                    semesterGradesInsertHelper(new,tempLastSemester);
                }
            }
        }
    }
    return new;
}

/**
 * gradeSheetPrintBest: prints the best grades (that counts) in the transcript
 * @param amount
 * @return
 * GRADE_SHEET_SUCCESS - grade printed
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 *
*/

GradeSheetResult gradeSheetPrintBest(GradeSheet gradeSheet,FILE* file,int amount) {
    if (amount < 1) {
        return GRADE_SHEET_INVALID_PARAMETERS;
    }
    SemesterGrades validCourses = NULL;
    validCourses = validList(validCourses, gradeSheet);
    int *temp = malloc(sizeof(int));
    *temp = -1;
    semesterGradesSortByHelper(validCourses,temp);
    semesterGradesPrintAmount(validCourses,file,amount);
    semesterGradesDestroy(validCourses);
    free(temp);
    return GRADE_SHEET_SUCCESS;
}

/**
 * gradeSheetPrintWorst: prints the worst grades (that counts) in the trancript
 * @param amount
 * @return
 * GRADE_SHEET_SUCCESS - grade printed
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 */

GradeSheetResult gradeSheetPrintWorst(GradeSheet gradeSheet,FILE* file,int amount){
    if(amount<1){
        return  GRADE_SHEET_INVALID_PARAMETERS;
    }
    SemesterGrades validCourses = NULL;
    validCourses = validList(validCourses, gradeSheet);
    int *temp = malloc(sizeof(int));
    *temp = 1;
    semesterGradesSortByHelper(validCourses,temp);
    semesterGradesPrintAmount(validCourses,file,amount);
    semesterGradesDestroy(validCourses);
    free(temp);
    return GRADE_SHEET_SUCCESS;
}

/**
 * copyGradeSheet: copies a GradeSheet.
 *
 * @return
 * NULL - if grade sheet to be copied is NULL
 * grade sheet otherwise
 */
GradeSheet copyGradeSheet (GradeSheet gradeSheet){
    if (gradeSheet==NULL){
        return NULL;
    }
    GradeSheet new_sheet;
    if(gradeSheetCreate(&new_sheet)!=GRADE_SHEET_SUCCESS){
        return NULL;
    }
    listDestroy(new_sheet->clean_transcript);
    listDestroy(new_sheet->semesters_list);
    new_sheet->semesters_list = listCopy(gradeSheet->semesters_list);
    new_sheet->clean_transcript = listCopy(gradeSheet->clean_transcript);
    return new_sheet;
}
/**
 * gradeSheetGetBestGrade
 * @param- gradesheet, course_id
  * @return
 * best grade of student in this subject, at any time
 */
int gradeSheetGetBestGrade(GradeSheet gradeSheet, int course_id){
    if (course_id<0 || course_id>1000000){
        return -1;
    }
    int value = -1;
    int tempgrade;
    LIST_FOREACH(SemesterGrades ,iter1,gradeSheet->semesters_list){
            tempgrade = bestGradeOfCourseInSemester(iter1,course_id);
             if (tempgrade > value) {
                value = tempgrade;
            }
        }
    return value;
}

/**
 * gradeSheetDoesGradeExists - checks if a grade is in the sheet
 * @param- gradesheet, course_id
  * @return
 * 1 if the grade is there, 0 otherwise
 */
int gradeSheetDoesGradeExists(GradeSheet gradeSheet, int course_id){
    assert(gradeSheet!=NULL);
    LIST_FOREACH(SemesterGrades,iterator,gradeSheet->semesters_list){
        if(DoesGradeExists(iterator,course_id)>=0){
            return 1;
        }
    }
    return 0;
}

/**
 * gradeSheetDestroy: De-allocates a GradeSheet.
 *
 * @return
 * GRADE_SHEET_SUCCESS - new grade sheet created
 */
void gradeSheetDestroy(GradeSheet gradeSheet){
    if(gradeSheet!=NULL){
        listDestroy(gradeSheet->semesters_list);
        listDestroy(gradeSheet->clean_transcript);
    }
    free(gradeSheet);
}
