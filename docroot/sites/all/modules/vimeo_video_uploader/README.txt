
-- Summary -- 

Vimeo Video Uploader Module.

Module helps users to Uploads videos to vimeo on creation of content from your drupal site.


-- Installation --

* The instalation of this module is like other Drupal modules.

1. Install This module (unpacking it to your Drupal.
   [/sites/all/modules] directory if you're installing by hand).

2. Download the Library api of Vimeo from "https://github.com/vimeo/vimeo.php".
   Create a folder "vimeo-lib-api" and place the downloaded vimeo.php file to vimeo-lib-api folder.
   [/sites/all/libraries/vimeo-lib-api]


-- Configuration --

* Configuration Page.
  [admin/config/media/vimeo_video_uploader]

* Before configuring,
  Create a App at vimeo.com, visit <https://developer.vimeo.com/apps>
  Get the Created App Authenticated. (takes few days to get authenticated by vimeo.com).
  Copy the details.
   - Vimeo User Id.
   - Client ID.
   - Client Secret.
   - Access token.
   - Access token secret.

* On module configuration page enter the above copied details.
  and Select content type from which you have to upload the video to vimeo.


