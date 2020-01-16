@echo off&setlocal ENABLEDELAYEDEXPANSION
for /f "delims=" %%a in (build_config.json) do set #json=!#json!%%a
set #json=!#json: =!
set #json=!#json:"=!
set #json=!#json:{= { !
set #json=!#json:}= } !
set #json=!#json:[= [ !
set #json=!#json:]= ] !
set "#prefix#="
set /a #arr=-1,#lv=0

set #id=""
set #name=""
set _devenv="%VS90COMNTOOLS%D:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com"

for %%a in (!#json!) do (
	for /f "tokens=1 delims={}[]" %%K in ("%%~a") do (
		for /f "tokens=1,2 delims=:" %%A in ("%%~a") do (
			if "%%~A"=="%%~a" (
				if defined arr_!#lv! set /a #arr+=1&echo,!#prefix#![!#arr!]=%%A 
			)
			if "%%~A:"=="%%~a" set #prefix#=!#prefix#!.%%A
			if not "%%~A"=="%%~a" if not "%%~A:"=="%%~a" (
					if "%%A"=="id" 	set #id=%%B
					if "%%A"=="name" set #name=%%B
			)
			if not !#id!=="" if not !#name!=="" (		
				
				rd /s /q  ".\Auto_AI\GameLogic"
				mkdir  ".\Auto_AI\GameLogic"
				xcopy /y  ".\Auto_AI\!#id!"  ".\Auto_AI\GameLogic"
				@echo Build Release Start!!
				del releaseOutput_!#id!.txt
				echo "%%a" >> releaseOutput_!#id!.txt
				time /T >> releaseOutput_!#id!.txt
				%_devenv% "Auto_AI.sln" /rebuild "Release|Win32" /Out releaseOutput_!#id!.txt
				@echo Release Build Finish!!
				time /T >> releaseOutput_!#id!.txt
				if not exist ".\!#id!_!#name!" md ".\!#id!_!#name!" 
				xcopy /y  ".\Release"  ".\!#id!_!#name!"					
				set #id=""
				set #name=""
			)
		)
	)
	if "%%~a"=="{" (
		if defined arr_!#lv! set /a #arr+=1
		setlocal
		if defined arr_!#lv! set #prefix#=!#prefix#![!#arr!]
		set /a #lv+=1
	)
	if "%%~a"=="}" (
		endlocal
	)
	if	"%%~a"=="]" (
		endlocal
	)
	if "%%~a"=="[" (
		setlocal
		set /a #lv+=1,#arr=-1
		set arr_!#lv!= +
	)
)