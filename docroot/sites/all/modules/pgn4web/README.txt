Drupal pgn4web module
========================================================================

This module allows users to add PGN chess games to content using the
javascript pgn4web library.

A PGN (or Portable Game Notation) file is the defacto exchange format
for chess games used on the Internet and in many chess database programs
or collections.

See http://en.wikipedia.org/wiki/Portable_Game_Notation .

Dependencies
------------------------------------------------------------------------

 * "Libraries API" module version 2.x (http://drupal.org/project/libraries).

 * "Extensible BBCode" module (http://drupal.org/project/xbbcode).

 * "pgn4web" (external) javascript library (http://pgn4web.casaschi.net).

Installation
------------------------------------------------------------------------

1. Copy and extract the "pgn4web" module into sites/all/modules. Do
   the same with the "Libraries API" and "Extensible BBCode" modules
   if needed.

2. Download the pgn4web library from http://pgn4web.casaschi.net.

   Extract pgn4web-2.57.zip (or later) into sites/all/libraries and
   rename the directory pgn4web-2.57 to pgn4web, so the library is
   located at sites/all/libaries/pgn4web/pgn4web.js.

   Note that only version 2.57 was tested to work with this module.

3. Enable the "pgn4web" module on the Administration >> Modules page.

Usage
------------------------------------------------------------------------

You can insert a PGN with one or multiple games inside '[pgn]' and
'[/pgn]' tags like this:

  [pgn parameter=value ...]
  ... chess game(s) in PGN format ...
  [/pgn]

Following tag parameters are supported:

 * layout=horizontal|vertical
 * height=auto|number (in pixels)
 * pgnData=URL of the file containing the PGN games
 * initialGame=first|last|random|number
 * initialVariation=number
 * initialHalfmove=start|end|random|comment|number
 * autoplayMode=game|loop|none
 * showMoves=figurine|text|puzzle|hidden

See http://code.google.com/p/pgn4web/wiki/BoardWidget_instructions for
some instructions on what these parameters mean and how to use them.

Example:

  [pgn height=500 initialHalfmove=16 autoplayMode=none]

  [Event "World championship"]
  [Site "Moscow URS"]
  [Date "1985.10.15"]
  [Round "16"]
  [White "Karpov"]
  [Black "Kasparov"]
  [Result "0-1"]

  1. e4 c5 2. Nf3 e6 3. d4 cxd4 4. Nxd4 Nc6 5. Nb5 d6 6. c4 Nf6 7. N1c3 a6 8.
  Na3 d5 9. cxd5 exd5 10. exd5 Nb4 11. Be2 Bc5 12. O-O O-O 13. Bf3 Bf5 14.
  Bg5 Re8 15. Qd2 b5 16. Rad1 Nd3 17. Nab1 h6 18. Bh4 b4 19. Na4 Bd6 20. Bg3
  Rc8 21. b3 g5 22. Bxd6 Qxd6 23. g3 Nd7 24. Bg2 Qf6 25. a3 a5 26. axb4 axb4
  27. Qa2 Bg6 28. d6 g4 29. Qd2 Kg7 30. f3 Qxd6 31. fxg4 Qd4+ 32. Kh1 Nf6 33.
  Rf4 Ne4 34. Qxd3 Nf2+ 35. Rxf2 Bxd3 36. Rfd2 Qe3 37. Rxd3 Rc1 38. Nb2 Qf2
  39. Nd2 Rxd1+ 40. Nxd1 Re1+ 0-1

  [/pgn]

Configuration
------------------------------------------------------------------------

1. This module uses the "Extensible BBCode" input filter to parse and
   replace the '[pgn]' tags.  You need to enable and configure that
   filter for the text formats you want to use.

   The only XBBCode tag that this module needs is the 'pgn' tag.  You
   may enable more XBBCode tags if you need them.

   To configure this, go to Administration >> Configuration >> Text
   formats.

2. Make sure the order and configuration of the input filters is
   correct:

   a. Following input filters should appear AFTER the "PGN formatter":

      - Convert line breaks into HTML

   b. Following input filters should appear BEFORE the "PGN formatter":

      - n/a

   c. If you use any HTML filtering or correcting input filters, such
      as "Limit allowed HTML tags" or "HTML Purifier", make sure the
      '<pre>' tag is not filtered out.

      Also the class-attribute on this '<pre>' element may not be
      filtered out, as well as any 'data-pgn4web-*' data-attributes.

      To be clear: if any filter removes or alters '<pre class="pgn4web">'
      the filter will not work!

   The reason '<pre>' was chosen is that it degrades well if Javascript
   is not enabled.  In this case, the PGN is show as-is which is pretty
   human-readable and can be indexed by search engines.

   When Javascript is enabled, it searches for all '<pre>' HTML-elements
   with the predefined class 'pgn4web' and converts those elements into
   playable Javascript chess boards.

3. You can alter the default look and feel of the chess board and moves
   on Administration >> Configuration >> Content Authoring >> pgn4web.

Advanced installation - minimal pgn4web library
------------------------------------------------------------------------

The pgn4web javascript library is quite big.  It also includes a lot of
files that are not used by this module.  If the space on your webserver
is limited, you can copy a minimal pgn4web library into
sites/all/libraries instead of the whole package.

To create the smaller package, run the following command from the
command-line:

  $ sh zip-custom-package.sh board-minimal

This creates a pgn4web-2.57-board-minimal.zip file.  You can use this
file instead of the whole library in sites/all/libraries.  Be careful
though: the small zip does not include pgn4web/ in its path for the
files, so you need to do:

  $ mkdir -p sites/all/libraries/pgn4web
  $ cd sites/all/libraries/pgn4web
  $ unzip pgn4web-2.57-board-minimal.zip

Known bugs and limitations
------------------------------------------------------------------------

 * Only unknown bugs at the moment.

 * If you have a lot of PGN games, the text input system of Drupal may
   time out because it takes to long to parse the text. In this case,
   the filtered text may be empty or contain "null".

   The number of games that can be used between [pgn]-tags, depends on
   memory and speed of the server.

   Solution: for big PGN database files, use the pgnData attribute
   instead, for example:

     [pgn pgnData=/sites/default/files/chess/big_file.pgn]
     <a href="/sites/default/files/chess/big_file.pgn">PGN</a>
     [/pgn]

   Note that the URL most point to a file in the same domain! If your
   site uses the URL 'example.org' a file referenced as 'www.example.org'
   will not be allowed, even if both domains refer to the same server.

   Note that the content of the [pgn]-tag will be ignored if the file
   exists.

TODO list
------------------------------------------------------------------------

I'll only add what seems useful/doable, which may be nothing at all.

 * Improve configuration page:
   - preview board.html with selected options,
   - select colors using farbtastic just like color module,
   - let users choose from some builtin color themes (similar to what
     color.module does for themes or what the board generator does
     at http://pgn4web-board.casaschi.net/board-generator.html),
   - provide additional options like (piece) font (size), etc.

 * Add a wysiwyg plugin to add PGN games and select starting position.

 * Replace board.html with real Drupal "page callback" so it can be
   use the Drupal's theme layer.  This may also make it more easy to
   specify more things in CSS instead of with URL Query parameters.

   However, the author of pgn4web said:

   " please try to resist the temptation of patching especially
     pgn4web.js and board.html, it's a minefield of side effects :-) "

   So I'm not likely to tackle this one soon, if at all.

Credits
------------------------------------------------------------------------

 * pgn4web authors - http://pgn4web.casaschi.net

Author(s) and maintainer(s)
------------------------------------------------------------------------

See http://drupal.org/project/pgn4web.
