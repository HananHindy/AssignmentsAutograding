import sys, os, os.path
import subprocess
import signal
import shutil
from subprocess import Popen, PIPE
from threading import Thread
from Queue import Queue, Empty
from datetime import datetime


#VERY IMPORTANT: MUST BE RUN USING python -u ./autograde.py (unbuffered)
#                in order to force the "FOS>" to be printed immediately
#                without the need for pressing enter (newline)

#VERY IMPORTANT: If you are running this script on Windows, DON'T forget to
#                change the .bochsrc and .bochsrc-no-gui parport1 option of students folders to: 
#                parport1: enabled=1, file="student_stdout" 
#                instead of file="CON", to allow the script to read the Bochs output correctly

#os.environ["PATH"] = os.pathsep.join(['d:\\fos_cygwin\\bin','d:\\fos_cygwin\\opt\\cross\\bin']) + os.pathsep + os.environ["PATH"]

#Global definitions: Change accordingly for every assignment
output_file_name = "Project_2016_Scenarios"

tests = ['tstkmalloc', 
         #'tstkfree',
         #'tstkvirtaddr', 
         #'tstkphyaddr', 
         #'tst3func',
         #'tm1', 
         #'tm2', 
         #'tm3', 
         #'tf1', 
         #'tf2', 
         #'tnf',
         #'tbf1', 
         #'tbf2', 
         #'tpp', 
         #'tia',
         #'tpr1_LRU', 
         #'tpr1_Clock', 
         #'tpr2', 
         #'tpr3', 
         #'tpr4_LRU', 
         #'tpr4_Clock',
         'envFreeS1_Clock', 
         'envFreeS1_LRU', 
         #'envFreeS2_Clock', 
         #'envFreeS2_LRU', 
         'scenario1_Clock', 
         'scenario1_LRU',
         'scenario2_Clock', 
         'scenario2_LRU',
         'scenario3_BF',
         'scenario3_NF']
         #'scenario4']
         #'BONUS_FIFO1', 
         #"BONUS_ModClk", 
         #"BONUS_FF1", 
         #"BONUS_FF2",
         #"BONUS_WF", 
         #"BONUS_ReAlloc1", 
         #"BONUS_ReAlloc2"]

outputs = ["[AUTO_GR@DING]Congratulations!! test kmalloc completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test kfree completed successfully.",
		   #"[AUTO_GR@DING]Congratulations!! test kheap_virtual_address completed successfully.",
		   #"[AUTO_GR@DING]Congratulations!! test kheap_physical_address completed successfully.",
		   #"[AUTO_GR@DING]Congratulations!! test the 3 creation functions is completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test malloc (1) completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test malloc (2) completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test malloc [3] completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test free [1] completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test free [2] completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test Next Fit completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test BEST FIT allocation (1) completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test BEST FIT allocation (2) completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! Test of PAGE PLACEMENT completed successfully!!",
           #"[AUTO_GR@DING]Nothing to be tested from here",	#tia
           #"[AUTO_GR@DING]Congratulations!! test PAGE replacement [ALLOCATION] is completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test PAGE replacement [ALLOCATION] is completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test PAGE replacement [CLOCK Alg.] is completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test PAGE replacement [LRU Alg.] is completed successfully.",
           #"[AUTO_GR@DING]Congratulations: stack pages created, modified and read successfully!",
           #"[AUTO_GR@DING]Congratulations: stack pages created, modified and read successfully!",
           "[AUTO_GR@DING]Congratulations!! test Scenario 1 completed successfully.",
           "[AUTO_GR@DING]Congratulations!! test Scenario 1 completed successfully.",
           #"TESTING ENVFREE SC 2",
           #"TESTING ENVFREE SC 2", 
           "TESTING MULTIPLE TIMES", 
           "TESTING MULTIPLE TIMES", 
           "TESTING MULTIPLE TIMES S2",
           "TESTING MULTIPLE TIMES S2",
           "TESTING MULTIPLE TIMES S3",
           "TESTING MULTIPLE TIMES S3"]
           #"TESTING MULTIPLE TIMES S4"]
           #"[AUTO_GR@DING]Congratulations!! test PAGE replacement [FIFO Alg.] is completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test PAGE replacement [Modified CLOCK Alg.] is completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test FIRST FIT allocation (1) completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test FIRST FIT allocation (2) completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test Worst Fit completed successfully.",
           #"[AUTO_GR@DING]Congratulations!! test realloc [1] is completed successfully.",
		   #"[AUTO_GR@DING]Congratulations!! test realloc [2] completed successfully."]

