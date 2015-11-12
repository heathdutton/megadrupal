CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers

INTRODUCTION
------------
This module provide field formatter to display file or image using ViewerJS library.
ViewerJS must be the easiest way to use presentations, spreadsheets, PDF's and other documents on your website or blog without any external dependencies.
It can work offline, doesn't depend on any third party services or might be (perceived as) invasive to the privacy of your users.
Supported files : pdf, odt, odp, ods, fodt, jpg, jpeg, png, gif

REQUIREMENTS
------------
This module requires the following modules and libraries:
 - Libraries (http://drupal.org/project/libraries)
 - ViewerJS (https://github.com/Strae/Viewer.js/archive/master.zip)

INSTALLATION VIA DRUSH
------------
A Drush command is provided in the latest versions for easy installation of the ViewerJS plugin.
    drush viewerjs-plugin
The command will download the plugin and unpack it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not recommended unless you know what you are doing.

INSTALLATION
------------
 * Download the ViewerJS library and extract it to sites/all/libraries/viewerjs
   (so that the viewer.js will be located at
   http://example.com/sites/all/libraries/viewerjs/viewer.js)
 * Download and extract the ViewerJS module to /sites/all/modules.

CONFIGURATION
-------------
 * Go to your Structure > Content Types > [your-content-type] > Manage display;
 * Create or use existing image or file field;
 * Choose ViewerJs Default format and configure options.
