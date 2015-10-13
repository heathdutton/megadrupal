
CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation
 * Usage
 * Todo

AUTHOR
------
stred (http://drupal.org/user/956840)

DESCRIPTION
-----------
This module provides a field to upload video directly to YouTube and a formatter
 to display the videos. It uses the Zend Gdata library to interface the 
 YouTube API
 and implements the "browser upload method" so the file never hits the Drupal
 file system. It saves storage and bandwidth and your users don't need a 
 YouTube Account. 
 
REQUIREMENTS
------------
Zend Framework 1 (at least Minimal Package)
  http://framework.zend.com/downloads/latest


INSTALLATION
------------
1. Install first the Zend Framework Library
  download it here http://framework.zend.com/downloads/latest 
  (take the Zend Framework Minimal Package in Zend Framework 1 section)
  Directory ZendFramework-1.xxx/library/Zend will only be used.
  Extract it so the path sites/all/libraries/Zend/Loader.php 
  (or sites/yoursitename.tld/libraries/Zend/Loader.php) is available.

2. Enable the module as usual in Administer -> Site building -> Modules.

3. Configure the YouTube account that will hold the video on the module 
configuration page (admin/config/media/youtube_uploader).

3b. To get a YouTube API key,
go to https://console.developers.google.com and click "Create Project".
Next in "APIs & auth" -> "Credentials" hit the button "Create a new Key" -> "Browser key".


USAGE
-----
You can *optionally* set YouTube options for the whole site on the module 
configuration page (admin/config/media/YouTube_uploader)
or per field on each field settings.
Options are
- automatic delete the video on YouTube when the video is removed on Drupal
- set the visibility of the video on YouTube (public or unlisted)
- a YouTube category
- a default description
- default tag(s)

You can configure the formatter to display 
  the video at a predefined or custom size
  the default thumbnail following an image preset with or without the title
  
Images for the thumbnails are stored in a YouTube_upload file directory.

When the video first is uploaded, the default thumbnail doesn't exist 
on YouTube 
(YouTube needs time to parse the video and generate the thumb) 
so you can click the link "refresh data from YouTube" 
(this link will refresh the title as well) or wait and the module will 
automatically fetch the thumbnail next time it will be displayed 
on the frontend or the backend.

Users with proper permission "Edit video on YouTube" will see a link to the
 corresponding YouTube edit video page. the permission "Upload a video" allows user
 to upload a file to YouTube. 

 The module has been tested to work with the views module
 
CREDITS
-------
To YouTube module for the formatter http://drupal.org/project/youtube
To video_upload module for ideas on the code and options 
http://drupal.org/project/video_upload
 
 TODO
 ----
 Implement a progress bar for the upload (it's actually only a throbber)
 Integrate video with a taxonomy vocabulary
 Re-use video previously uploaded 
 (probably with the help of the FileField Sources module)
 
 
