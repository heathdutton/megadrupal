INTRODUCTION
------------

The FlickrUp module allows to upload photos to Flickr.
The module provides its own field type, widget and formatters.
It allow use Flickr photo as a field.

REQUIREMENTS
------------

1. Libraries API
   download url: https://www.drupal.org/project/libraries

2. phpFlickr
   download url: https://github.com/dan-coulter/phpflickr/archive/master.zip
   license: GNU General Public License

3. [OPTIONAL. For using Galleria formatter/plugin]
   Galleria plugin (http://galleria.io/download)
   donwload url: http://galleria.io/download
   license: MIT License

INSTALLATION
------------

1. Download and install the Libraries API module.

2. Download phpFlickr library. Unpack and rename the library directory to
   "phpflickr" and place it inside the "sites/all/libraries" directory.
   Make sure the path to the file phpFlickr.php becomes:
   "sites/all/libraries/phpflickr/phpFlickr.php".

3. Download Galleria plugin. Unpack and rename the plugin directory to
   "galleria" and place it inside the "sites/all/libraries" directory. Make
   sure that the path to the plugin files becomes:
   "sites/all/libraries/galleria/galleria.js"
   and "sites/all/libraries/galleria/galleria.min.js".

4. Get Flickr API authorization information: API Key and API Secret from
   (https://www.flickr.com/services/apps/create/apply/).
   You should create a Flickr App.
   In the Authentication Flow set the following options:
      - App Type: Web Application
      - Callback URL: http://yourdomain/flickrup/auth
   After that, return to the your Drupal site.
   Set the API Key and API Secret in module settings
   (http://yourdomain/admin/config/media/flickrup).
   Save settings.
