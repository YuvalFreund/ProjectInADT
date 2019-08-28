CC = gcc -std=c99
OBJS = CleanCourse.o CourseGrades.o SemesterGrades.o GradeSheet.o Student.o CourseManager.o 
OBJS_TEST = CleanCourse_test.o CourseGrades_test.o SemesterGrades_test.o GradeSheet_test.o list_example_test.o Student_test.o CourseManager_test.o
TEST_FILES = CleanCourse_test CourseGrades_test SemesterGrades_test GradeSheet_test list_example_test Student_test CourseManager_test
EXEC = mtm_cm
EXEC_TESTS = tests
CFLAGS = -Wall -pedantic-errors -Werror -DNDEBUG

#mtm_cm routine - build all .o files and link with relevant flags
$(EXEC) : $(OBJS) mtm_cm.o
		$(CC) -o $@ $(CFLAGS) mtm_cm.o  $(OBJS) -L. -lmtm
CleanCourse.o: CleanCourse.c list.h CleanCourse.h
	$(CC) $(CFLAGS) -c $*.c
CourseGrades.o: CourseGrades.c mtm_ex3.h CourseGrades.h
	$(CC) $(CFLAGS) -c $*.c
CourseManager.o:CourseManager.c list.h mtm_ex3.h GradeSheet.h CleanCourse.h SemesterGrades.h
	$(CC) $(CFLAGS) -c $*.c
GradeSheet.o: GradeSheet.c list.h mtm_ex3.h GradeSheet.h CleanCourse.h SemesterGrades.h CourseGrades.h
	$(CC) $(CFLAGS) -c $*.c
mtm_cm.o: mtm_cm.c list.h mtm_ex3.h GradeSheet.h CleanCourse.h SemesterGrades.h CourseGrades.h Student.h CourseManager.h
	$(CC) $(CFLAGS) -c $*.c
SemesterGrades.o: SemesterGrades.c list.h mtm_ex3.h SemesterGrades.h CourseGrades.h
	$(CC) $(CFLAGS) -c $*.c
Student.o: Student.c list.h mtm_ex3.h GradeSheet.h CleanCourse.h SemesterGrades.h CourseGrades.h Student.h
	$(CC) $(CFLAGS) -c $*.c

#tests routine - build all .o files and link to every test needed
tests : $(TEST_FILES)
CleanCourse_test : CleanCourse_test.o $(OBJS)
	$(CC) $(CFLAGS) CleanCourse_test.o $(OBJS) -o $@ -L. -lmtm
CleanCourse_test.o: ./tests/CleanCourse_test.c list.h CleanCourse.h test_utilities.h
	$(CC) $(CFLAGS) -c ./tests/$*.c

CourseGrades_test.o: ./tests/CourseGrades_test.c test_utilities.h CourseGrades.h CourseGrades.h Student.h CourseManager.h
	$(CC) $(CFLAGS) -c ./tests/$*.c
CourseGrades_test : CourseGrades_test.o $(OBJS)
	$(CC) $(CFLAGS) CourseGrades_test.o $(OBJS) -o $@ -L. -lmtm

SemesterGrades_test.o: ./tests/SemesterGrades_test.c test_utilities.h SemesterGrades.h list.h CourseGrades.h
	$(CC) $(CFLAGS) -c ./tests/$*.c
SemesterGrades_test : SemesterGrades_test.o $(OBJS)
	$(CC) $(CFLAGS) SemesterGrades_test.o $(OBJS) -o $@ -L. -lmtm
	
GradeSheet_test.o: ./tests/GradeSheet_test.c GradeSheet.h CleanCourse.h SemesterGrades.h list.h CourseGrades.h test_utilities.h
	$(CC) $(CFLAGS) -c ./tests/$*.c
GradeSheet_test : GradeSheet_test.o $(OBJS)
	$(CC) $(CFLAGS) GradeSheet_test.o $(OBJS) -o $@ -L. -lmtm

list_example_test.o: ./tests/list_example_test.c test_utilities.h ./list_mtm/list_mtm.h
	$(CC) $(CFLAGS) -c ./tests/$*.c
list_mtm.o: ./list_mtm/list_mtm.c ./list_mtm/list_mtm.h	
	$(CC) $(CFLAGS) -c ./list_mtm/$*.c
list_example_test : list_example_test.o list_mtm.o list_example_test.o
	$(CC) $(CFLAGS) list_example_test.o list_mtm.o -o $@ -L. -lmtm

Student_test.o: ./tests/Student_test.c Student.h list.h GradeSheet.h CleanCourse.h SemesterGrades.h CourseGrades.h test_utilities.h	
	$(CC) $(CFLAGS) -c ./tests/$*.c
Student_test : Student_test.o $(OBJS)
	$(CC) $(CFLAGS) Student_test.o $(OBJS) -o $@ -L. -lmtm

CourseManager_test.o: ./tests/CourseManager_test.c Student.h list.h GradeSheet.h CleanCourse.h SemesterGrades.h CourseGrades.h test_utilities.h CourseManager.h mtm_ex3.h
	$(CC) $(CFLAGS) -c ./tests/$*.c
CourseManager_test : CourseManager_test.o $(OBJS)
	$(CC) $(CFLAGS) CourseManager_test.o  $(OBJS) -o $@ -L. -lmtm

clear: $(OBJS) $(OBJS_TEST)
		rm -f $(OBJS) $(OBJS_TEST)