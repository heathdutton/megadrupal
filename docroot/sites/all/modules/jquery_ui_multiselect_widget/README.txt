README.txt
----------
Adds Eric Hynds jQuery UI MultiSelect Widget (http://www.erichynds.com/jquery/jquery-ui-multiselect-widget/ ) to select fields (optional multiselect only) in Drupal.
Replaces all selects with attribute "multiselect" by this better behaviour.

JavaScript API
------------
You may get the created multiselect object from:
Drupal.behaviors.jquery_ui_multiselect_widget.multiselect;

Furthermore you may of course implement your own multiselect by calling
$('selector').multiselect();
or even:
$('selector').multiselect().multiselectfilter();

DEPENDENCIES
------------
- none -

INSTALLATION
------------
0.    Download and extract Eric Hynds's "jQuery UI MultiSelect Widget" jQuery Plugin to the "libraries" folder (sites/all/libraries), named "jquery-ui-multiselect-widget". This folder for example should contain a "src" folder as direct subfolder.
      Hint: Simply "Download ZIP" (Button) from the projects GitHub Page: https://github.com/ehynds/jquery-ui-multiselect-widget and remove the "...-master" suffix.
      Example file structure:
        sites/all/libraries/jquery-ui-multiselect-widget/src/jquery.multiselect.js
        sites/all/libraries/jquery-ui-multiselect-widget/src/jquery.multiselect.filter.js
        sites/all/libraries/jquery-ui-multiselect-widget/jquery.multiselect.css
        sites/all/libraries/jquery-ui-multiselect-widget/jquery.multiselect.filter.css
        ...
1.    Download and enable this module
2.    (Already enabled and working with default settings now!)
3.    Go to {YourURL}/admin/config/user-interface/jquery_ui_multiselect_widget and configure it
4.    Done! Happy Using!
5.    Visit our Websites. Please.


AUTHOR/MAINTAINER
-----------------
- Julian Pustkuchen (http://Julian.Pustkuchen.com)
- Development proudly sponsored by:
    webks: websolutions kept simple (http://www.webks.de)
      and
    DROWL: Drupalbasierte Lösungen aus Ostwestfalen-Lippe (http://www.drowl.de)