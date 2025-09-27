
# $folderImage = 'Image'
$folderVideo = 'Video'
$folderOutput = 'Output'
$folderTemp = 'temp'
$folderMusic = 'Music'
$resourcesFolder = 'resources'
$nameFile = 'DoubleInferno'
$musicName = 'Can You Hear The Music'
#$frameRate = 60
$startFadeD = 10
$EndFade = 101
$endFadeD = 0.2

# ffmpeg -framerate 60 -i test/images%d.bmp -c:v libx264 -pix_fmt yuv420p Doublevirdis2.mp4

# Rassemble toute les video de video/$nameFile_$i
# cree la liste des video
Write-Output "Listing the files"
# vide le texte
Clear-Content list.txt
# recup le nombre de fichier Count ${folderVideo}/${nameFile}_*.mp4'
$numberFile = (Get-ChildItem -Path ${resourcesFolder}/${folderVideo} -Filter ${nameFile}_*.mp4).Count

for ($i = 1; $i -le $numberFile; $i++) { 
    Write-Output "file '${resourcesFolder}/${folderVideo}/${nameFile}_$i.mp4'" >> list.txt
} 
Write-Output "File listed"

# Concatene les petites video
ffmpeg -safe 0 -f concat -i  list.txt -c copy ${resourcesFolder}/$folderTemp/${nameFile}_concat.mp4 -y

# get times of each piece 
$timeVideo = ffprobe -i ${resourcesFolder}/${folderTemp}/${nameFile}_concat.mp4 -show_entries format=duration -v quiet -of csv="p=0"
Write-Output "Time Video :${timeVideo}"
$timeMusic = ffprobe -i "${resourcesFolder}/${folderMusic}/${musicName}.mp3" -show_entries format=duration -v quiet -of csv="p=0"
Write-Output "Time Music :${timeMusic}"
$factor = $timeMusic / $timeVideo + 0.2
Write-Output "Time factor ${factor}"

Write-Output "Add Music ${musicName}"
ffmpeg -i ${resourcesFolder}/${folderTemp}/${nameFile}_concat.mp4 -i "${resourcesFolder}/${folderMusic}/${musicName}.mp3"  -filter_complex "[0:v]setpts=${factor}*PTS[v]"   -map "[v]" -map 1:a -c:v libx264 -c:a aac -shortest ${resourcesFolder}/${folderTemp}/${nameFile}_music.mp4 -y
Write-Output "Add fade"
ffmpeg -i ${resourcesFolder}/${folderTemp}/${nameFile}_music.mp4 -vf "fade=t=in:st=0:d=${startFadeD}, fade=t=out:st=${endFade}:d=${endFadeD}"  -c:a copy ${resourcesFolder}/${folderOutput}/${nameFile}_fade.mp4 -y
Set-Location "C:\Users\audri\OneDrive\Bureau\Docs\Programation_Elec\CCode\DoublePendulum\"

Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
