
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Innitial Configuration
 * Demonstration
 * Additional Modules

INTRODUCTION
------------

Media Pane module is a module that makes it very easy for users to add media
(audio, video, images, etc.) to a site using the Panels module. 

It combines functionality from Fieldable panels pane module
(http://drupal.org/project/fieldable_panels_pane) and media module
(http://drupal.org/project/media) as well as additional configuration
functionality.

Additional features include:
  --Independent selection of view mode for pane giving a lot of flexibility,
  --Linking media content to drupal internal paths or external urls,
  --(For image content) built in overlay feature for picture annotation,


INSTALLATION
------------

Please make sure that Fieldable panels panes module
(http://drupal.org/project/fieldable_panels_pane) and Media 
(http://drupal.org/project/media) are installed, as well as all of their
dependencies before you enable the Media Pane module.

I recommend using media-7.x-2.x over media-7.x-1.x despite the current release
not being stable yet because it comes with a much better widget.

INNITIAL CONFIGURATION
----------------------

Before starting to use this module you need to configure file type display
(admin/structure/file-types). Choose the file type (eg. Images) and configure
the display for the different display modes (eg. Default -> Images -> Large,
Preview -> Image -> square_thumbnail, etc.). Now when you choose a view mode 
on the media pane, the media will be displayed as you just configured it.

DEMONSTRATION
-------------

For a demonstration of how easy it is to use the Media Pane module with the
Panels In Place Editor here is a demo video:
http://www.youtube.com/watch?v=-GNPruNYDrg (NO SOUND).

ADDITIONAL MODULES
-------

To use the media pane for rendering video and audio files on your site, you
need to install additional modules like jwplayer:
(http://drupal.org/project/jw_player).
