$PSDefaultParameterValues['Out-File:Encoding'] = 'utf8'

$tmp = New-TemporaryFile

.\TEST_bigint.exe | Out-File $tmp.FullName

python $tmp.FullName

if ($LastExitCode -eq 0) {
    Remove-Item $tmp.FullName 
    Write-Host "SUCCESS"
}else {
    Write-Host $tmp.FullName
    Write-Host "FAILED"
}


