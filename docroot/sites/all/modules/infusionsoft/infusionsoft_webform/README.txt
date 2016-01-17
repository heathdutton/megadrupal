Description
-----------
This module adds Infusionsoft API integration to your webform.
Information provided by site visitors can be used to create contacts or update
existing contacts in Infusionsoft.

Requirements
------------
Drupal 7.x
Webform module 7.x-3.x or greater
Infusionsoft API module 7.x-1.x

Installation
------------
1. Copy the entire infusionsoft directory to the Drupal sites/all/modules
   directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Edit the Infusionsoft settings under "Administer" -> "Configuration" ->
   "Web services" -> "Infusionsoft Settings"

4. Create a webform node at node/add/webform.

5. Add a mandatory email component to the webform with a "Field Key" value of
   "Email"

6. (Optional) Add more webform components with "Field Key" values which match
   Infusionsoft contact table field names.
   (See http://help.infusionsoft.com/developers/tables/contact)

7. Use the "Infusionsoft" local tab to indicate webform submissions should be
   sent to the Infusionsoft API, and (optionally) add tags and an opt-in reason.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/infusionsoft
