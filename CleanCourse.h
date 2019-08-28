
#ifndef CLEANCOURSE_H
#define CLEANCOURSE_H

/**
 * CleanCourse structure
 *
 * CleanCourse holds a list of courses with the last semesters the course was done by the student.

 * The following functions are available:
 * ff
 */

typedef struct CleanCourse_t *CleanCourse;

/** Type used for returning error codes from student functions */
typedef enum CleanCourseResult_t {
    CLEAN_COURSE_INVALID_PARAMETERS,
    CLEAN_COURSE_OUT_OF_MEMORY,
    CLEAN_COURSE_SEMESTER_NOT_EXISTS,
    CLEAN_COURSE_SUCCESS
} CleanCourseResult;

/**
 * cleanCourseCreate: create a new cleanCourse
 * @param course_id
 * @param course - pointer for creating the structure
 * @return
 * GRADE_SHEET_SUCCESS - grade added
 * GRADE_SHEET_OUT_OF_MEMORY - parameters aren't valid
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 */
CleanCourseResult cleanCourseCreate(int course_id,CleanCourse *course);

/**
 * getCleanCourseId: returns the id of the course
 * @param course
 * @return
 * -1 - if null
 * course_id - otherwise - what we wanted
 */
int getCleanCourseId(CleanCourse course);
/**
* cleanCourseGetLastSemester-
 * @param- clean course
 * @return- last semester that course was done as int
*/
int cleanCourseGetLastSemester(CleanCourse course);
/**
 * cleanCourseCopy: adds a semester the course was made
 * @param course
 * @return
 * -1 - if null
 * course_id - otherwise - what we wanted
 */
CleanCourse cleanCourseCopy(CleanCourse course);

/**
 * cleanCourseAdd: adds a semester the course was made
 * @param course
 * @return
 * -1 - if null
 * course_id - otherwise - what we wanted
 */
CleanCourseResult cleanCourseAdd(CleanCourse course, int semester);

CleanCourseResult cleanCourseRemove(CleanCourse course, int semester);

void print_check(CleanCourse course);

void destroyCleanCourse(CleanCourse course);

#endif //CLEANCOURSE_H
