
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
The media youtube upload module provides a page, block and fields to upload
videos directly to YouTube. It uses the Google api php client library to 
interface the YouTube API and implements the "browser upload method" so the file
never hits the Drupal file system. It saves storage and bandwidth and your users
do not need a YouTube Account allthough the option is there.
 
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

1. Add a YouTube upload field with the YouTube upload widget:
 - Permissions can be set for the allowed extensions and max uploadsize settings
   at: admin/people/permissions#module-media_youtube_upload.
 - Select which YouTube fields you want the user to be able to customize.
 - Enter default values for the YouTube fields. Disabled fields will still use
   the default value to upload videos to YouTube.
 - Select the number of values for the field.
2. Use the provided "file/add/youtube" page to upload videos to YouTube:
 - Enter settings as explained in option 1 at: "file/add/youtube/settings".
3. Use the provided block "YouTube Upload Element". This block uses the same
   settings as the page at "file/add/youtube/settings".

 
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