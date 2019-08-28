
#ifndef COURSEGRADES_H
#define COURSEGRADES_H

/**
 * CourseGrade structure
 *
 * Course grade will hold information about a specific course
 * courseGradeCreate: Allocates a new empty CourseGrade.
 * courseGradePrintCourseGrade: prints a course grade with the given output functions
 * courseGradeDestroy: De-allocates a new empty CourseGrade.
 * The following functions are available:
 *
 */

/** Type for defining the student */
typedef struct CourseGrade_t *CourseGrade;

/** Type used for returning error codes from student functions */
typedef enum CourseGradeResult_t {
    COURSE_GRADE_OUT_OF_MEMORY,
    COURSE_GRADE_INVALID_PARAMETERS,
    COURSE_GRADE_SUCCESS
} CourseGradeResult;

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
CourseGradeResult courseGradeCreate(int course_id, char* points, int grade,CourseGrade *courseGrade);

/**
 * courseGradeUpdateGrade: updates a grade for CourseGrade.
 * @param courseGrade
 * @param grade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_INVALID_PARAMETERS - if NULL
 */
CourseGradeResult courseGradeUpdateGrade(CourseGrade courseGrade, int grade);

/**
 * courseGradePrintCourseGrade: prints a course grade with the given output functions
 * @param courseGrade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_INVALID_PARAMETERS - if NULL
 */
CourseGradeResult courseGradePrintCourseGrade(CourseGrade courseGrade, FILE* file);

/**
 * courseGradeCompareGrade: compare the courses by Id
 * @param courseGrade1
 * @param courseGrade2
 * @return
 * int - if grade courseGrade1 > id courseGrade 2 value is bigger than 0, if 0 - equal, else
 * grade courseGrade2 is bigger
 */
int courseGradeCompareGrade(CourseGrade courseGrade1,CourseGrade courseGrade2);

/**
 * courseGradeCopy: compare the courses by Id
 * @param courseGrade1
 * @return
 * copies courseGrade 1 into a new courseGrade structure (with allocation of memory)
 */
CourseGrade courseGradeCopy(CourseGrade courseGrade1);

/**
 * courseGradeReturnCourseid: gets a courseGrade and returns the id of the course
 * @param courseGrade
 * @return
 * course id or -1 if courseGrade is NULL
 */
int courseGradeReturnCourseid(CourseGrade courseGrade);

/**
 * courseGradeReturnCourseGrade: gets a courseGrade and returns the grade of the course
 * @param courseGrade
 * @return
 * grade or -1 if courseGrade is NULL
 */
int courseGradeReturnCourseGrade(CourseGrade courseGrade);
/**
 * courseGradeReturnCoursePoints: gets a courseGrade and returns the Points of the course
 * @param courseGrade
 * @return
 * points or NULL if courseGrade if NULL
 */
char* courseGradeReturnCoursePoints(CourseGrade courseGrade);
/**
 * courseGradeInsertHelper: insert the helper in to course grade
 * @param courseGrade
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 * COURSE_GRADE_INVALID_PARAMETERS - if NULL
 */
CourseGradeResult courseGradeInsertHelper(CourseGrade courseGrade,int helper);
/**
 * courseGradeDestroy: De-allocates a new empty CourseGrade.
 * @param course_id
 * @return
 * COURSE_GRADE_SUCCESS - new grade sheet created
 */
int courseGradeGetHelper(CourseGrade courseGrade);

/**
 * courseGradeGetHelper- get the helper from the coursegrade
 * @param courseGrade
 * @return
 * -1 if course is null
 * else - helper
 */
void courseGradeDestroy(CourseGrade courseGrade);


#endif //COURSEGRADES_H
