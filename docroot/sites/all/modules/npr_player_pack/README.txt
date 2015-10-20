====== NPR Player Pack ======
===== Overview =====
The NPR Player Pack is a pack of audio players that allow you to play audio
files pulled from the NPR Module. It works be creating several field
formatters that are designed to work the with npr_audio field created
by the NPR Fields module.

The following players are (or will be) supported:
  * Wordpress Player 2.0 (currently working)
  * JW Player (currently working)
  * JPlayer (basic functionality only)
  * XSFP Player (currently working)

Each module within the NPP is configurable per content type.
You may assign only one player per npr_audio field per content type.

===== Instructions =====

The NPP is designed to work out-of-the-box. The default of each player is to
look locally for SWF and JS files in /sites/all/libraries/player/<playername>.
If your source files are installed like this, you won't be required to
configure the module at all.

However, if your source files are hosted externally, you will have to
configure the formatter when you assign it to a content type field display.
All options are pretty self-explanatory.
