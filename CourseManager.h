
#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <stdbool.h>
#include "mtm_ex3.h"
/**
 * Generic CourseManager System
 *
 * Course Manager System is a social network containing a group of students.
 * The system holds a friendship network between the students that are listed to the system.
 * The system allows to manage students grades and requests.
 *
 * The following functions are available:
 *   addStudent             - Add a student to the System
 *   removeStudent          - Removes a student from the System
 *   studentLogIn           - Logs a specific student into the system (one at a time)
 *   studentLogOut          - Logs a student out of the System
 *   studentFriendRequest   - Adds a request to the request list
 *   studentHandleRequest   - Handles a request waiting for answer by the logged student
 *   studentUnFriend        - Unfriends a student from the logged student friends list
 *   grade_sheet_add        - Adds a grade in a specific subject to his sheet
 *   grade_sheet_remove     - Removes a grade in a specific subject from his sheet
 *   grade_sheet_update     - Updates a grade in a specific subject
 *   reportFull             - Prints a full sheet of the logged student grades
 *   reportClean            - Prints a clean sheet of the logged student grades
 *   reportBest             - Prints the best grades of the logged student
 *   reportWorst            - Prints the worst grades of the logged student
 *   reportReference        - Prints all the students who have done the course and received a grade
 *   reportFacultyRequest   - Prints an answer to a faculty request
 *
 *   Errors - all the functions will return error codes as fragged in ex3.h or -1 if success
 *   MTM_SUCCESS = -1!!!
 */

/** Type for defining the CourseManager */
typedef struct CourseManager_t *CourseManager;

/**
 * CourseManagerCreate: Allocates a new empty student.
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
MtmErrorCode courseManagerCreate(CourseManager *courseManager);

/**
 * addStudent: Add a student to the System.
 *
 * @param id - Id of the student, as an int smaller than billion
 * @param first_name - first name of student, as a string
 * @param last_name - sur name of student, as a string
 * @return
 * 	MTM_SUCCESS - student added
 * 	MTM_OUT_OF_MEMORY - if a memory problem occurred
 * 	MTM_INVALID_PARAMETERS - id illegal
 * 	MTM_STUDENT_ALREADY_EXISTS - if student exists already
 */
MtmErrorCode addStudent(CourseManager courseManager, int id, char* first_name, char* last_name);

/**
 * removeStudent- Removes a student from the System
 *
 * @param id - student to be removed
 * @return
 * 	MTM_STUDENT_DOES_NOT_EXIST - the student is not in the system
 * 	MTM_SUCCESS - student removed
 */
MtmErrorCode removeStudent(CourseManager courseManager, int id);

/**
 * studentLogIn- Logs a specific student into the system (one at a time)
 *
 * @param id - student to be logged in
 * @return
 * 	MTM_STUDENT_DOES_NOT_EXIST - the student is not in the system
 * 	MTM_ALREADY_LOGGED_IN - the student is already logged in
 * 	MTM_SUCCESS - student removed
 */
MtmErrorCode studentLogIn(CourseManager courseManager, int id);

/**
 * studentLogOut- Logs a specific student into the system (one at a time)
 *
 * @param id - student to be logged out
 * @return
 * 	MTM_NOT_LOGGED_IN - the student is not logged in
 * 	MTM_SUCCESS - student logged out
 */
MtmErrorCode studentLogOut(CourseManager courseManager);

/**
 * studentFriendRequest- Adds a friend request to set of requests
 *
 * @param other_id - student to be asked friends with
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_STUDENT_DOES_NOT_EXIST - student requested to be friends with is not in the system
 * 	MTM_ALREADY_FRIEND - the students are already friends
 * 	MTM_ALREADY_REQUESTED - the request was already made
 * 	MTM_SUCCESS - student placed request
 */
MtmErrorCode studentFriendRequest(CourseManager courseManager, int other_id);

