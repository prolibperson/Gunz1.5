@rem -----------------------------
@rem 환경 변수 설정
@rem -----------------------------
@CALL Utils\bin\setenv

@cd Utils\bin
@call _getdatetime.bat datetime
@cd ..\..
if "%datetime%" == "" GOTO ERROR


@rem -----------------------------
@rem Clear
@rem -----------------------------
del "Gunz\Runtime\Gunz.exe"
del "Gunz\Runtime\Gunz.pdb"


@rem -----------------------------
@rem 소스, 리소스를 받아온다.
@rem -----------------------------
CALL updatefiles

@rem -----------------------------
@rem 컴파일
@rem -----------------------------

%DEVENV% "Gunz\Gunz.sln" /rebuild Release_publish /out "Gunz\Runtime\build.log"


@rem -----------------------------
@rem ddsgen 실행
@rem ------------------------------

@cd "Gunz\Runtime"

CALL Delete_DDS
CALL ddsgen

CALL copy_share


@rem -----------------------------
@rem publish 실행
@rem ------------------------------

if "%1" == "" goto :no_target
if "%datetime%" == "" goto :no_target
SET build_target=\\Databank\TeamWorks\Gunz\BuildClient\!Daily\Gunz_%datetime%\


:no_target


CALL publish kor %build_target%

@cd "..\.."


GOTO END


:ERROR
echo 빌드 에러
pause
exit

:END
