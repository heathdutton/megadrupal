This module provides a content type called "Chess PGN Game" for displaying
chess games in pgn format. It users ltpgnvier:

http://www.lutanho.net/

Installation Instructions
=========================

1) Download ltpgnviewer from http://www.lutanho.net/ and put into the directory
   sites/all/libraries/ltpgnviewer

2) Install the libaries api module ( http://drupal.org/project/libraries )

3) Install the ltpgn module

Usage
=====

To test, create content of type Chess PGN Game. Select a pgn file to upload,
e.g. ltpgn/examples/kkgame.pgn. Click on the arrows to play through the game.

Permissions
===========

The module has 2 permissions:
1) chess pgn admin - allows someone to change the admin settings of the module.
2) chess pgn user - allows a user to create and edit pgn games.
