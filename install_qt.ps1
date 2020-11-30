param ($qtInstallPath)

$script_path = split-path -parent $MyInvocation.MyCommand.Definition
$qt_version = "4.0.0"
$qt_creator_installer_url = "https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-unified-windows-x86-$qt_version-online.exe"
$qt_installer_path = Join-Path -Path $script_path -ChildPath ".temp\qt_installer.exe"
$qt_default_install_path = "C:\QT2"

$qtInstallPath = Read-Host -Prompt "QT install path [$qt_default_install_path]"

if ("" -eq $qtInstallPath) {
    $qtInstallPath = $qt_default_install_path
}

function ConvertFrom-SecureToPlain {
    
    param( [Parameter(Mandatory=$true)][System.Security.SecureString] $SecurePassword)
    
    # Create a "password pointer"
    $PasswordPointer = [Runtime.InteropServices.Marshal]::SecureStringToBSTR($SecurePassword)
    
    # Get the plain text version of the password
    $PlainTextPassword = [Runtime.InteropServices.Marshal]::PtrToStringAuto($PasswordPointer)
    
    # Free the pointer
    [Runtime.InteropServices.Marshal]::ZeroFreeBSTR($PasswordPointer)
    
    # Return the plain text password
    $PlainTextPassword
    
}

$qt_email = Read-Host -Prompt 'QT account email'
$qt_pass = Read-Host -assecurestring 'QT account password'

$qt_pass_decrypted = (ConvertFrom-SecureToPlain $qt_pass)

mkdir (Join-Path -Path $script_path -ChildPath ".temp") -ea 0

if (!($qt_installer_path | Test-Path)) {
    Invoke-WebRequest -Uri $qt_creator_installer_url -OutFile $qt_installer_path
}

& $qt_installer_path --root $qtInstallPath --accept-licenses --default-answer --email "$qt_email" --password "$qt_pass_decrypted" --accept-obligations --confirm-command install
