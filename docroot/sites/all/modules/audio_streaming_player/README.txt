
-- SUMMARY --

The Audio Streaming Player module displays a themable Audio Player in a
Drupal block witch you can assign to any Drupal Region.


-- REQUIREMENTS --

jPlayer library. 
Download it from http://www.jplayer.org/2.6.0/jQuery.jPlayer.2.6.0.zip


-- INSTALLATION --

1. Download and unpack the Libraries module directory in your modules folder
   (this will usually be "sites/all/modules/").
   Link: http://drupal.org/project/libraries
2. Download and unpack the "Audio Streaming Player" module directory in your 
   modules folder (this will usually be "sites/all/modules/").
3. Download and unpack the jPlayer plugin in "sites/all/libraries".
    Make sure the path to the plugin file becomes:
    "sites/all/libraries/jPlayer/jquery.jplayer-min.js"
   Link: http://www.jplayer.org/latest/jQuery.jPlayer.2.6.0.source.zip
4. Go to "Administer" -> "Modules" and enable the "Audio Streaming Player"
   module.


-- CONFIGURATION --

1. Go to "Configuration" => "Media" => "Audio Streaming Player" to configure the
   module
2. A form is displayed to perform the following actions:
    * Include the url of the audio you want to play.
    * Select one of the skins for the player (Circular Player, Black Player and
      Text Based Player)
    * Establish whether the player will have the auto-play feature.

-- CUSTOMIZATION --

If you will like to create a custom skin for you web site, you can do so
by selecting the Text Base theme and overwriting the css from your
theme's style sheet.

If you like to implement a persistent audio player that would continue 
playing as the user browses the pages, we recommend to implement Ajax Pages
Module
(https://www.drupal.org/project/ajax_pages) witch we have test and works 
perfectly with this "Audio Streaming Player".

-- CONTACT --

Current maintainers:
* Alain Martinez (alarez) - https://www.drupal.org/user/403881
* Kevin Coto (kevin.coto) - https://www.drupal.org/user/2960485


This project has been sponsored by:
* PARALLELDEVS
  Specialized in development, theming and customization of Drupal Sites.
  Visit http://www.paralleldevs.com for more information.