#The main function
def main():
       
    print "MAKE SURE TO RUN THIS SCRIPT USING python -u ./autograde.py (UNBUFFERED)"
    if check_copy_error_from_windows() == True:
        return

    print "Started at: " + datetime.now().strftime("%Y-%m-%d %I:%M:%S %p")
    global output_file_name
    output_file_name += "_" + datetime.now().strftime("%Y_%m_%d")
    output_file_name += ".csv"

    print "=============="
    print "Instructions:"
    print "     Output file: " + output_file_name
    print "     Values: 1 success, 0 fail, -2 compilation error ,-3 restarting, -4 WS needs to be larger, -5 initial WS problem, -6 timeout, -7 panic, -3 restart, -100 undefined student mistake (student commented the test call, or any other mistake)"
    print "     If a bochs window remains open and blocks autograde execution, kindly close it and autograde will continue"
    print "=============="

    dirs_finished = 0
    compile_errors= 0           #-2 value in the output file is a compilation error
    file_exists = os.path.isfile(output_file_name)
    output_file = open(output_file_name, 'a+')
    if(file_exists == False):
        output_file.writelines("Group ID")
        questions_names = get_questions_names()
        map(lambda x: output_file.write("," + x +",Time"), questions_names) #write the header(columns names)

    # traverse root directory, and list directories as dirs and files as files
    for _, dirs, _ in os.walk("./Groups"): #root, dirs, files
        dirs_count = len(dirs)
        dirs.sort() #WARNING: MANDATORY
        #print dirs
        print "Total Folders = ", dirs_count

        #go inside each folder and execute the grading system
        for directory_name in dirs:
            if "Done_" in directory_name:
                dirs_finished+=1
                continue
            
            testCaseCount = 0
            group_ID = directory_name
            is_new_group = 1
            for test in tests:
                noOfTests = len(tests)
                copy_test_code(directory_name, testCaseCount)
                
                #print the progress to console
                progress = float(dirs_finished) / float(dirs_count)
                sys.stdout.write("\r")
                sys.stdout.write("Current Group: %s [Tests finished so far: %0.2f%%] Groups Finished so far: %0.2f%%" %(group_ID, testCaseCount * 100.0/noOfTests, dirs_finished * 100.0/dirs_count))
                sys.stdout.flush()

                if testCaseCount == 0:
                    grade = -99
                    duration = -99
                else:
                    grade, duration = gradeGroup(directory_name, testCaseCount)

                if grade == -2:
                   compile_errors = compile_errors + 1
            
                #output to file
                if is_new_group:
                    output_file.writelines("\n") #new line
                    output_file.write(group_ID)
                    is_new_group = 0
                    #print "" #new line
                    #print current_student_ID,
                
                output_file.write(", " + str(grade) + ", " + str(duration))
                    #print ",", grade
    
                output_file.flush()
                testCaseCount +=1
            oldName = "./Groups/" + directory_name
            newName = "./Groups/Done_" + directory_name
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

def gradeGroup(group_dir, question_ID):

    #student_question_ID = student_ID + question_ID
    os.chdir("./Groups/"+group_dir)
    
    makeProc = subprocess.Popen("make", shell=True, stdout=PIPE, stderr=PIPE)
