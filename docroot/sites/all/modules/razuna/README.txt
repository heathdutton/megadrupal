
RAZUNA
http://drupal.org/project/razuna
====================================

DESCRIPTION
-----------
Razuna is an basic image/file uploader and browser using Razuna cloud server
using Razuna API 2 library.
Razuna can easily be integrated into any web application that needs a file
uploader & browser on/from clouds.
Currently it has very basic functionality of the various functionality
available with Razuna API 2.
See INTEGRATION METHODS for more information.

FEATURES
-----------
- Basic file operations: upload, delete
- Image(jpg, png, gif) operations: resize, create thumbnails, preview
- Various configurable options available on Razuna ui and also through Razuna
  API 2.

INSTALLATION
-----------
1) Copy Razuna directory to your modules directory.
2) Enable the module at module administration page.
3) Configure Razuna by clicking on the settings icon next to the module name.
    or go on to Configuration->Media->Configure Razuna.
4) Enter all the required details you got after registering to Razuna.
5) Make sure that your curl is enabled before using this module.
6) You should have account with Razuna,
   https://secure.razuna.com/signup (30 days trial)
7) Please enable your cURL library (http://php.net/manual/en/curl.setup.php).
8) You can download razuna library (API 2)
   https://github.com/razuna/razuna-php-library.
   Place that in 'sites/all/libraries/razuna/' folder
   (sites/all/libraries/razuna/Razuna.class.php).

CONFIGURATION
--------------
1) Razuna Hostname
   The host name which you have entered at the time of registration on Razuna
   e.g. (http://xyz.razuna.com/)
2) Razuna API Key
   API key is associated with the User on your Razuna Host.
   To find the API key please follow the below steps
    - login to your host (http://xyz.razuna.com/)
    - click on your name on the top right corner
    - click on the administration from the drop down
    - you'll be able to see the users currently present in your host
    - click on the user whose API key is needed
    - one pop-up will be shown, select the API key tab
3) Razuna Folder Id
    For selecting the Folder from Razuna server in which files will be placed.
    This will be overridden in future.
    Plan is to provide a functionality of selecting Folder from Razuna server.
    To find the Folder ID please follow the below steps
    - login to your host (http://xyz.razuna.com/)
    - you'll be able to see the list of folders in the left side
    - click on the folder whose Folder ID is needed
    - click on the Folder Sharing & Settings link on the right side
    - a pop-up will be shown having Folder Id below labels and description
4) Razuna Email Id
    A notification will be sent on this email id on file upload.

SCOPE
-----------
- This module is developed & tested with Razuna API 2 library.
- File/Image is getting stored on drupal environment also( default
  functionality of drupal) for the fields associated with razuna, ideally which
  should not be the case.
- Seems not working with txt files.

FREQUENTLY FACED ISSUES
-----------
- Undefined function curl_init()
Your Curl is not enabled properly.

- Razuna API Calling Failed.
If you have set every thing right and you are still getting this error.
Check Firewall settings. Firewall seems tobe blocking the curl requests.

TODO
-----------
- Make it work for other formats (pdf, avi, mp4, mpeg, doc, docx, wmv etc.)
- Integrate various functionality like create folder, create user, use
  renditions etc. which are available through Razuna API
- Get it worked with txt files as well.
