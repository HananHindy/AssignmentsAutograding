import sys, os, os.path
import subprocess
import signal
import shutil
from subprocess import Popen, PIPE
import time
import threading
from threading import Thread
from Queue import Queue, Empty
from datetime import datetime

#VERY IMPORTANT: Please read WindowsAutogradeReadme.txt first for instructions

#VERY IMPORTANT:Currenlty this script runs only with Cygwin Python(/bin/python2.7.exe), it doesn't run correctly with official windows python

#VERY IMPORTANT: If you are running this script on Windows, DON'T forget to
#                change the .bochsrc and .bochsrc-no-gui parport1 option of students folders to: 
#                parport1: enabled=1, file="student_stdout" 
#                instead of file="CON", to allow the script to read the Bochs output correctly


#Global definitions: Change accordingly for every assignment
output_file_name = "Assignment2Early"

tests = ['Q1', 'Q2', 'Q3', 'Q4', 'B1']

outputs = ["[EVAL2] WriteString: Succeeded",
           "[EVAL2] CreateIntArray: Succeeded",
           "[EVAL2] GetElementFromArray: Succeeded",
		   "[EVAL2] SortTheArray: Succeeded",
		   "[EVAL2] [BONUS] DeleteIntArray: Succeeded"]

#The main function
def main():
    
    print "Please read WindowsAutogradeReadme.txt first for instructions"
    print "Currenlty this script runs only with Cygwin Python (/bin/python2.7.exe), it doesn''t run correctly with official windows Python "
    print "MAKE SURE that students'' .bochsrc and .bochsrc-no-gui file in question_template folder has parport1 option = parport1: enabled=1, file=student_stdout, instead of CON "
    if check_copy_error_from_windows() == True:
        return

    print "Started at: " + datetime.now().strftime("%Y-%m-%d %I:%M:%S %p")
    global output_file_name
    output_file_name += "_" + datetime.now().strftime("%Y_%m_%d_%I_%M_%S")
    output_file_name += ".csv"

    print "=============="
    print "Instructions:"
    print "     Output file: " + output_file_name
    print "     Values: 1 success, 0 fail, -2 compilation error ,-3 restarting, -4 WS needs to be larger, -5 initial WS problem, -6 timeout,  -100 undefined student mistake (student commented the test call, or any other mistake)"
    print "     If a bochs-no-gui process remains open and blocks Autograde execution, kindly close from the Task Manager it and Autograde will continue"
    print "=============="

    dirs_finished = 0
    compile_errors= 0           #-2 value in the output file is a compilation error
    file_exists = os.path.isfile(output_file_name)
    output_file = open(output_file_name, 'a+')
    if(file_exists == False):
        output_file.writelines("Student ID")
        questions_names = get_questions_names()
        map(lambda x: output_file.write("," + x), questions_names) #write the header(columns names)

    # traverse root directory, and list directories as dirs and files as files
    for _, dirs, _ in os.walk("./Students"): #root, dirs, files
        dirs_count = len(dirs)
        dirs.sort() #WARNING: MANDATORY
        #print dirs
        print "Total Number Of Students = ", dirs_count

        #go inside each folder and execute the grading system
        for directory_name in dirs:
            if "Done_" in directory_name:
                dirs_finished+=1
                continue
            
            testCaseCount = 0
            student_ID = directory_name
            is_new_student = 1
            for test in tests:
                noOfTests = len(tests)
                copy_test_code(directory_name, testCaseCount)
                
                #print the progress to console
                progress = float(dirs_finished) / float(dirs_count)
                sys.stdout.write("\r")
                sys.stdout.write("Current Student: %s [Tests finished so far: %0.2f%%] Students Finished so far: %0.2f%%" %(student_ID, testCaseCount * 100.0/noOfTests, dirs_finished * 100.0/dirs_count))
                sys.stdout.flush()

                grade = gradeStudent(directory_name, testCaseCount)

                if grade == -2:
                   compile_errors = compile_errors + 1
            
                #output to file
                if is_new_student:
                    output_file.writelines("\n") #new line
                    output_file.write(student_ID)
                    is_new_student = 0
                    #print "" #new line
                    #print current_student_ID,
    
                output_file.write(", " + str(grade))
                    #print ",", grade
    
                output_file.flush()
                testCaseCount +=1
            oldName = "./Students/" + directory_name
            newName = "./Students/Done_" + directory_name
            os.rename(oldName,newName)
            dirs_finished = dirs_finished +1
            testCaseCount = 0
        break

    output_file.close()

    #print the final progress to console
    #progress = float(dirs_finished) / float(dirs_count)
    #sys.stdout.write("\r")
    #sys.stdout.write("Current Group: %s [Groups finished so far: %0.2f%% (%d Compilation Error(s)]" %(group_ID, progress * 100.0, compile_errors))
    #sys.stdout.flush()
    
    print ""
    print "Autograde Finished"

