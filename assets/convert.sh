#!/bin/sh

# grit ball.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
# grit character.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
mkdir -p ../nitrofiles/sprite
mkdir -p ../nitrofiles/bg
mkdir -p ../nitrofiles/fnt

templatesize=(16 32 32 32 64 64 64 128 128 128 128)

for s in  {0..10}
do
    grit ball-$(printf %02d $s).png -ftb -fh! -gTFFFFFF -gt -gB8 -m!
    # if [ ${templatesize[$s]} -gt 127 ];
    # then
    #     for j in  {0..3}
    #     do
    #         grit ball-$(printf %02d $s)-$j.png -ftb -fh! -gTFFFFFF -gt -gB8 -m!
    #     done
    # fi
    if test -f "ball-$(printf %02d $s)-half.png"; then
        grit ball-$(printf %02d $s)-half-00.png -ftb -fh! -gTFFFFFF -gt -gB8 -m!
        grit ball-$(printf %02d $s)-half-01.png -ftb -fh! -gTFFFFFF -gt -gB8 -m!
    fi
done

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite

grit suika_bottom.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit suika_top.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img *.map ../nitrofiles/bg

grit default.png -ftb -fh! -gTFF00FF -gt -gB8 -m!

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

for file in *.img; do
    mv -- "$file" "${file%.img}".fnt
done

mv *.pal *.fnt ../nitrofiles/fnt
