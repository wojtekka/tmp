/tmp - tiny/tani mp3 player
===========================

Guidelines:
- simple and cheap mp3 player,
- no gadgets like LCD display,
- file upload via external card reader.

The project has been designed using Linux software so AVR Studio (and
other Windows IDE's) users will have to create project manually and add
all attached files. It would be nice if someone sent me the prepared
project so I could add it to the distribution.

I use software RS232 port for debugging. The data is sent through MISO
at 9600bps. You'll need to use external level shifter.

I'll gladly accept any fixes and improvements. It would be the best to
put `#ifdefs` around them, so the new code could be turned on or off,
according user's wish without the need to maintain separate branches.

The name is a pun. `/tmp` is (or was) a temporary directory on Unix systems.
The device I built did not last long. The English word `tiny` and Polish
word `tani` (meaning `cheap`) sound similar.

License
-------

Released under the terms of GPL version 2

(C) Copyright 2003-2005 Wojtek Kaniewski <wojtekka@irc.pl>