#=========================================

def gradeStudent(student_dir, question_ID):


    #student_question_ID = student_ID + question_ID
    os.chdir("./Students/"+student_dir)
    
    #-----------------------------------------------------
    # Edit the .bochsrc-no-gui, replace the partport1 line to this question timestamped output file
    # We do this to make sure that Autograde doesn't mistakenly take the same file for the next question
    #-----------------------------------------------------
    #   fraction of a second time stamp
    QTimestamp = str(time.time()).replace(".","")
    #   This question specific student_stdout file name
    student_stdout_default = "student_stdout"
    student_stdout_Q_specific = student_stdout_default+"_"+QTimestamp

    bochsrcDefault = open(os.getcwd()+"/.bochsrc-no-gui", 'r')
    bochsrcAutograde = open(os.getcwd()+"/.bochsrc_autograde_"+QTimestamp, 'w')
    for line in bochsrcDefault:
        bochsrcAutograde.write(line.replace(student_stdout_default, student_stdout_Q_specific))
    bochsrcDefault.close()
    bochsrcAutograde.close()
    #-----------------------------------------------------

    # TODO: Improvment: If a compilation error occurs to a student folder, let the script save the output 
    #                   of "make" into a disk file (just like it does for Bochs output)

    # According to the number of cores in your machine, you can change the -j number to compile faster
    # example: I use -j4 on my quad core machine
    makeProc = subprocess.Popen("make -j2", shell=True, stdout=PIPE, stderr=PIPE)
    #makeProc.wait()
    makeProc.communicate() #wait() caused the deadlock with stdout, err = PIPE, online documentation warns against that, and suggested the use of communicate()

    question_grade = -100
    if (os.path.isfile(os.getcwd()+"/obj/kern/bochs.img") == True):
        
        myCommand = 'bochs-no-gui -q -f .bochsrc_autograde_' + QTimestamp
        #proc = Popen(myCommand, shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE, preexec_fn=os.setsid)
        #si = subprocess.STARTUPINFO()
        #si.dwFlags |= subprocess.STARTF_USESTDHANDLES
        #proc = Popen(myCommand, shell=True, startupinfo=si, stdout=PIPE, stderr=STDOUT) #shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)

        proc = Popen(myCommand, shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
        
        #===== WARNING: Unix only work-around: non-blocking streams
        # default read() and readline() in Python block until a
        # valid output is available.
        # This makes it difficult to stop the Bochs process
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
        question_grade = 0

        #A closure (inner function) that I will use later to terminate any stucking threads or processes
        # and finialize remainig resources
        def finializeAndCleanUp():
            try:
                if(proc.poll() is None):    #kill the Bochs process if not already killed
                    proc.kill()                 
                    #os.kill(proc.pid, signal.SIGKILL)
                thefile.close()
            except Exception, msg:
                print msg

        try:
            while os.path.isfile(os.getcwd() + "/" + student_stdout_Q_specific) == False:
                #keep waiting till we find the new student_stdout file generated by Bochs
                time.sleep(0.001)
                
            #TODO: Consider removing the try catch statements as a running time optimization
            thefile = open(os.getcwd() + "/"+student_stdout_Q_specific,"r") #+ "/Students"+"/"+student_dir+
            while True:
                try:     
                    #TODO: Revise this part to enable time out for project correction
                      
                    #def signal_handler(signum, frame):
                    #    raise Exception("Timed out!")
                    #signal.signal(signal.SIGALRM, signal_handler)
                    #signal.alarm(60*1)   # 1 Min 
                    #try:
                    #    chunk = thefile.read(1)
                    #except Exception, msg:
                    #    question_grade = -6
                    #    finializeAndCleanUp()
                    #    print msg
                    #    break

                    #time.sleep(0.00005)  #this slowed down the loop consideralby, taking about 5 seconds
                    chunk = thefile.read(1)
                    if not chunk:
                        continue
                    #else:
                    #    sys.stdout.write(chunk)
                    #    sys.stdout.flush()
                        
                    stream_data += chunk 
                    
                    if "InitialWSError1" in stream_data:
                        question_grade = -4
                        finializeAndCleanUp()
                        break
                    elif "InitialWSError2" in stream_data:
                        question_grade = -5
                        finializeAndCleanUp()
                        break
                    elif stream_data.count('!! FCIS says HELLO !!') > 1:
                        question_grade = -3         #Restart Case
                        finializeAndCleanUp()
                        break
                    elif outputs[question_ID] in stream_data:
                        question_grade = 1      # Questions other than 1         
                            
                        finializeAndCleanUp()
                        break
                    elif "FOS>" in stream_data:
                        finializeAndCleanUp()
                        break
               
                    elif proc.poll() is not None: #if process is finished, break the loop
                        finializeAndCleanUp()
                        break
                    #elif chunk is None:
                        #break
                    #elif chunk == "":
                        #break
                               
                except IOError:
                    if proc.poll() is not None: 
                        #print "exiting loop, io error"
                        finializeAndCleanUp()
                        break
                    
                except Exception, msg:
                    #print msg
                    if proc.poll() is not None:
                        #print "exiting loop"
                        finializeAndCleanUp()
                        break
        
        except IOError, msg:
                #print "exiting, io error opening student_stdout_..."
                print msg
                finializeAndCleanUp()
        
        except Exception, msg:
                #print "exiting"
                finializeAndCleanUp()
                print msg


        
		#print stream_data
        
        # Additional check, unnecessary since we returned to use the blocking
        # stream read, but just in case that the TA closes the Bochs window
        # by mistake very fast, or in the student readline() cases
        if len(stream_data) < 210:
            print "WARNING: stream output very short ("+str(len(stream_data))+" chars). Very fast closing of Bochs window might have happened"


    else:
        #print "Compilation error !!!"
        question_grade = -2
        
    #delete the obj folder (optional)
    shutil.rmtree('./obj', ignore_errors=True)

#    print os.getcwd()
    os.chdir("../../")
#    print os.getcwd()

    return question_grade


#=========================================================================
def get_questions_names():
    return tests

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

def copy_test_code(student_dir,question_ID):
    root_dst_dir = './Students/'+student_dir
    root_src_dir = './Tests/' + tests[question_ID]
    for src_dir, dirs, files in os.walk(root_src_dir):
        dst_dir = src_dir.replace(root_src_dir, root_dst_dir)
        if not os.path.exists(dst_dir):
            os.mkdir(dst_dir)
        for file_ in files:
            src_file = os.path.join(src_dir, file_)
            dst_file = os.path.join(dst_dir, file_)
            if os.path.exists(dst_file):
                os.remove(dst_file)
            shutil.copy(src_file, dst_dir)

    
if __name__=="__main__":
   main()

