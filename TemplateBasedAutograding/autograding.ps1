$baseDirectory = split-path -parent $MyInvocation.MyCommand.Definition

$msbuild = "C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
$MSBuildLogger="/flp1:Append;LogFile=Build.log;Verbosity=Normal; /flp2:LogFile=BuildErrors.log;Verbosity=Normal;errorsonly"
$devenv = "C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe"
$action = "Y"

Clear-Host

Write-Host "Please make sure you do not have any instances of Visual studio running before running the script..." -ForegroundColor Red -BackgroundColor White
Write-Host "You have 10 seconds" -ForegroundColor Red -BackgroundColor White

Write-Host "Grading Criteria" -ForegroundColor Red -BackgroundColor White
Write-Host "`t -1 `t`t Default" -ForegroundColor Red -BackgroundColor White
Write-Host "`t -2 `t`t No sln file found" -ForegroundColor Red -BackgroundColor White
Write-Host "`t -3 `t`t Compilation Error" -ForegroundColor Red -BackgroundColor White
Write-Host "`t -4 `t`t Throws exception" -ForegroundColor Red -BackgroundColor White
Write-Host "`t 0 `t`t Wrong Answer" -ForegroundColor Red -BackgroundColor White
Write-Host "`t -5 `t`t TimeLimitExceeds" -ForegroundColor Red -BackgroundColor White
Write-Host "`t -6 `t`t TimeOut" -ForegroundColor Red -BackgroundColor White

#Start-Sleep -s 10
$questions = @("[1] Fast Bounded Fib")

$studentsDir = $baseDirectory +"\Students";

$date = [System.DateTime]::Now.ToString("dd-MMMM-yyyy-HH-mm-ss")
$fileName = $date + "Result.csv"
New-Item $baseDirectory\$fileName -type file
$firstLine = "StudentId"
foreach ($element in $questions) {
	$firstLine += "," + $element 
}

Add-Content $baseDirectory\$fileName $firstLine

$studentResponses =  $studentsDir | Get-ChildItem | ?{ $_.PSIsContainer }

foreach($student in $studentResponses)
{
    $studntRecordInFile = $student.Name
    foreach ($element in $questions)
    {
       # $questionPath = $student | Get-ChildItem -Filter $element | Select-Object -First 1 | ?{ $_.PSIsContainer }
        $questionPath = $student 
        $studentGrade = -1
        $slnFile = $questionPath | Get-ChildItem -Recurse -Filter *.sln | Select-Object -First 1

        if ($slnFile) 
        {
            #Write-Host "Building $projectFileAbsPath"
            $tempPath = $questionPath.FullName
            & $msbuild $slnFile.FullName /t:rebuild
                
            if($LASTEXITCODE -eq 0)
            {
                #Write-Host "Build SUCCESS" -ForegroundColor Green
                $exePath  =  $questionPath | Get-ChildItem -Recurse -Filter *.exe | Select-Object -First 1
                $pinfo = New-Object System.Diagnostics.ProcessStartInfo
                $pinfo.FileName = $exePath.FullName
                $pinfo.Arguments = "1"
                $pinfo.UseShellExecute = $falsee
                $pinfo.CreateNoWindow = $true
                $pinfo.RedirectStandardOutput = $true
                $pinfo.RedirectStandardError = $true
                $pinfo.WorkingDirectory = $exePath.Directory.FullName;


                $process = New-Object System.Diagnostics.Process
                $process.StartInfo = $pinfo
                # Start the process
                $process.Start() | Out-Null

                # Wait a while for the process to do something
                #sleep -Seconds 5
                #$process.WaitForExit()
                Wait-Process -Id $process.id -Timeout 10
            
                # If the process is still active kill it
                if (!$process.HasExited) {
                    $process.Kill()
                }
            

                # get output from stdout and stderr
                $stdout = $process.StandardOutput.ReadToEnd()
                $stderr = $process.StandardError.ReadToEnd()
                if($process.ExitCode -eq -1)
                {
                    $studentGrade = -6
                }
				elseif($process.ExitCode -lt 0)
                {
                    $studentGrade = -4
                }
                elseif($stderr.Length > 0)
                {
                    $studentGrade = -4
                }
                elseif($stdout.Contains("[#EVAL]Time limit exceed"))
                {
                    $studentGrade = 0.3
                }
                elseif($stdout.Contains("[#EVAL]Wrong Answer"))
                {
                    $studentGrade = 0.25
                }
                elseif($stdout.Contains("[#EVAL]Congratulations"))
                {
                    $studentGrade = 1
                }
            }
            else
            {
                $studentGrade = "-3";
            }

        }
        else
        {
            $studentGrade = "-2";
        }
         $studntRecordInFile += "," + $studentGrade

    }
   
    Add-Content $baseDirectory\$fileName $studntRecordInFile
    $newName = "Done_" + $student.Name
    Rename-Item $student.FullName $newName

}