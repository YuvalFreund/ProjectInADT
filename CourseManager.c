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
#include "Student.h"
#include "CourseManager.h"
#define  MTM_SUCCESS -1

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

typedef struct Request_t{
    int id_asking;
    int id_asked;
}*Request;

/** Type for defining the student */
struct CourseManager_t {
    List students;
    int connectedStudent;
    List friendshipRequest;
};

static void RequestFreeForList(Request request){
    assert(request!=NULL);
    free(request);
}
static void freeRequest(ListElement element) {
    RequestFreeForList(element);
}

static int getIdAsked(Request request){
    assert(request!=NULL);
    return request->id_asked;
}
static int getIdAsking(Request request){
    assert(request!=NULL);
    return request->id_asking;
}

static Request RequestCopyForList(Request request){
    assert(request!=NULL);
    Request new = malloc(sizeof(struct Request_t));
    //new->id_asked = malloc(sizeof(int));
    new->id_asked = getIdAsked(request);
    //new->id_asking = malloc(sizeof(int));
    new->id_asking = getIdAsking(request);
    return new;
}
static ListElement RequestCopy(ListElement element) {
    if(element == NULL){
        return NULL;
    }
    Request new = RequestCopyForList(element);
    return new ? new : NULL;
}

static ListElement copyStudent(ListElement element) {
    if(element == NULL){
        return NULL;
    }
    Student new = studentCopy(element);
    return new? new : NULL;
}
static void freeStudent(ListElement element) {
    studentDestroy(element);
}

/**
 * compareStudents
 * a compare function for 2 students- first based on their grades and the id
 */

