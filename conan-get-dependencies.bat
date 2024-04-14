conan install ..\engine\. ^
--output-folder=dependencies ^
-s build_type=Release ^
-s arch=x86 ^
--build=missing ^
--format=json > build.json
