#!/bin/bash

convert_small () {
	SRC="$1"
	DST="$2"
	COL="$3"
	TMP=`mktemp -t kcemu.convert.XXXXXXXXXX` || exit 1
	echo "converting $SRC to $DST (color = $COL, temp = $TMP)..."
	rsvg-convert -a -f png -o "$TMP" -h 22 "$SRC"
	convert -colorize "$COL" "$TMP" "$DST"
	rm -f "$TMP"
}

convert_big () {
	SRC="$1"
	DST="$2"
	COL="$3"
	GAMMA="$4"
	TMP=`mktemp -t kcemu.convert.XXXXXXXXXX` || exit 1
	echo "converting $SRC to $DST (color = $COL, gamma = $GAMMA, temp = $TMP)..."
	rsvg-convert -a -f png -o "$TMP" -h 32 "$SRC"
	convert -colorize "$COL" -gamma "$GAMMA" "$TMP" "$DST"
	rm -f "$TMP"
}

COLOR_GREEN=15,0,15
COLOR_RED=0,30,30
GAMMA=1.4,1.4,1.4

convert_small player_play.svg   ../media-play.png      $COLOR_GREEN
convert_small player_fwd.svg    ../media-forward.png   $COLOR_GREEN
convert_small player_rew.svg    ../media-rewind.png    $COLOR_GREEN
convert_small player_start.svg  ../media-start.png     $COLOR_GREEN
convert_small player_end.svg    ../media-end.png       $COLOR_GREEN
convert_small player_pause.svg  ../media-pause.png     $COLOR_GREEN
convert_small player_stop.svg   ../media-stop.png      $COLOR_GREEN
convert_small player_eject.svg  ../media-eject.png     $COLOR_GREEN
convert_small player_record.svg ../media-record.png    $COLOR_RED

convert_big jabber_protocol.svg ../window-help.png     $COLOR_GREEN $GAMMA
convert_big tux.svg             ../window-about.png    $COLOR_GREEN $GAMMA
convert_big xmms.svg            ../window-audio.png    $COLOR_GREEN $GAMMA
convert_big xedit.svg           ../window-copying.png  $COLOR_GREEN $GAMMA
convert_big 5floppy_mount.svg   ../window-disk.png     $COLOR_GREEN $GAMMA
convert_big blockdevice.svg     ../window-module.png   $COLOR_GREEN $GAMMA
convert_big kdat.svg            ../window-tape.png     $COLOR_GREEN $GAMMA
convert_big kuser.svg           ../window-thanks.png   $COLOR_GREEN $GAMMA
convert_big help.svg            ../window-question.png $COLOR_GREEN $GAMMA
convert_big info.svg            ../window-info.png     $COLOR_GREEN $GAMMA