static int compareStudents(ListElement element1, ListElement element2, ListSortKey key){
    assert(element1 != NULL || element2 != NULL);
    //check grade distance for students
    int distance = studentGetHelper(element1) -
                   studentGetHelper(element2);
    distance = (distance*(*(int *)key));
    //if points are equal, check student id
    if(distance == 0) {
        distance = studentGetStudentId(element1) -
                   studentGetStudentId(element2);
    }
    return distance;
}

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
MtmErrorCode courseManagerCreate(CourseManager *courseManager){
    *courseManager = malloc(sizeof(struct CourseManager_t));
    if(*courseManager == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    (*courseManager)->connectedStudent = -1;//value for unlogged student
    (*courseManager)->friendshipRequest = listCreate(RequestCopy,freeRequest);
    if((*courseManager)->friendshipRequest == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    (*courseManager)->students = listCreate(copyStudent,freeStudent);
    if((*courseManager)->students == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;//success flag
}

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
MtmErrorCode addStudent(CourseManager courseManager, int id,
                        char* first_name, char* last_name){
    if(courseManager == NULL || id<=0 || id >= 1000000000){
        return MTM_INVALID_PARAMETERS;
    }
    LIST_FOREACH(Student,iterator,courseManager->students){
        //search for student in students list
        if(studentGetStudentId(iterator) == id){
            return MTM_STUDENT_ALREADY_EXISTS;
        }
    }
    Student new;//create new student
    studentCreate(id,first_name,last_name,&new);
    if(new == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    listInsertLast(courseManager->students,new);//insert to students list
    studentDestroy(new);
    return MTM_SUCCESS;//success
}
/**
 * removeStudent- Removes a student from the System
 *
 * @param id - student to be removed
 * @return
 * 	MTM_STUDENT_DOES_NOT_EXIST - the student is not in the system
 * 	MTM_SUCCESS - student removed
 */
MtmErrorCode removeStudent(CourseManager courseManager, int id){
    assert(courseManager!=NULL);
    bool exists = false;
    LIST_FOREACH(Student,iterator,courseManager->students){
        //search for student in students list
        if(studentGetStudentId(iterator) == id){
            exists = true;
            listRemoveCurrent(courseManager->students);//delete student from courseManager
            break;
        }
    }
    if(exists == false){
        return MTM_STUDENT_DOES_NOT_EXIST;
    }
    //if this student is connected - disconnect
    if(courseManager->connectedStudent == id){
        studentLogOut(courseManager);
    }
    //removing the student from the friends request list
    LIST_FOREACH(Request,iterator,courseManager->friendshipRequest){
        if(iterator->id_asking == id || iterator->id_asked == id){
            listRemoveCurrent(courseManager->friendshipRequest);
            //each request of the student or from the student will be deleted
        }
    }
    //remove the student from all friendship lists of every student
    LIST_FOREACH(Student,iterator,courseManager->students){
        if(studentGetStudentId(iterator))
            studentRemoveFriend(iterator,id);
    }
    return MTM_SUCCESS;//success
}

/**
 * studentLogIn- Logs a specific student into the system (one at a time)
 *
 * @param id - student to be logged in
 * @return
 * 	MTM_STUDENT_DOES_NOT_EXIST - the student is not in the system
 * 	MTM_ALREADY_LOGGED_IN - the student is already logged in
 * 	MTM_SUCCESS - student removed
 */
MtmErrorCode studentLogIn(CourseManager courseManager, int id){
    if(courseManager->connectedStudent != -1){
        return MTM_ALREADY_LOGGED_IN;
    }
    bool exists = false;
    LIST_FOREACH(Student,iterator,courseManager->students){
        if(studentGetStudentId(iterator) == id){
            exists = true;
            break;
        }
    }
    if(exists == false){
        return MTM_STUDENT_DOES_NOT_EXIST;
    }
    courseManager->connectedStudent = id;
    return MTM_SUCCESS;//success
}

/**
 * studentLogOut- Logs a specific student out of the system
 *
 * @param id - student to be logged out
 * @return
 * 	MTM_NOT_LOGGED_IN - the student is not logged in
 * 	MTM_SUCCESS - student logged out
 */
MtmErrorCode studentLogOut(CourseManager courseManager){
    if(courseManager->connectedStudent == -1){
        return MTM_NOT_LOGGED_IN;
    }
    courseManager->connectedStudent = -1;
    return MTM_SUCCESS;//success
}

/**
 * studentFriendRequest - friend request from another student
 *
 * @param other_id - student to be asked friends from
 * @return
 * 	MTM_NOT_LOGGED_IN - the student is not logged in
 * 	MTM_ALREADY_FRIEND - the students are already friends
 * 	MTM_STUDENT_DOES_NOT_EXIST - there's no such student
 * 	MTM_ALREADY_REQUESTED - there's a request waiting
 * 	MTM_SUCCESS - student logged out
 */
MtmErrorCode studentFriendRequest(CourseManager courseManager, int other_id){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    int appeared = 0;
    int asking_id=courseManager->connectedStudent;
    int asked_id = other_id;
    if (asked_id==asking_id){
        return MTM_ALREADY_FRIEND; //lonely student asks himself
    }
    LIST_FOREACH(Student, iterator, courseManager->students) {
        int temp_id = studentGetStudentId(iterator);
        if (temp_id == other_id) {
            appeared = 1;
            break;
        }
    }
    if (appeared == 0) {
        return MTM_STUDENT_DOES_NOT_EXIST;// no such student in system
    }
    LIST_FOREACH(Request, iterator, courseManager->friendshipRequest) {
        asking_id = getIdAsking(iterator);
        asked_id = getIdAsked(iterator);
        if (asking_id == courseManager->connectedStudent &&
            asked_id == other_id) {// request appeared
            return MTM_ALREADY_REQUESTED;
        }
    }
    asking_id = courseManager->connectedStudent;
    Student connected = getStudentFromId(courseManager,asking_id);
    if (studentCheckIfFriends(connected, other_id) == 1) {
        return MTM_ALREADY_FRIEND;
    }
    Request new_request = malloc(sizeof(new_request));
    new_request->id_asked = other_id;
    new_request->id_asking = courseManager->connectedStudent;
    listInsertLast(courseManager->friendshipRequest,new_request);
    freeRequest(new_request);
    return MTM_SUCCESS;
}
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
MtmErrorCode studentHandleRequest(CourseManager courseManager, int other_id, char* action) {
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    int appeared = 0, friend_added;
    LIST_FOREACH(Student, iterator, courseManager->students) {
        int temp_id = studentGetStudentId(iterator);
        if (temp_id == other_id) {
            appeared = 1;
            break;
        }
    }
    if (appeared == 0) {
        return MTM_STUDENT_DOES_NOT_EXIST;// no such student in system
    }
    Student connected = getStudentFromId(courseManager,
            courseManager->connectedStudent);
    if (studentCheckIfFriends(connected, other_id) == 1) {
        return MTM_ALREADY_FRIEND;
    }
    int asked_id = courseManager->connectedStudent;
    Student asked = getStudentFromId(courseManager, asked_id);
    Student asking = getStudentFromId(courseManager, other_id);
    int asking_id;
    LIST_FOREACH(Request, iterator, courseManager->friendshipRequest) {
        asking_id = getIdAsking(iterator);
        asked_id = getIdAsked(iterator);
        if (asked_id == courseManager->connectedStudent &&
              asking_id == other_id)  {
            if (strcmp(action, "accept")==0){
                studentAddFriend(asked, other_id);
                studentAddFriend(asking, asked_id);
                friend_added = 1;
            }
            listRemoveCurrent(courseManager->friendshipRequest);
        }
        if (asking_id == courseManager->connectedStudent &&
             asked_id == other_id) {
            listRemoveCurrent(courseManager->friendshipRequest);
        }
    }
    if (strcmp(action, "reject") != 0 && strcmp(action, "accept") != 0) {
        return MTM_INVALID_PARAMETERS;
    }
    if (friend_added == 1) {
        return MTM_SUCCESS;
    }
    return MTM_NOT_REQUESTED;
}

/**
 * studentUnFriend- Unfriends a student from the logged student friends list
 * and also the connected student from the unfriended student
 * @param other_id - id of student who is to be removed from the logged in student friends list
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_STUDENT_DOES_NOT_EXIST - student to be unfriended is not in the system
 * 	MTM_NOT_FRIEND - the students are not friends
 * 	MTM_SUCCESS - students unfriend
 */
MtmErrorCode studentUnFriend(CourseManager courseManager, int other_id){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    int appeared = 0;
    LIST_FOREACH(Student, iterator, courseManager->students) {
        int temp_id = studentGetStudentId(iterator);
        if (temp_id == other_id) {
            appeared = 1;
            break;
        }
    }
    if (appeared == 0) {
        return MTM_STUDENT_DOES_NOT_EXIST;// no such student in system
    }
    int asking_id= courseManager->connectedStudent;
    Student asking=getStudentFromId(courseManager,asking_id);
    if (studentCheckIfFriends(asking,other_id)==0){
        return MTM_NOT_FRIEND;
    }
    studentRemoveFriend(asking, other_id);
    Student asked = getStudentFromId(courseManager, other_id);
    studentRemoveFriend(asked, asking_id);
    return MTM_SUCCESS;
}

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
MtmErrorCode grade_sheet_add(CourseManager courseManager, int semester,
                             int course_id, char* points, int grade){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    if(semester <= 0 || course_id >= 1000000 || course_id <=0 || grade < 0 ||
            grade > 100 ||
            studentAddGrade(loggedin,semester,course_id,points,grade) !=
                    STUDENT_SUCCESS){
        return MTM_INVALID_PARAMETERS;
    }
    return MTM_SUCCESS;
}

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
MtmErrorCode grade_sheet_remove(CourseManager courseManager,int semester, int course_id){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    if(studentRemoveGrade(loggedin,semester,course_id) == STUDENT_COURSE_NOT_EXIST){
        return MTM_COURSE_DOES_NOT_EXIST;
    }
    return MTM_SUCCESS;
}

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
MtmErrorCode grade_sheet_update(CourseManager courseManager,int course_id,int grade){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    if(studentUpdateGrade(loggedin,course_id,grade) == STUDENT_COURSE_NOT_EXIST){
        return MTM_COURSE_DOES_NOT_EXIST;
    }
    if(grade<0 || grade > 100){
        return MTM_INVALID_PARAMETERS;
    }
    return MTM_SUCCESS;
}

/**
 * reportFull- prints a transcript
 *
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_SUCCESS - printed
 */
MtmErrorCode reportFull(CourseManager courseManager,FILE* file){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    studentPrintGradesFull(loggedin,file);
    return MTM_SUCCESS;
}


/**
 * reportClean- prints a clean transcript
 *
 * @return
 * 	MTM_NOT_LOGGED_IN - there's no student logged to the system
 * 	MTM_SUCCESS - grades was added
 */
MtmErrorCode reportClean(CourseManager courseManager,FILE* file){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    studentPrintClean(loggedin,file);
    return MTM_SUCCESS;
}

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
MtmErrorCode reportBest(CourseManager courseManager,FILE* file, int amount){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    if (amount<=0){
        return MTM_INVALID_PARAMETERS;
    }

    if(amount<=0){
        return MTM_INVALID_PARAMETERS;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    studentPrintBest(loggedin,file,amount);
    return MTM_SUCCESS;
}
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
MtmErrorCode reportWorst(CourseManager courseManager,FILE* file, int amount){
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    if (amount<=0) {
        return MTM_INVALID_PARAMETERS;
    }
    if(amount<=0){
        return MTM_INVALID_PARAMETERS;
    }
    Student loggedin = getStudentFromId(courseManager,courseManager->connectedStudent);
    studentPrintWorst(loggedin,file,amount);
    return MTM_SUCCESS;
}

MtmErrorCode reportReference(CourseManager courseManager,FILE* file, int course_id, int amount) {
    if (courseManager->connectedStudent == -1) {
        return MTM_NOT_LOGGED_IN;
    }
    if (amount <= 0 || course_id < 0 || course_id > 1000000) {
        return MTM_INVALID_PARAMETERS;
    }
    Student logged = getStudentFromId(courseManager, courseManager->connectedStudent);
    List reference = listCreate(copyStudent, freeStudent);
    List friends = studentReturnFriends(logged);
    Student temp;
    LIST_FOREACH(int*, iter, friends) {
        temp = getStudentFromId(courseManager, *iter);
        int tempgrade = studentGetBestGrade(temp, course_id);
        if (tempgrade >= 0) {
            studentInsertHelper(temp, tempgrade);
            listInsertLast(reference, temp);
        }
    }
    int *key = malloc(sizeof(int));
    *key = 1;
    listSort(reference, compareStudents, key);
    int i = 1;
    LIST_FOREACH(Student, iter2, reference) {
        if (i <= amount) {
            studentPrintStudentName(iter2, file);
        } else {
            break;
        }
        i++;
    }
    listDestroy(reference);
    free(key);
    return MTM_SUCCESS;
}

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

MtmErrorCode reportFacultyRequest(CourseManager courseManager,FILE* file, int course_id, char* request){
    if(courseManager->connectedStudent == -1){
        return MTM_NOT_LOGGED_IN;
    }
    if(strcmp(request,"remove_course") == 0){
        LIST_FOREACH(Student,iterator,courseManager->students){
            if(studentGetStudentId(iterator) == courseManager->connectedStudent){
                //we found the connected student in the list, now we'll check if the course was taken
                if(studentDoesGradeExists(iterator,course_id) == 0){
                    return MTM_COURSE_DOES_NOT_EXIST;
                }
            }
        }
    }
    if(strcmp(request,"cancel_course") != 0 && strcmp(request,"remove_course") != 0
       && strcmp(request,"register_course") != 0){
        return MTM_INVALID_PARAMETERS;
    }
    mtmFacultyResponse(file,"your request was rejected");
    return MTM_SUCCESS;
}
/**
 * getStudentFromId- returns the pointer to the student needed
 *
 * @param id - student to searched for
 * @return
 * 	NULL - if the student is not logged in
 *  pointer  to student else
 */
Student getStudentFromId(CourseManager courseManager, int student_id) {
    if (student_id < 0 || student_id > 1000000000) {
        return NULL;
    }
    LIST_FOREACH(Student, iterator, courseManager->students) {
        if (studentGetStudentId(iterator) == student_id){
            return iterator;
        }
    }
    return NULL; //shouldn't get here
}

/**
* courseManagerDestroy- De-allocates courseManager
*
* @param courseManager to de-allocate
* @return
*
*/
void courseManagerDestroy(CourseManager courseManager){
    listDestroy(courseManager->friendshipRequest);
    listDestroy(courseManager->students);
    free(courseManager);
}
/**
* printing functions for tests
*/
int getLoggedInStudentId(CourseManager courseManager){
    return courseManager->connectedStudent;
}
void printFriendshipList(CourseManager courseManager){
    LIST_FOREACH(Request,iterator,courseManager->friendshipRequest){
        printf("\nrequest from %d to %d",iterator->id_asking,iterator->id_asked);
    }
}

