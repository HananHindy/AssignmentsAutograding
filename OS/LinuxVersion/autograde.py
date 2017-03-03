import sys, os, os.path
import subprocess
import signal
from subprocess import Popen, PIPE
from threading import Thread
from Queue import Queue, Empty
from datetime import datetime


#VERY IMPORTANT: MUST BE RUN USING python -u ./autograde.py (unbuffered)
#                in order to force the "FOS>" to be printed immediately
#                without the need for pressing enter (newline)


#Global definitions: Change accordingly for every assignment
output_file_name = "ass4_2015"
number_of_questions = 5

stud_ID_width_in_directory_name = 10 #characters


#The main function
def main():

    print "MAKE SURE TO RUN THIS SCRIPT USING python -u ./autograde.py (UNBUFFERED)"
    if check_copy_error_from_windows() == True:
        return

    print "Started at: " + datetime.now().strftime("%Y-%m-%d %I:%M:%S %p")
    global output_file_name
    output_file_name += "_" + datetime.now().strftime("%Y_%m_%d__%H_%M_%S")
    output_file_name += ".csv"

    print "=============="
    print "Instructions:"
    print "     Output file: " + output_file_name
    print "     Values: 1 success, 0 fail, -2 compilation error, -100 undefined student mistake (student commented the test call, or any other mistake)"
    print "     If a bochs window remains open and blocks autograde execution, kindly close it and autograde will continue"
    print "=============="

    current_student_ID = ""
    dirs_finished = 0
    compile_errors= 0 #-2 value in the output file is a compilation error

    output_file = open(output_file_name, 'w')
    output_file.writelines("Student ID")
    questions_names = get_questions_names()
    map(lambda x: output_file.write("," + x), questions_names) #write the header(columns names)

    
    # traverse root directory, and list directories as dirs and files as files
    for _, dirs, _ in os.walk("."): #root, dirs, files
        dirs_count = len(dirs)
        dirs.sort() #WARNING: MANDATORY
        #print dirs
        print "Total Folders = ", dirs_count

        #go inside each folder and execute the grading system
        for directory_name in dirs:
            student_ID = directory_name[:stud_ID_width_in_directory_name]
            is_new_student = student_ID <> current_student_ID
            if is_new_student:
                current_student_ID = student_ID

            #print the progress to console
            progress = float(dirs_finished) / float(dirs_count)
            sys.stdout.write("\r")
            sys.stdout.write("Current Student: %s [Folders finished so far: %0.2f%% (%d Compilation Error(s)]" %(current_student_ID, progress * 100.0, compile_errors / number_of_questions))
            sys.stdout.flush()

            grade = gradeStudent(student_ID, directory_name[stud_ID_width_in_directory_name:])

            if grade == -2.0:
                compile_errors = compile_errors + 1
            
            #output to file
            if is_new_student:
                output_file.writelines("\n") #new line
                output_file.write(current_student_ID)
                #print "" #new line
                #print current_student_ID,

            output_file.write(", " + str(grade))
                #print ",", grade

            output_file.flush()
            dirs_finished = dirs_finished +1

        break

    output_file.close()

    #print the final progress to console
    progress = float(dirs_finished) / float(dirs_count)
    sys.stdout.write("\r")
    sys.stdout.write("Current Student: %s [Folders finished so far: %0.2f%% (%d Compilation Error(s)]" %(current_student_ID, progress * 100.0, compile_errors / number_of_questions))
    sys.stdout.flush()
    
    print ""
    print "Autograde Finished"

#=========================================

def gradeStudent(student_ID, question_ID):

    student_question_ID = student_ID + question_ID
    os.chdir("./"+student_question_ID)
    
    makeProc = subprocess.Popen("make", shell=True, stdout=PIPE, stderr=PIPE)
