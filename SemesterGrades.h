
#ifndef SEMESTERGRADES_H
#define SEMESTERGRADES_H

#include "list.h"
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

/** Type for defining the student */
typedef struct SemesterGrades_t *SemesterGrades;

/** Type used for returning error codes from student functions */
typedef enum SemesterGradesResult_t {
    SEMESTER_GRADES_OUT_OF_MEMORY,
    SEMESTER_GRADES_INVALID_PARAMETERS,
    SEMESTER_GRADES_COURSE_NOT_EXIST,
    SEMESTER_GRADES_SUCCESS
} SemesterGradesResult;

/**
 * semesterGradesCreate: Allocates a new empty SemesterGrades.
 *
 * @return
 * SEMESTER_GRADES_SUCCESS - new grade sheet created
 * SEMESTER_GRADES_OUT_OF_MEMORY - memory problem occurred
 */
SemesterGradesResult semesterGradesCreate(int semester, SemesterGrades *semesterGrade);

/**
 * semesterGradesCopy: copies a semester.
 * @return
 * SemesterGrades - if succeed
 * NULL - otherwise
 */
SemesterGrades semesterGradesCopy(SemesterGrades semesterGrade);

//this function returns pints but as a string and not int
char* CHpointsLastCourseInSemester(SemesterGrades semester,int course_id);

/**
 * semesterGradesInsertHelper: prints a specific course in a semester
 * @param semeter, helper
 * @return
 * SEMESTER_GRADES_SUCCESS - helper inserted
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 */
SemesterGradesResult semesterGradesInsertHelper(SemesterGrades semester, int helper);


/**
 * semesterGradesGetSemester: returns the semester of the semester.
 *
 * @return
 * -1 - if NULL
 * semester - otherwise
 */
int semesterGradesGetSemester(SemesterGrades semesterGrade);

/**
 * semesterGradesAddGrade: insert a grade to the semester
 * @param course_id
 * @param points
 * @param grade
 * @return
 * SEMESTER_GRADES_SUCCESS - grade added
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 */
SemesterGradesResult semesterGradesAddGrade(SemesterGrades semester, int course_id, char* points, int grade);

/**
 * semesterGradesRemoveGrade: removes a grade from the semester
 * @param course_id
 * @param semester
 * @return
 * SEMESTER_GRADES_SUCCESS - grade removed
 * SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
 */
SemesterGradesResult semesterGradesRemoveGrade(SemesterGrades semester, int course_id);

/**
 * semesterGradesUpdateGrade: updates a grade in semester
 * @param grade
 * @param course_id
 * @return
 * SEMESTER_GRADES_SUCCESS - grade updated
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 * SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
 */
SemesterGradesResult semesterGradesUpdateGrade(SemesterGrades semester, int course_id, int grade);

/**
 * semesterGradesPrintSemester: prints the full semester
 * @return
 */
void semesterGradesPrintSemester(SemesterGrades semester,FILE* file);

/**
 * semesterGradesIsEmpty: check if semester is empty.
 *
 * @return 0 of emtpy, else 1
 */
int semesterGradesIsEmpty(SemesterGrades semesterGrade);

/**
 * semesterGradesPrintCourseInSemester: prints a specific course in a semester
 * @param course_id
 * @return
 */
void semesterGradesPrintCourseInSemester(SemesterGrades semester,FILE* file, int course_id);

/**
 * functions for printing a stmester:
 * convertStringToInt - converts a string into int.
 * findTotalPoints - finds the total points done in a semester.
 * findTotalFailedPoints - finds the points sum of all failed classes in the semester.
 * findEffectivePoints - finds the sum of all effective points in the semester.
 * findEffectiveGradeSum - finds the sum of of the effective course grades, when
 * each grade is multiplied by the number of course points.
 * arrayToSumGrade - sums the elements of the grades array
 */
int convertStringToInt(char* string);

int findTotalPoints (SemesterGrades semester);

int findTotalFailedPoints (SemesterGrades semester);

int findEffectivePoints (SemesterGrades semester);

int findEffectiveGradeSum (SemesterGrades semester);

int arrayToSumGrade (SemesterGrades semester, int* course_flags,int size, int sum_flag);
/**
 * the next 2 functions extract the points or grade
 * of the lest time that a course appeard in a semester
 * semesterGradesUpdateGrade: updates a grade in semester
 * @param semester
 * @param course_id
 * @return
 * SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
 */

int gradeLastTimeOfCourseInSemester(SemesterGrades semester,int course_id);

int pointsLastTimeOfCourseInSemester (SemesterGrades semester,int course_id);


/**
 * DoesGradeExists - checks if a grade exists in the semester
 * @param semester
 * @param course_id
 * @return
 * -1 - the semester doesn't hold the course
 * grade otherwise
 */

int DoesGradeExists(SemesterGrades semester,int course_id);

void semesterGradesPrintHelper(SemesterGrades semester);

/**
 * semesterGradesSortByHelper: sorting courses as required for best/worst lists of courses.
 * @return
 * SEMESTER_GRADES_SUCCESS - list sorted
 */
SemesterGradesResult semesterGradesSortByHelper(SemesterGrades semesterGrade, ListSortKey key);

/**
 * semesterGradesPrintAmount: prints amount of courses from semster
 * @param semester
 * @param amount - amount of courses to print
 * @return
 */
void semesterGradesPrintAmount(SemesterGrades semester,FILE* file,int amount);

/**
 * bestGradeOfCourseInSemester- return the best grade the student
 * got in this course in the semester
* @param semester
* @param course_id
* @return
* SEMESTER_GRADES_COURSE_NOT_EXIST - the semester doesn't hold the course
*/

int bestGradeOfCourseInSemester(SemesterGrades semester,int course_id);

/**
 * semesterGradesDestroy: De-allocates a GradeSheet.
 *
 * @return
 * SEMESTER_GRADES_SUCCESS - new grade sheet created
 * SEMESTER_GRADES_INVALID_PARAMETERS - parameters aren't valid
 */
void semesterGradesDestroy(SemesterGrades semesterGrade);

#endif //SEMESTERGRADES_H
