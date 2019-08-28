
#ifndef GRADESHEET_H
#define GRADESHEET_H


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

/** Type for defining the gradesheet */
typedef struct GradeSheet_t *GradeSheet;

/** Type used for returning error codes from student functions */
typedef enum GradeSheetResult_t {
    GRADE_SHEET_OUT_OF_MEMORY,
    GRADE_SHEET_INVALID_PARAMETERS,
    GRADE_SHEET_COURSE_NOT_EXIST,
    GRADE_SHEET_SUCCESS
} GradeSheetResult;

/**
 * GradeSheetCreate: Allocates a new empty GradeSheet.
 *
 * @return
 * GRADE_SHEET_SUCCESS - new grade sheet created
 * GRADE_SHEET_OUT_OF_MEMORY - memory problem occurred
 */
GradeSheetResult gradeSheetCreate(GradeSheet *gradeSheet);

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
GradeSheetResult gradeSheetAddGrade(GradeSheet gradeSheet, int semester,
                                    int course_id, char* points, int grade);

/**
 * gradeSheetRemoveGrade: removes a grade from the transcript (from the specific semester
 * and update the latest semester in course list)
 * @param semester
 * @param course_id
 * @return
 * GRADE_SHEET_SUCCESS - grade removed
 * GRADE_SHEET_COURSE_NOT_EXIST - the transcript doesn't hold the course
 */
GradeSheetResult gradeSheetRemoveGrade(GradeSheet gradeSheet, int semester, int course_id);

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
GradeSheetResult gradeSheetUpdateGrade(GradeSheet gradeSheet, int course_id, int grade);

/**
 * gradeSheetPrintFull: prints the full transcript
 * @return
 */
void gradeSheetPrintFull(GradeSheet gradeSheet,FILE* file);

/**
 * gradeSheetPrintClean: prints the clean transcript
 * @return
 */
void gradeSheetPrintClean(GradeSheet gradeSheet,FILE* file);

/**
 * gradeSheetPrintBest: prints the best grades (that counts) in the transcript
 * @param amount
 * @return
 * GRADE_SHEET_SUCCESS - grade printed
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 */
GradeSheetResult gradeSheetPrintBest(GradeSheet gradeSheet,FILE* file, int amount);

/**
 * gradeSheetPrintWorst: prints the worst grades (that counts) in the trancript
 * @param amount
 * @return
 * GRADE_SHEET_SUCCESS - grade printed
 * GRADE_SHEET_INVALID_PARAMETERS - parameters aren't valid
 */
GradeSheetResult gradeSheetPrintWorst(GradeSheet gradeSheet,FILE* file, int amount);

/**
 * copyGradeSheet: copies a GradeSheet.
 *
 * @return
 * NULL - if grade sheet to be copied is NULL
 * grade sheet otherwise
 */
GradeSheet copyGradeSheet (GradeSheet gradeSheet);
/**
 * gradeSheetGetBestGrade
 * @param- gradesheet, course_id
  * @return
 * best grade of student in this subject, at any time
 */
int gradeSheetGetBestGrade(GradeSheet gradeSheet, int course_id);

/**
 * gradeSheetDoesGradeExists - checks if a grade is in the sheet
 * @param- gradesheet, course_id
  * @return
 * 1 if the grade is there, 0 otherwise
 */
int gradeSheetDoesGradeExists(GradeSheet gradeSheet, int course_id);

/**
 * gradeSheetDestroy: De-allocates a GradeSheet.
 *
 * @return
 * GRADE_SHEET_SUCCESS - new grade sheet created
 */
void gradeSheetDestroy(GradeSheet gradeSheet);


#endif //GRADESHEET_H
