Firstly, thank you for your interest in the VChess module!  

This is under very active development at the moment, so if you need any help, 
please feel free to contact me, preferably by raising a VChess issue: 
- http://drupal.org/project/issues/vchess

or (if you are absolutely sure nobody else will ever be interested in what you have to say), 
by contacting me via my profile page:
- http://drupal.org/user/620770

In general, if you are having a problem with VChess, someone else will likely 
have the same issue, so please use the project issue queue.


CURRENT FEATURES
* user-friendly move interface
* performs move validation; illegal moves are not allowed
* allows castling
* handles check
* handles checkmate
* handles pawn promotion, with choice of promoted piece
* resign button
* displays game scoresheet in full algebraic notation (e.g. "e4", "Nf3", "Bxh8"), including castling (e.g. "O-O"), move disambiguation (e.g. "N2f3"), pawn promotion (e.g. "h8=Q"), check and checkmate (e.g. "Qxf7+", "Rd8#")
* simple user statistics (won, drawn, lost, rating, most recent rating change)


INSTALLATION
1) VChess Download 
I recommend you download the most recent development version of the VChess
files (7.x-1.x-dev) from the VChess project page:
- - http://drupal.org/project/vchess
You might be tempted to install the "Recommended release", but at the moment, 
that will just give you old code, since I don't build a "Recommended release" very often.  
The dev files should be increasingly stable, as I am now regularly adding tests to ensure 
there is no regression.
 
2) File placement
Unzip the files and put them in the usual place for modules (e.g. for VChess this is probably 
"sites --> all --> modules --> vchess")

3) Creation of module database tables
This is currently the most difficult part of installation, since Drupal core has some
problems for the ENUM and Timestamp datatypes.  I describe below how to fix the particular
files in the main part of Drupal.  These files are not part of VChess, but they are part of 
the rest of your Drupal installation, and with a little care you should be able to find them
and make the small changes required.  

a) Fix for ENUM datatype (in Drupal core)

Unfortunately, currently in Drupal core, the ENUM datatype for MySQL is not 
supported.  I wanted to use it, so I have found out where to fix this problem
in Drupal core, but currently you will need to follow the following post to
patch the core files yourself:
- http://drupal.org/node/1464354 

b) Fix for Timestamp datatype (in Drupal core)

Unfortunately, there is also not proper support for the Timestamp datatype in
Drupal core, so you will need to follow the instructions to patch the core
files here: 
- http://drupal.org/node/1466122

An alternative option for the database, but which is less recommended, is that  
if you know how to create database tables, look in the vchess.install file and  
comment out the ENUM and Timestamp fields, install the module and then add the 
extra fields by hand.

4) Enable the VChess module
Go to the drupal "Administration -> Modules" page and look for VChess.  Select the module
and do "Save Configuration".  If all goes well, the module should install without error.
If all does not go well, make sure you have followed the above steps for "Creation of module
database tables"


FIX PATH
In the file render.inc, there is a hardcoded path which you may well need to change to
make the module work in your environment:
  function vchess_render_board($board, $player, $active, $gid) {
    $path = "http://localhost" . base_path() . drupal_get_path('module', 'vchess');
This is not ideal and will hopefully be fixed at some stage.


SET PERMISSIONS
Go to the permissions page to set the VChess permissions
e.g. http://localhost/chess_drupal_7/?q=admin/people/permissions
Make sure you only give the "reset games" permission to the Administrator!


USAGE
The following are the main pages of VChess:

1) Main VChess page 
e.g. http://localhost/chess_drupal_7/?q=vchess/main
This gives a list of current games and enables you to create a new game

2) New random game
e.g. http://localhost/chess_drupal_7/?q=vchess/random_game_form
Allows you to start a new game against a named opponent (you can event play against yourself, 
though the way the board flips to the other side is a little disconcerting at first)

3) New opponent game
e.g. http://localhost/chess_drupal_7/?q=vchess/opponent_game_form
Allows you to start a new game against a named opponent (you can event play against yourself, 
though the way the board flips to the other side is a little disconcerting at first)

4) Players
e.g. http://localhost/chess_drupal_7/?q=vchess/players
Provides a list of all players and the following basic statistics:
* Played
* Won
* Lost
* Drawn
* Rating
* Rating change (most recent)

5) Reset games
http://localhost/chess_drupal_7/?q=vchess/reset_games
WARNING: This resets the complete games database!!
This function is for testing the VChess functionality and being able to 
start with a clean games database when any testing is complete.  It requires
the permission "reset games" to be allowed to use it.  (See above in the SET
PERMISSIONS section).


FEEDBACK
Please let me have your feedback, bugs, experience with it (good or bad)!
Happy chess playing!

drupalshrek, 13th April 2012
