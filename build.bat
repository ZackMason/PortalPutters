IF "%1"=="" (
    call 4cBuildWin.bat
) ELSE (
    IF "%1"=="win" (
        call 4cBuildWin.bat
    ) ELSE (
        IF "%1"=="web" (
            call 4cBuildWeb.bat
            call zip_web_build.bat
        )
    )
)