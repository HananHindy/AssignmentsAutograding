$baseDirectory = split-path -parent $MyInvocation.MyCommand.Definition

$msbuild = "C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
$MSBuildLogger="/flp1:Append;LogFile=Build.log;Verbosity=Normal; /flp2:LogFile=BuildErrors.log;Verbosity=Normal;errorsonly"
$mstestPath = "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\mstest.exe"
$action = "Y"

Clear-Host

Write-Host "Please make sure you do not have any instances of Visual studio running before running the script..." -ForegroundColor Red -BackgroundColor White
Write-Host "You have 10 seconds" -ForegroundColor Red -BackgroundColor White

#Start-Sleep -s 10
$testCases = @("CeaserTestEnc1",
               "CeaserTestDec1",
               "CeaserTestAnalysis1",
               "CeaserTestEnc2",
               "CeaserTestDec2",
               "CeaserTestAnalysis2",
               "CeaserTestEnc3",
               "CeaserTestDec3",
               "CeaserTestAnalysis3",
               "CeaserTestNewEnc1",
               "CeaserTestNewDec1",
               "CeaserTestNewAnalysis1",
               "MonoTestEnc1",
               "MonoTestDec1",
               "MonoTestAnalysisNaive1",
               "MonoTestEnc2",
               "MonoTestDec2",
               "MonoTestAnalysisNaive2",
               "MonoTestEnc3",
               "MonoTestDec3",
               "MonoTestAnalysisNaive3",
               "MonoTestEnc4",
               "MonoTestDec4",
               "MonoTestAnalysisFrequency",
               "MonoTestNewEnc",
               "MonoTestNewDec",
               "MonoTestNewAnalysisNaive",
               "PlayfairTestEnc1",
               "PlayfairTestDec1",
               "PlayfairTestEnc2",
               "PlayfairTestDec2",
               "PlayfairTestEnc3",
               "PlayfairTestDec3",
               "PlayfairTestEnc4",
               "PlayfairTestDec4",
               "PlayfairTestBonusAnalysis",
               "PlayfairTestNewEnc",
               "PlayfairTestNewDec")

$studentsDir = $baseDirectory +"\Groups";

$date = [System.DateTime]::Now.ToString("dd-MMMM-yyyy-HH-mm-ss")
$fileName = $date + "Result.csv"
New-Item $baseDirectory\$fileName -type file
$firstLine = "GroupId"
foreach ($element in $testCases) {
	$firstLine += "," + $element 
}

Add-Content $baseDirectory\$fileName $firstLine

$studentResponses =  $studentsDir | Get-ChildItem | ?{ $_.PSIsContainer }

foreach($student in $studentResponses)
{
    #copy test cases to ovverrride
    $from = $baseDirectory + "\SecurityPackageTest"
    $to = $student.FullName
    Copy-Item $from $to -recurse -Force
    $studntRecordInFile = $student.Name
   
    $questionPath = $student 
    $studentGrade = -1 #Default Grade
    $slnFile = $questionPath | Get-ChildItem -Recurse -Filter *.sln | Select-Object -First 1

    if ($slnFile) 
    {
        #Write-Host "Building $projectFileAbsPath"
        $tempPath = $questionPath.FullName
        & $msbuild $slnFile.FullName /t:rebuild
                
        if($LASTEXITCODE -eq 0)
        {
            #Write-Host "Build SUCCESS" -ForegroundColor Green
            $dllPath  =  $questionPath | Get-ChildItem -Recurse -Filter SecurityPackageTest.dll | Select-Object -First 1
            $dllFullPath =  $dllPath.FullName
            & $mstestPath /testcontainer:$dllFullPath | Tee-Object -Variable scriptOutput | Out-Null
            foreach($test in $testCases)
            {
                $studentGrade = "NOGRADE"
                foreach ($line in $($scriptOutput -split "`r`n"))
                {
                    if($line -match $test)
                    {
                        if($line -match "Passed")
                        {
                            $studentGrade  = "1"
                        }
                        else
                        {
                            $studentGrade  = "0"
                        }
                    }
                }
                $studntRecordInFile += "," + $studentGrade
            }
          
        }
        else
        {
            $studentGrade = "COMPERROR";
            $studntRecordInFile += "," + $studentGrade
        }

    }
    else
    {
        $studentGrade = "NOSLNFILE";
        $studntRecordInFile += "," + $studentGrade
    }
    
  
    Add-Content $baseDirectory\$fileName $studntRecordInFile
}