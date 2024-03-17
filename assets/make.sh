sizes=(16 20 28 32 42 55 64 78 88 108 128)
resized_sizes=()
template_sizes=()


for i in "${sizes[@]}"
do 
    if [ ${i} -gt 32 ];
    then
        template_sizes+=(64)
        # echo 64
    else
        template_sizes+=($(( (i + 15) / 16 * 16 )))
        # echo ${i}
    fi
    if [ ${i} -ge 64 ];
    then
        resized_sizes+=(64)
        # echo 64
    else
        resized_sizes+=($i)
        # echo ${i}
    fi
done

# sizes=(16 20 28 32 42 55 64 64 64 64 64)

for i in "${!sizes[@]}";
do



    echo ${i} ${sizes[i]} 
    magick convert sphere-$(printf %02d $i).png -background "rgb(255,255,255)" -channel A -threshold 250 -background "rgb(255,255,255)" -resize "${resized_sizes[i]}"x"${resized_sizes[i]}" -gravity NorthWest -extent "${template_sizes[i]}"x"${template_sizes[i]}" ball-$(printf %02d $i).png 
    if [ ${sizes[i]} -gt 64 ];
    then
        # magick convert ball-$(printf %02d $j).png -crop 100%x50%+0+0 ball-$(printf %02d $j)-half.png
        magick convert sphere-$(printf %02d $i).png -background "rgb(255,255,255)" -channel A -threshold 250 -background "rgb(255,255,255)" -resize "${resized_sizes[i]}"x"${resized_sizes[i]}" -gravity NorthWest -crop x50% -extent 64x32 ball-$(printf %02d $i)-half-00.png 
        magick convert sphere-$(printf %02d $i).png -background "rgb(255,255,255)" -channel A -threshold 250 -background "rgb(255,255,255)" -resize "${resized_sizes[i]}"x"${resized_sizes[i]}" -gravity NorthWest -crop x50%+0+32 -extent 64x32 ball-$(printf %02d $i)-half-01.png 

    fi
done
# magick convert sphere-00.png -background none -resize 128x128 -gravity NorthWest -extent 128x128 ball10.png
# convert input.png -crop 4x4@ +repage output%02d.png