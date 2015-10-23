-- SUMMARY --

This module allow to share the selected text in social networks like twitter,
google plus and linkedin.

It has an API so that developers can to add their custom share selection links.

-- REQUIREMENTS --

* Token - https://www.drupal.org/project/token.

-- INSTALLATION --

* Place the entire Share Selection directory into your Drupal sites
	modules directory (eg: sites/all/modules/contrib).
* Enable this module by navigating to: Administration > Modules.

-- CONFIGURATION --

* The Share Selection settings are found in admin/config/services/share-selection/general.
* Every service can have its own options, this configuration is in admin/config/services/share-selection/services.
* For Google plus to work you need to create an app in https://console.developers.google.com and set the client id.
* For Linkedin to work you need to create an app in https://developer.linkedin.com/ and set the apikey.

-- STYLING AND CUSTOMIZATION--

* It creates a <div> for every share selection link with a class and id, It is very easy to style.
* Developers can add more share selection links, see share_selection.api.php
