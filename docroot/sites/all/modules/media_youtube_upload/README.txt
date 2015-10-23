
CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation
 * Usage
 * Todo

AUTHOR
------
alexverb (https://www.drupal.org/u/alexverb)

DESCRIPTION
-----------
The Media: YouTube upload module extends the file field with a widget that can
upload videos directly to a preconfigured YouTube account. Aside from the field,
it is also possible to make use of a page, block or media widget for uploading
your videos. The biggest advantage of using this module is that the video file
never hits your server and saves a lot of bandwidth, while still maintaining
control over your videos through a YouTube file in your Drupal installation.
 
REQUIREMENTS
------------
Google api php client library
  https://github.com/google/google-api-php-client
A defined Google Application
  https://console.developers.google.com/project
Google CORS Upload file
  https://raw.githubusercontent.com/youtube/api-samples/master/javascript/cors_upload.js


INSTALLATION
------------
Automatic

Using "drush en -y media_youtube_upload" will download/enable the module and the
libraries. The libraries will be created on sites/all/libraries

Use "drush ytu-libraries" to only (re)install them. Pass the directory as a
parameter to use a different libraries folder:
"drush ytu-libraries sites/mysite/libraries".

Manual

1. Enable the module as usual in Administer -> Site building -> Modules.

2. Install 
- the Google API php client library
  download it here https://github.com/google/google-api-php-client
  Extract it so the path <br />sites/all/libraries/google-api-php-client/src/Google/autoload.php
  or sites/[domain]/libraries/google-api-php-client/src/Google/autoload.php is 
  available.
  
- the Google CORS Upload file
  download it here https://raw.githubusercontent.com/youtube/api-samples/master/javascript/cors_upload.js
  Extract it so the path sites/all/libraries/google-api-cors-upload/cors_upload.js
  or sites/[domain]/libraries/google-api-cors-upload/cors_upload.js is available.
  
CONFIGURATION
-------------
Configure the Google App that will hold the video on the module configuration
page at: admin/config/media/media_youtube_upload.

1. To get credentials,
 - Go to https://console.developers.google.com/project. 
 - Next in "APIs & auth" -> "Credentials" -> "OAuth".
 - Click "Create new Client ID"
 - Then under "Application Type" select "Web Application".
For the redirect uri use http://yourdomain/media_youtube_upload/oauth2callback

2. You will next have to authorise your application by clicking on a link on the
module settings page. This should be done only one time. The required token will
be automatically refreshed for any additional request.


USAGE
-----

1. Add a filefield with the YouTube upload widget:
 - Permissions can be set for the allowed extensions and max uploadsize settings
   at: admin/people/permissions#module-media_youtube_upload.
 - Select which YouTube fields you want the user to be able to customize.
 - Enter default values for the YouTube fields. Disabled fields will still use
   the default value to upload videos to YouTube.
 - Select the number of values for the field.
2. Add a filefield with the Media widget:
 - Configurable at "admin/config/media/media_youtube_upload/media_settings".
 - These settings will be used on all media widgets with the youtube upload.
3. Use the upload page "file/add/youtube":
 - Configurable at "admin/config/media/media_youtube_upload/page_settings". 
 - Below in the settings you can configure redirection after successful upload.
4. The YouTube upload block:
 - Configurable at "admin/config/media/media_youtube_upload/block_settings".
 - Below in the settings you can configure redirection after successful upload.

 
CREDITS
-------
To media_youtube module for the formatter:
  http://drupal.org/project/media_youtube
To youtube_uploader module for ideas on the code and options. A lot of code has
been re-used, but implemented to draw the power of the file_entity module:
  http://drupal.org/project/youtube_uploader

TODO
----
Check issue queue at: https://www.drupal.org/project/issues/media_youtube_upload