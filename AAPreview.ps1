$folderImage = 'Image'
$folderVideo = 'Video'
# $folderOutput = 'Output'
$folderTemp = 'temp'
$folderMusic = 'Music'
$nameFile = 'DoubleInferno'
$frameRate = 60
$NumberOfimageByVideo = 30
$musicName = 'Can You Hear The Music'
$resourcesFolder = 'resources'
$folderPreview = 'Preview'


# Genral Parameters and Drawing Parameters 
# GeneralParameters gp = { 5000, 1, 3000, 0, "test", NULL };
# Drawing_Param dp = { 2, 2000, 12, 0, M_PI / 2, M_PI / 2 + 0.1, 1, 0.5f, 0.03, 10, "viridis" };

$totalFrames = 6000
$imagewidth = 2000
$N_pend = 3000
$skipFrames = 0
$typePendulum = 2
$thread = 1
$sizeBrush = 12
$updateColor = 0
$startAngle = [math]::PI / 2
$endAngle = [math]::PI / 2 + 0.1
$th1or2 = 1
$propLength = 0.5
# $dt = 0.01
$dt = 1
$alpha = 10
$cmap = "inferno"


$timeMusic = ffprobe -i "$resourcesFolder/${folderMusic}/${musicName}.mp3" -show_entries format=duration -v quiet -of csv="p=0"
Write-Output "Time Music :${timeMusic}"


Set-Location "C:\Users\audri\OneDrive\Bureau\Docs\Programation_Elec\CCode\DoublePendulum\"

#clear the image folder 
Remove-Item -Path "${resourcesFolder}/${folderImage}/*" -Recurse

# faire une palette de couleur
# faire 10 images en haute qualité
# faire une video en basse qualité : 300 image 200x200

# palett de couleur
Write-Output "Palette :"
$startAnglePalette = - [math]::PI / 2
$enAnglePalette =  [math]::PI/2
.\main.exe -f 1 -h 1 -p 500 -s 0 -o $folderPreview -y 1 -w 2000 -b 5 -u $updateColor -i $startAnglePalette -j $enAnglePalette -z $th1or2 -k $propLength -t $dt -a $alpha -c $cmap

Write-Output "HQ :"
# faire 10 images en haute qualite
$dtQualty = 10*$dt
$folderImageQualty = 'ImagesHQ'
.\main.exe -f 10 -h $thread -p $N_pend -s $skipFrames -o ${folderPreview}/$folderImageQualty -y $typePendulum -w $imagewidth -b $sizeBrush -u $updateColor -i $startAngle -j $endAngle -z $th1or2 -k $propLength -t $dtQualty -a $alpha -c $cmap
# .\main.exe -f 10 -h 1 -p 3000 -s 0 -o "Preview/ImagesHQ" -y 2 -w 2000 -b 20 -u 0 -i [math]::PI / 2 -j [math]::PI / 2+0.1 -z 1 -k 0.5 -t 3 -a 10  -c "inferno"
Write-Output "Video :"
$imagewidth = 200




$index = 1
for ($frame = 0; $frame -lt $totalFrames; $frame += $NumberOfimageByVideo ) {

    $tempFrame = $frame + $NumberOfimageByVideo 
    # make the images
    Write-Output "Doing frame ${frame} to ${tempFrame}:"
    .\main.exe -f $tempFrame -h $thread -p $N_pend -s $frame -o ${folderPreview}/$folderImage -y $typePendulum -w $imagewidth -b $sizeBrush -u $updateColor -i $startAngle -j $endAngle -z $th1or2 -k $propLength -t $dt -a $alpha -c $cmap
    
    # make the video
    Write-Output "Video ${index} frame ${frame} to ${tempFrame}:"
    ffmpeg -framerate $frameRate -start_number $frame -i ${resourcesFolder}/${folderPreview}/${folderImage}/images%d.bmp -c:v libx264 -pix_fmt yuv420p ${resourcesFolder}/${folderPreview}/${folderVideo}/${nameFile}_$index.mp4  -v quiet -y
    
    # delete video
    Write-Output "Deleting frame ${frame} to ${tempFrame}:"
    for ($i = $frame; $i -lt $tempFrame; $i++) {
        Remove-Item "${resourcesFolder}/${folderPreview}/${folderImage}/images${i}.bmp"
    }
    Write-Output "Done"
    $index = $index + 1
    
}    

# ffmpeg -framerate $frameRate -i test/images%d.bmp -c:v libx264 -pix_fmt yuv420p ${folderVideo}/${nameFile}_$i.mp4
# Rassemble toute les video de video/$nameFile_$i


$folderVideo = 'Video'
$folderOutput = 'Output'
$folderTemp =  'temp'
$folderMusic = 'Music'
$resourcesFolder = 'resources'
$nameFile = 'Testing'
$musicName = 'Can You Hear The Music'

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
$numberFile = (Get-ChildItem -Path ${resourcesFolder}/${folderPreview}/${folderVideo} -Filter ${nameFile}_*.mp4).Count

for ($i = 1; $i -le $numberFile; $i++) { 
    Write-Output "file '${resourcesFolder}/${folderPreview}/${folderVideo}/${nameFile}_$i.mp4'" >> list.txt
 } 
Write-Output "File listed"

ffmpeg -safe 0 -f concat -i  list.txt -c copy ${resourcesFolder}/${folderPreview}/$folderTemp/${nameFile}_concat.mp4 -y

# get times of each piece 
$timeVideo = ffprobe -i ${resourcesFolder}/${folderPreview}/${folderTemp}/${nameFile}_concat.mp4 -show_entries format=duration -v quiet -of csv="p=0"
Write-Output "Time Video :${timeVideo}"
$timeMusic = ffprobe -i "${resourcesFolder}/${folderMusic}/${musicName}.mp3" -show_entries format=duration -v quiet -of csv="p=0"
Write-Output "Time Music :${timeMusic}"
$factor = $timeMusic / $timeVideo + 0.2
Write-Output "Time factor ${factor}"

Write-Output "Add Music ${musicName}"
ffmpeg -i ${resourcesFolder}/${folderPreview}/${folderTemp}/${nameFile}_concat.mp4 -i "${resourcesFolder}/${folderMusic}/${musicName}.mp3"  -filter_complex "[0:v]setpts=${factor}*PTS[v]"   -map "[v]" -map 1:a -c:v libx264 -c:a aac -shortest ${resourcesFolder}/${folderPreview}/${folderTemp}/${nameFile}_music.mp4 -y
Write-Output "Add fade"
ffmpeg -i ${resourcesFolder}/${folderPreview}/${folderTemp}/${nameFile}_music.mp4 -vf "fade=t=in:st=0:d=${startFadeD}, fade=t=out:st=${endFade}:d=${endFadeD}"  -c:a copy ${resourcesFolder}/${folderPreview}/${folderOutput}/${nameFile}_fade.mp4 -y


Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