/**
 * studentHandleRequest- Handles friendship request for logged in student
 *
 * @param other_id - id of student who requested a friendship from the logged user
 * @param action - Accept/Reject request
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_STUDENT_DOES_NOT_EXIST - student requested to be friends with is not in the system
 * 	MTM_ALREADY_FRIEND - the students are already friends
 * 	MTM_NOT_REQUESTED - the request has not been not made
 * 	MTM_INVALID_PARAMETERS - the action is illegal
 * 	MTM_SUCCESS - Request handled
 */
MtmErrorCode studentHandleRequest(CourseManager courseManager, int other_id, char* action);

/**
 * studentUnFriend- Unfriends a student from the logged student friends list
 *
 * @param other_id - id of student who is to be removed from the logged in student friends list
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_STUDENT_DOES_NOT_EXIST - student requested to be friends with is not in the system
 * 	MTM_NOT_FRIEND - the students are not friends
 * 	MTM_SUCCESS - students unfriend
 */
MtmErrorCode studentUnFriend(CourseManager courseManager, int other_id);

/**
 * grade_sheet_add- adds a grade to the sheet of the logged in student
 *
 * @param semester - semester to be added to
 * @param course_id - the courses id
 * @param points - courses points
 * @param grade - courses grade

 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_INVALID_PARAMETERS - one of the parameters are not valid
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode grade_sheet_add(CourseManager courseManager, int semester, int course_id, char* points, int grade);

/**
 * grade_sheet_remove- remove a grade from the sheet of the logged in student
 *
 * @param semester - semester to be added to
 * @param course_id - the courses id


 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_COURSE_DOES_NOT_EXIST - the course does not exist in the system
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode grade_sheet_remove(CourseManager courseManager, int semester, int course_id);

/**
 * grade_sheet_update- update a grade from the sheet of the logged in student
 *
 * @param course_id - the courses id
 * @param new_grade - grade to be updated

 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_COURSE_DOES_NOT_EXIST - the course does not exist in the system
 * 	MTM_INVALID_PARAMETERS - the parameters arent valid
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode grade_sheet_update(CourseManager courseManager,int course_id,int grade);

/**
 * reportFull- prints a transcript
 *

 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_SUCCESS - printed
 */
MtmErrorCode reportFull(CourseManager courseManager,FILE* file);

/**
 * reportClean- prints a clean transcript
 *

 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode reportClean(CourseManager courseManager,FILE* file);

/**
 * reportBest- prints the best grades in the transcript
 *
 * @param amount - how many grades to print
 *
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_INVALID_PARAMETERS - amount isn't positive
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode reportBest(CourseManager courseManager,FILE* file, int amount);

/**
 * reportWorst- prints the worst grades in the transcript
 *
 * @param amount - how many grades to print
 *
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_INVALID_PARAMETERS - parameter arent valid
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode reportWorst(CourseManager courseManager, FILE* file, int amount);

/**
 * reportReference- prints the id of students that took the course from his friends list
 * prints in order of grades, and if the same grades by id
 * @param course_id - how many grades to print
 * @param amount - how many friends to print
 *
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_INVALID_PARAMETERS - parameter aren't valid
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode reportReference(CourseManager courseManager,FILE* file, int course_id, int amount);

/**
 * reportFacultyRequest- Prints an answer to a faculty request
 * @param course_id - to which course the request is relevant
 * @param request - Remove, Cancel, Register
 *
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_COURSE_DOES_NOT_EXIST - the course is not in the student's transcript
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode reportFacultyRequest(CourseManager courseManager, FILE* file, int course_id, char* request);
/**
* getStudentFromId- returns the pointer to the student needed
*
* @param id - student to searched for
* @return
* 	NULL - if the student is not logged in
*  pointer  to student else
*/
Student getStudentFromId(CourseManager courseManager, int student_id);

/**
* courseManagerDestroy- De-allocates courseManager
*
* @param courseManager to de-allocate
* @return
*
*/
void courseManagerDestroy(CourseManager courseManager);

int getLoggedInStudentId(CourseManager courseManager);
void printFriendshipList(CourseManager courseManager);

#endif /* COURSEMANAGER_H */
