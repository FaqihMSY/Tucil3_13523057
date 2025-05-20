# build.ps1
# Bersihkan isi bin kecuali .gitkeep
if (Test-Path .\bin) {
    Get-ChildItem -Path .\bin\* -Exclude .gitkeep | Remove-Item -Recurse -Force
} else {
    New-Item -ItemType Directory -Path .\bin | Out-Null
    New-Item -ItemType File -Path .\bin\.gitkeep | Out-Null
}
# Build ulang
cmake -G "MinGW Makefiles" -B bin
cmake --build bin
# Jalankan
.\bin\rush_hour.exe
