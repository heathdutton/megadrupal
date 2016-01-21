This module integrates Etherpad Lite into Drupal 7 WYSIWYG module.

= Installation instructions =
 * change directory to the root of the drupal installation containing
   index.php. Then execute following commands:

cd sites/all
mkdir -p libraries
cd libraries
git clone git://github.com/TomNomNom/etherpad-lite-client.git
git clone git://github.com/ether/etherpad-lite-jquery-plugin.git
drush dl etherpad wysiwyg libraries
drush en etherpad

 * go to configuration of your drupal installation (?q=admin/config/content/etherpad-lite)
 * check that the etherpad http url is correct
   Note: make sure that for production websites you should use a URL like
   http://mysite.com:9001 rather than http://localhost. Otherwise you'll get
   "You do not have permission to access this pad".
 * paste the APIKEY.TXT of your etherpad installation into the Etherpad API field
 * click test to see if connection could be established (should show a big green success)
 * then save configuration

VERY IMPORTANT (hopefully temporary hack):
 * open settings.json from your etherpad installation
 * change the line containing "defaultPadText" : "Welcome ..."
   to "defaultPadText" : ""

 * congratulations, you have configured etherpad-lite integration with Drupal

Next steps:
 * go to configuration of you wysiwyg (?q=admin/config/content/wysiwyg)
 * add etherpad to some of the input formats
 * to enable several users working on the same node in the same time you should use the
   Local Editable formatter for the text fields i.e. go to the Manage Display tab on your
   content type (e.g. ?q=admin/structure/types/manage/article/display) and choose
   Local Editable for the text field you want to have etherpad-lite enabled
 * remember that in Etherpad-Lite one author can have only one instance of the pad.
