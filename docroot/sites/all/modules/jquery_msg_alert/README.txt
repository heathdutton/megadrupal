jQuery Msg Alert

This module displays Drupal's messages and Watchdog entries as a little alert at the bottom corner.
Every message sent by drupal_set_message() will be converted to a dialog alert, using jQuery Msg Alert plugin.
Watchdog entries can be displayed as status alerts and you can configure the type and severity of notification 
you want to receive as alert. You also can configure the time interval between then and they will reorganize 
themselves to be always near each other. These alerts are actually jQuery UI dialogs, so you have a huge option
of themes to customize the look and feel.

INSTALLATION:
Copy the folder to your sites/all/modules and enable jQuery Msg Alert. 

USAGE:
By default this module will pop an alert for every drupal message type (status, error and warning). 
You can configure it at Configuration page and also by permissions.

THEME:
Drupal 7 ships with a default jQuery UI theme, but if you want to change, download a new one at 
jQuery UI Themeroller or build your own:
http://jqueryui.com/themeroller/

The best usage would be to place the jQuery UI theme files (images and a CSS file) inside your Drupal's theme folder.

Attention: Don't forget to edit this module configuration to point to your new jQuery UI css file.

INFO:
This module has two permissions, one to administer the settings and the other to be able to see the dialogs alerts. 

If your user isn't seeing these alerts, make sure he has the permission.

jQuery Msg Alert's plugin page:
http://carvalhar.com/componente/jQueryMsgAlert/index.html