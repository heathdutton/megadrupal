Projekktor README
=================

This module adds the Projekktor HTML5 Video Player to your website. At this
point it can be used in several ways:
1) As a field formatter for files or file like entities. Currently contains
   formatters for the file, link, media, media youtube, node reference, video,
   and youtube field modules. If more than one file is added to a field it will
   create a playlist.
2) It can be deployed to add the library site wide on all pages. With this
   option videos can be added by embeding <video> tags into content, but they
   must include the class "projekktor" (ie. <video class="projekktor"></video>).
   There are downsides to this approach. It slow down your site and the theme
   in your sitewide setting will override any other theme you might want to use.
3) It can be used as an API for adding videos. This is based on a very simple
   php function. Simply call projekktor_add() in php to load the library.
4) It has a also has views integration. Use the projekktor view format to
   make a playlist from your view items.  Or use an unformatted list view with
   the projekktor field formatter to create a separate player for each video.

Basic Installation
------------------
Make sure you have the dependencies installed (libraries and file).
Grab the projekktor library from their home page http://www.projekktor.com
Unzip the library into your sites/<whatever>/libraries directory.
The js file should be at: sites/<whatever>/libraries/projekktor.min-[version#].js
The theme file in the projekktor library needs to be writable by the webserver.
Grab the jCarousel library at: http://sorgalla.com/jcarousel/
Unzip the jcarousel library into your sites/<whatever>/libraries directory.
The js file should be located at: 
sites/<whatever>/libraries/jquery.jcarousel/lib/jquery.jcarousel.min.js
Place the module in your sites/<whatever>/modules directory an enable it.
Configure the module at admin/config/projekktor to create video player presets.
The full list of player options is at:
http://www.projekktor.com/docs/configuration#config

Using the field formatters
--------------------------
You can select Projekktor: HTML5 Video Player in the manage display settings
for any content that has an applicable field. The menu here will provide preset
and theme options for that field. You may have more than one field with
Projekktor as the formatter, and it will create multiple players per page.
Using a the node reference field you can reference files from multiple nodes, 
multiple fields, and multiple content types as long as it is a file entity.

Using the field collection
--------------------------
Set up a field collection that contains three fields. It should have a text
field an image field, and a file field. Set the file field for an unlimited
number of files. Each field collection represents a playlist item. The text
field becomes the title, the image field becomes the poster, and the files
become the videos. So you can upload videos in several different formats for
each playlist item.

Using views
-----------
Use the projekktor "view" display format when creating your view if you want
to create a playlist. The "field" format can be set to anything.
The player will recognize anything of the field type "file" as the media item.
The first field type "image" in the view will be used for the thumbnail.
If it's a media: youtube file the thumbnail will be generated using the
media: youtube thumbnail wrapper.
The video title is the filename. 

Using the formatter with the media module
-----------------------------------------
Basically you can just choose projekktor in your media file display settings.
These are weighted, so if you want projekktor prioritized it needs to be near
the top of the list.

Loading projekktor site wide
----------------------------
This option is available in the configuration form. Any content added to pages
within <video> tags and having the class projekktor will turn into a player.
I would discourage using this option for many reasons, but it is available.

Using the API
-------------
Projekktor can be easily loaded onto any page using php
Just call the projekktor_add function:
  projekktor_add($player_id, $preset_name, $theme_name, $playlist);

All the function variables are optional. They are discussed below.

/**
 * This function attaches the required JavaScripts and settings.
 *
 * It forms the basis for the Projekktor API.
 * 
 * @param string $player_id
 *   A string to use for the unique player instance id.
 *   Should be a unique css id selector in a video tag like 'projekktor-49'.
 *   Defaults to 'video'.
 * @param string $preset_name
 *   A string to use for the name of a preset options array.
 *   Defaults to 'default'.
 * @param string $theme_name
 *   A string to use for the name of a projekktor theme installed on the site.
 *   Defaults to 'default'.
 * @param array $playlist
 *   An indexed array of playlist items containing:
 *   - delta: A mixed array that denotes a playlist item containing:
 *     - key: An associative array for a playlist item file or url containing:
 *       - src: A string to use for the source of a file or url to a video.
 *       - type: A string to use for the file mime type.
 *     - config: (optional) An associative array of per playlist item options
 *       containing.
 *       - title: (optional) The string to use for the video title.
 *   The playlist is passed in the javascript array.
 *   Each delta may contain an indexed array of multiple file types (ie. mp4,
 *   webm, ogv) for HTML5 cross browser support of each playlist item.
 *   Numerous options for each playlist item can be passed via the config array.
 * 
 * @see http://www.projekktor.com/docs/playlists
 */

Adding projekktor controllbar themes
------------------------------------
Additional themes are available for download at http://www.projekktor.com
Add the themes to the theme folder in the projekktor library.
!Important: In the addon themes rename or copy the style.css file as
projekktor.style.css. Drupal won't load a file named style.css and the module
won't recognize it. New themes will show up in your forms automatically after
clearing the site cache.

Adding jCarousel skins
----------------------
Add these to: sites/<whatever>/libraries/jquery.jcarousel/skins/
The module will detect them automatically after clearing your cache.

To anyone so inclined I would encourage you to look into creating custom
playlists using rss, xml, or json. The way the module is designed all content is
added to the player via the javascript array (unless it's your embeded tags).
So theoretically it should be able to "play" almost anything. All prokekktor
needs is the file and to know the mime type (ie text/xml). See:
http://www.projekktor.com/docs/playlists

Please refer all bug reports and feature requests to the project page:
http://drupal.org/project/projekktor
