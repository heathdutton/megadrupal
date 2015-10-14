This module harnesses the power of webform and Highrise API implemented over HTTP using (GET/POST/PUT/DELETE) via XML. Highrise is a web based tool by 37signals.

This module can be used to integrate your "Contact Us" webform on the drupal web site with Highrise. Each time a user submits the contact us form, a new contact is added to the Highrise account.

Requirements:
* PHP 5.1.x + compiled with curl extension
* Drupal 6.16+
* Highrise-PHP-Api
  * Download the HighriseAPI.class.php file from https://github.com/ignaciovazquez/Highrise-PHP-Api/tree/master/lib
  * Place this file in the lib folder of the highrise module.

Dependencies:
The Highrise module requires the Webform 6.x-3.11 module

Features:

* The Highrise module uses the Highrise API to add contacts to Highrise using Drupal webforms.
* You can define a tag to be associated with your highrise contact.
* The url of the web page from where the user was created is appended to the background field in the highrise contact.

Configuration:

* Install the Highrise module.
* Enter the Highrise account name and authentication token on the settings page(admin/config/services/highrise).
  * NOTE: Enter only the account name in the settings and not the full URL. e.g enter only <accountname> for a URL like https://<accountname>.highrisehq.com
* Enter the tag that you want to be associated with the new contacts on the settings page..
* Enable the webforms for Highrise integration by creating mappings on the "Create Mapping" page(admin/config/services/highrise/create).
* Once a webform is mapped with the Highrise fields, a new contact will be added to the Highrise contacts each time the webform is submitted.
