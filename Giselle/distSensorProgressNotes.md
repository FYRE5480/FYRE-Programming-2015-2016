Previously: Mega (slave) and Launchpad (master, stand-in for Rio) will transmit proper distance number across I2C. (Except for -1, which puts across 255, which is weird because I changed literally nothing since it was sending across -1. And then I ran it again and it was properly -1. Who knows.) Put the Rio in the loop and it's not actually working yet. -Nix

16.02.10: I do actually have the proper port number in the current Rio code. (See? Wired correctly.) Figured out that the Rio isn't sending to the Mega (slave) the proper message to make the Mega send back the distance. To fix this . . . make the Rio write random stuff to the Mega and make the Mega send back on receiving literally anything? -Nix

16.02.12: Well, code now uploads to the robot again. What I proposed last time doesn't actually work for whatever reason. Connor, this link is for you: http://www.chiefdelphi.com/forums/showthread.php?p=1415788 Download the file, it's actually in there. I'm going to start looking into using something else like idk. -Nix

16.02.13: Trying to use DIOs and bypassing arduino altogether. -Nix
	5:34p: Ended up using new distance sensor with analog inputs and using the following equation to convert: y = 0.1319x-1.3456 where y is the actual distance and x is the given distance.