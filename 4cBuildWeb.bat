xcopy assets web_build\assets /s /e /y /q

pushd web_build
call EmscriptenBuild.bat
popd