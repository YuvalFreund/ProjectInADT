
#ifndef STUDENT_H
#define STUDENT_H
#include "list.h"
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
typedef struct Student_t *Student;

/** Type used for returning error codes from student functions */
typedef enum StudentResult_t {
    STUDENT_OUT_OF_MEMORY,
    STUDENT_INVALID_PARAMETERS,
    STUDENT_ALREADY_FRIENDS,
    STUDENT_NOT_FRIEND,
    STUDENT_COURSE_NOT_EXIST,
    STUDENT_SUCCESS
} StudentResult;

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
StudentResult studentCreate(int id, char* first_name, char* last_name, Student *student);

/**
 * studentPrintcheck: prints all the ID of the student's friends
 * @param Student
 * @return
*/
void studentPrintCheck(Student student);


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
StudentResult studentAddFriend(Student student, int id);

/**
 * studentRemoveFriend: Removes a friend from the students friend list
 *
 * @param id - id of student added
 * @return
 * STUDENT_SUCCESS - student removed
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 * STUDENT_NOT_FRIEND - the student isn't a friend in the list
 */
StudentResult studentRemoveFriend(Student student, int id);

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
StudentResult studentAddGrade(Student student, int semester, int course_id, char* points, int grade);

/**
 * studentRemoveGrade: removes a grade from the students transcript (from the specific semester
 * and update the latest semester in course list)
 * @param semester
 * @param course_id
 * @return
 * STUDENT_SUCCESS - grade removed
 * STUDENT_COURSE_NOT_EXIST - the student didn't take the course
 */
StudentResult studentRemoveGrade(Student student, int semester, int course_id);

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
StudentResult studentUpdateGrade(Student student, int course_id, int grade);

/**
 * studentPrintGrades: prints the student full transcript
 * @return
 */
void studentPrintGradesFull(Student student,FILE* file);

/**
 * studentPrintClean: prints the student clean transcript
 * @return
 */
void studentPrintClean(Student student,FILE* file);

/**
 * studentPrintBest: prints the best grades (that counts) for the student
 * @param amount
 * @return
 * STUDENT_SUCCESS - grade printed
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 */
StudentResult studentPrintBest(Student student,FILE* file, int amount);

/**
 * studentPrintWorst: prints the worst grades (that counts) for the student
 * @param amount
 * @return
 * STUDENT_SUCCESS - grades printed
 * STUDENT_INVALID_PARAMETERS - parameters aren't valid
 */
StudentResult studentPrintWorst(Student student,FILE* file, int amount);
/**
 * studentGetStudentId: returns student's id
 * @param student
 * @return
 * -1 if student is NULL
 * student's id- else
 */
int studentGetStudentId (Student student);
/**
 * studentCheckIfFriends: check if a student appears
 * in the connected student friends list
 * @return
 * -1 if student is NULL
 * 1 if they are friends
 * 0 else
 */
int studentCheckIfFriends(Student student, int friend_id);

/**
* studentCopy: copies a student.
*
* @return
* NULL - if student to be copied is NULL
* student otherwise
*/

Student studentCopy (Student student);

/**
 * studentGetHelper- takes the helper from the student
 *
 * @return
 */
int studentGetHelper(Student student);

/**
 * studenInsertHelper- insersts  helper from the student
 *
 * @return
 */
void studentInsertHelper(Student student, int helper1);

/**
 * studentGetBestGrade- takes the best grade of student in a course
 * @param- student, course_id
 * @return
 */
int studentGetBestGrade(Student student, int course_id);

/**
 * studentReturnFriends- returns the friends list of student
 * @param- student
 * @return list
 */
List studentReturnFriends(Student student);

/**
 * studentPrintStudentName: prints the student name only
 * @return
 */
void studentPrintStudentName(Student student,FILE* file);

/**
 * studentDoesGradeExists - checks if a grade is in the student's sheet
 * @param- gradesheet, course_id
  * @return
 * 1 if the grade is there, 0 otherwise
 */
int studentDoesGradeExists(Student student, int course_id);

/**
 * studentDestroy: De-allocates a student.
 *
 * @return
 * STUDENT_SUCCESS - new student created
 */
void studentDestroy(Student student);


#endif //STUDENT_H