#    makeProc.wait()
    makeProc.communicate() #wait() caused the deadlock with stdout, err = PIPE, online documentation warns against that, and suggested the use of communicate()


    question_grade = -100.0
    if (os.path.isfile(os.getcwd()+"/obj/kern/bochs.img") == True):
              
        myCommand = "/usr/local/bin/bochs -q"
        proc = Popen(myCommand, shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)

        #===== WARNING: Unix only work-around: non-blocking streams
        # default read() and readline() in Python block until a
        # valid output is available.
        # This makes it difficult to stop the bochs process
        # when we reach the FOS>, because we will not know when to check
        # as long as read() is working and doesn't return
        #
        # The workaround using fcntl non-blocking flag is Unix only solution
        # we should find an equivalent library to other platforms, in case
        # we migrate the course to windows or Mac
        
        #import fcntl
        #file_flags = fcntl.fcntl(proc.stdout.fileno(), fcntl.F_GETFL)
        #fcntl.fcntl(proc.stdout.fileno(), fcntl.F_SETFL, file_flags | os.O_NONBLOCK)

        # UPDATE: We didn't use this non-blocking work-around, and we returned
        #         to using the default blocking read(1), but we made sure
        #         to always check for the FOS> after every character read
        #         from the stream. if for some unusual reason the FOS>
        #         prompt isn't printed, we will have to manually close the
        #         bochs window to allow the script to continue working.
        #         - JUST MAKE SURE to run the script with python - u (unbuffered)
        #=============
        stream_data = ""
        while True:            
            try:       #TODO: Consider removing the try catch statement as a running time optimization
                chunk = proc.stdout.read(1)
                stream_data += chunk
                if "[EV@LU@TI0N] Finished" in stream_data:
                    os.kill(proc.pid, signal.SIGKILL)
                    break
                elif "[EV@LU@TI0N] 1" in stream_data:
                    question_grade = 1.0
                    
                elif "[EV@LU@TI0N] 0.75" in stream_data:
                    question_grade = 0.75               
                    
                elif "[EV@LU@TI0N] 0.5" in stream_data:
                    question_grade = 0.5               
                    
                elif "[EV@LU@TI0N] 0.25" in stream_data:
                    question_grade = 0.25               
                    
                elif "[EV@LU@TI0N] 0" in stream_data:
                    question_grade = 0.0
                    
                elif "FOS>" in stream_data:
                    #Don't change the grade, leave the value alone
                    #we only need to close the bochs window here
                    os.kill(proc.pid, signal.SIGKILL)
                    break
                elif proc.poll() is not None: #if process is finished, break the loop
                    break
                #elif chunk is None:
                    #break
                #elif chunk == "":
                    #break
                    
            except IOError:
                if proc.poll() is not None: 
                    #print "exiting loop, io error"
                    break
            except:
                if proc.poll() is not None:
                    #print "exiting loop"
                    break


##        stream_data = ""
##        while proc.poll() is None: #while working
##            line = proc.stdout.readline()
##            stream_data += line
##            if "[EV@LU@TI0N] Failed" in line:
##                question_grade = 0               
##                #proc.terminate() #available only after python 2.6
##                os.kill(proc.pid, signal.SIGKILL)
##                break
##            elif "[EV@LU@TI0N] Succeeded" in line:
##                question_grade = 1               
##                os.kill(proc.pid, signal.SIGKILL)
##                break
##            elif "FOS>" in line:
##                #dont change the grade, leave the value alone
##                #so we only need to close the bochs window here
##                os.kill(proc.pid, signal.SIGKILL)
##                break
##            #else:
##                #print line,
##
        
		#print stream_data
		
        # Additional check, unnecessary since we returned to use the blocking
        # stream read, but just in case that the TA closes the bochs window
        # by mistake very fast, or in the student readline() cases
        if len(stream_data) < 210:
            print "WARNING: stream output very short ("+str(len(stream_data))+" chars). Very fast closing of bochs window might have happened"


    else:
        #print "Compilation error !!!"
        question_grade = -2.0
        
    #delete the obj folder (optional)
    import shutil
    shutil.rmtree('./obj', ignore_errors=True)

#    print os.getcwd()
    os.chdir("..")
#    print os.getcwd()

    return question_grade


#=========================================================================

def get_questions_names():

    folder_count = 0
    questions_names = []
    
    for _, dirs, _ in os.walk("."): #root, dirs, files
        dirs.sort() #WARNING: MANDATORY
        for i in range(0, number_of_questions):
            try:
                q = dirs[i]
                q = q[stud_ID_width_in_directory_name:]
                questions_names.append(q)
            except:
                print "ERROR: Not enough question folders for each student, Or unexpected error !"
        
        break

    return questions_names


def check_copy_error_from_windows():
    error_exist = False
    for root, _, files in os.walk("."): #root, dirs, files
        for file in files:
            _, extension = os.path.splitext(file)
            if extension == ".s":
                print "Copy error in file extension:"
                path = root.split('/')
                print os.path.dirname(root)       
                print (len(path) - 1) *'---' , os.path.basename(root)       
                print len(path)*'---', file
                error_exist = True

    return error_exist

if __name__=="__main__":
   main()
