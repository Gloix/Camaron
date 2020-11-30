$script_path = split-path -parent $MyInvocation.MyCommand.Definition

$glew_version = "2.1.0"
$glm_version = "0.9.9.8"

$glew_zip_download_url = "https://iweb.dl.sourceforge.net/project/glew/glew/$glew_version/glew-$glew_version-win32.zip?viasf=1"
$glm_zip_download_url = "https://github.com/g-truc/glm/releases/download/$glm_version/glm-$glm_version.zip"

$temp_path = Join-Path -Path $script_path -ChildPath ".temp"
$temp_path = Join-Path -Path $temp_path -ChildPath "win32"

$libs_path = Join-Path -Path $script_path -ChildPath "libs"
$libs_path = Join-Path -Path $libs_path -ChildPath "win32"

$glew_install_path = Join-Path -Path $libs_path -ChildPath "glew"
$glm_install_path = Join-Path -Path $libs_path -ChildPath "glm"

$glew_temp_path = Join-Path -Path $temp_path -ChildPath "glew"
$glm_temp_path = Join-Path -Path $temp_path -ChildPath "glm"

$glew_zip_path = Join-Path -Path $temp_path -ChildPath "glew.zip"
$glm_zip_path = Join-Path -Path $temp_path -ChildPath "glm.zip"

mkdir $temp_path -ea 0
mkdir $libs_path -ea 0

if (!($glew_zip_path | Test-Path)) {
    Invoke-WebRequest -Uri $glew_zip_download_url -OutFile $glew_zip_path
}
if (!($glm_zip_path | Test-Path)) {
    Invoke-WebRequest -Uri $glm_zip_download_url -OutFile $glm_zip_path
}

Remove-Item -Path $glew_install_path -Recurse -Force -ea 0
Remove-Item -Path $glm_install_path -Recurse -Force -ea 0

Expand-Archive -Force -LiteralPath $glew_zip_path -DestinationPath $glew_temp_path
Expand-Archive -Force -LiteralPath $glm_zip_path -DestinationPath $glm_temp_path

Write-Host (Join-Path -Path $glew_temp_path -ChildPath "glew-$glew_version")
Move-Item (Join-Path -Path $glew_temp_path -ChildPath "glew-$glew_version") $glew_install_path
Move-Item (Join-Path -Path $glm_temp_path -ChildPath "glm") $glm_install_path