#    makeProc.wait()
    makeProc.communicate() #wait() caused the deadlock with stdout, err = PIPE, online documentation warns against that, and suggested the use of communicate()

    question_grade = -100
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
        question_grade = 0

        #A closure (inner function) that I will use later to terminate any stucking threads or processes
        # and finialize remainig resources
        def finializeAndCleanUp():
            try:
                os.kill(proc.pid, signal.SIGKILL)
            except Exception, msg:
                print msg

        #TODO: Consider removing the try catch statements as a running time optimization
        startTimeOfQuestion = datetime.now()
        
        while True:
            try:
                def signal_handler(signum, frame):
                    raise Exception("Timeout!")
                signal.signal(signal.SIGALRM, signal_handler)
                signal.alarm(60*5)   # 5 Min 
                try:
                    chunk = proc.stdout.read(1)
                except Exception, msg:
                    question_grade = -6
                    finializeAndCleanUp()
                    print msg
                    break
                stream_data += chunk 
                #print stream_data
                if "InitialWSError1" in stream_data:
                	question_grade = -4
                	finializeAndCleanUp()
                	break
                elif "InitialWSError2" in stream_data:
                	question_grade = -5
                	finializeAndCleanUp()
                	break
                elif "[EVAL_FINAL]panic" in stream_data:
                    if tests[question_ID] == 'tia' and "kernel [EVAL_FINAL]panic" in stream_data:
                        question_grade = 1
                        os.kill(proc.pid, signal.SIGKILL)
                        break
                    else:
                        question_grade = -7			#Panic Case
                        finializeAndCleanUp()
                        break
                elif stream_data.count('!! FCIS says HELLO !!') > 1:
                	question_grade = -3         #Restart Case
                	finializeAndCleanUp()
                	break
                elif outputs[question_ID] == "TESTING MULTIPLE TIMES" and stream_data.count('[AUTO_GR@DING]Congratulations!! The array is sorted correctly') == 3:
                	question_grade = 1
                	finializeAndCleanUp()
                	break
                elif outputs[question_ID] == "TESTING MULTIPLE TIMES S2" and "[AUTO_GR@DING]Fibonacci #30 = 1346269" in stream_data and stream_data.count('[AUTO_GR@DING]Congratulations!! The array is sorted correctly') == 4:
                    question_grade = 1
                    finializeAndCleanUp()
                    break
                elif outputs[question_ID] == "TESTING MULTIPLE TIMES S3" and stream_data.count('[AUTO_GR@DING]Congratulations!! The array is sorted correctly') == 2:
                    question_grade = 1
                    finializeAndCleanUp()
                    break
                elif outputs[question_ID] == "TESTING MULTIPLE TIMES S4" and stream_data.count('[AUTO_GR@DING]Congratulations!! The array is sorted correctly') == 1:
                    question_grade = 1
                    finializeAndCleanUp()
                    break
                elif outputs[question_ID] == "TESTING ENVFREE SC 2" and stream_data.count('[AUTO_GR@DING]Congratulations!! test Scenario 2 completed successfully.') == 1 and stream_data.count('[AUTO_GR@DING]Congratulations!! The array is sorted correctly') == 2:
                    question_grade = 1
                    finializeAndCleanUp()
                    break
                elif outputs[question_ID] in stream_data:
                	question_grade = 1
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

        
		#print stream_data
        
        # Additional check, unnecessary since we returned to use the blocking
        # stream read, but just in case that the TA closes the Bochs window
        # by mistake very fast, or in the student readline() cases
        if len(stream_data) < 210:
            print "WARNING: stream output very short ("+str(len(stream_data))+" chars). Very fast closing of Bochs window might have happened"


    else:
        startTimeOfQuestion = datetime.now()
        #print "Compilation error !!!"
        question_grade = -2

    timeNowQuestion = datetime.now()

    #delete the obj folder (optional)
    shutil.rmtree('./obj', ignore_errors=True)

    #try:
    #    while os.path.isfile(os.getcwd() + "/"+ student_stdout_Q_specific) == True:
    #        #keep waiting till we are sure that the current student_stdout file is deleted
    #        os.remove(os.getcwd() + "/" + student_stdout_Q_specific)
    #        time.sleep(0.001)
    #except Exception, msg:
    #    print msg

#    print os.getcwd()
    os.chdir("../../")
#    print os.getcwd()

    return question_grade, (timeNowQuestion - startTimeOfQuestion).seconds


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

def copy_test_code(groupDir,question_ID):
    root_dst_dir = './Groups/'+groupDir
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

