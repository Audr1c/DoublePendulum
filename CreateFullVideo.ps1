$folderImage = 'Image'
$folderVideo = 'Video'
# $folderOutput = 'Output'
$folderTemp =  'temp'
$folderMusic = 'Music'
$nameFile = 'DoubleInferno'
$frameRate = 60
$NumberOfimageByVideo = 200
$musicName = 'Can You Hear The Music'
$resourcesFolder = 'resources'


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
$dt = 0.015
# $dt = 1
$alpha = 10
$cmap = "inferno"

# Create folders if they don't exist
if (!(Test-Path $resourcesFolder)) {
    New-Item -ItemType Directory -Path $resourcesFolder
}
if (!(Test-Path "${resourcesFolder}/${folderImage}")) {
    New-Item -ItemType Directory -Path "${resourcesFolder}/${folderImage}"
}
if (!(Test-Path "${resourcesFolder}/${folderVideo}")) {
    New-Item -ItemType Directory -Path "${resourcesFolder}/${folderVideo}"
}
if (!(Test-Path "${resourcesFolder}/${folderTemp}")) {
    New-Item -ItemType Directory -Path "${resourcesFolder}/${folderTemp}"
}
if (!(Test-Path "${resourcesFolder}/${folderMusic}")) {
    New-Item -ItemType Directory -Path "${resourcesFolder}/${folderMusic}"
}



$timeMusic = ffprobe -i "${folderMusic}/${musicName}.mp3" -show_entries format=duration -v quiet -of csv="p=0"
Write-Output "Time Music :${timeMusic}"


Set-Location "C:\Users\audri\OneDrive\Bureau\Docs\Programation_Elec\CCode\DoublePendulum\"

#clear the image folder 
Remove-Item -Path "${resourcesFolder}/${folderImage}/*" -Recurse

$index = $skipFrames/$NumberOfimageByVideo + 1
for ($frame = $skipFrames; $frame -lt $totalFrames; $frame += $NumberOfimageByVideo ) {

    $tempFrame = $frame + $NumberOfimageByVideo 
    # make the images
    Write-Output "Doing frame ${frame} to ${tempFrame}:"
    .\main.exe -f $tempFrame -h $thread -p $N_pend -s $frame -o $folderImage -y $typePendulum -w $imagewidth -b $sizeBrush -u $updateColor -i $startAngle -j $endAngle -z $th1or2 -k $propLength -t $dt -a $alpha -c $cmap
    
    # make the video
    Write-Output "Video ${index} frame ${frame} to ${tempFrame}:"
    ffmpeg -framerate $frameRate -start_number $frame -i ${resourcesFolder}/${folderImage}/images%d.bmp -c:v libx264 -pix_fmt yuv420p ${resourcesFolder}/${folderVideo}/${nameFile}_$index.mp4  -v quiet -y
    
    # delete image
    $failed = $false
    Write-Output "Deleting frame ${frame} to ${tempFrame}:"
    for ($i = $frame; $i  -lt $tempFrame; $i++) {
        if (Test-Path "${resourcesFolder}/${folderImage}/images${i}.bmp"){
            Remove-Item "${resourcesFolder}/${folderImage}/images${i}.bmp"
        }
        else {
            Write-Output "Couldn't find file ${resourcesFolder}/${folderImage}/images${i}.bmp"
            $failed = $true
            break
        }
    }
    if ($failed){
        Write-Output "Failed at $index"
    }
    Write-Output "Done"
    $index = $index + 1
    
}
    

# ffmpeg -framerate $frameRate -i test/images%d.bmp -c:v libx264 -pix_fmt yuv420p ${folderVideo}/${nameFile}_$i.mp4
# Rassemble toute les video de video/$nameFile_$i

Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
Write-Output "DONE !!!!!!!!!!!!"
