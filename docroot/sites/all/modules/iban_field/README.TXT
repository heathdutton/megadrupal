Description
-----------
This module adds an IBAN Field using the Drupal Field API.
The module comes shipped with a second module (Webform IBAN)
which can be used to extend the functionality to the Webform
module found at: (http://http://drupal.org/project/webform).

IBAN numbers are validated by country, length and the mod97
validation.

Requirements
------------
Drupal 7.x
Webform module (http://http://drupal.org/project/webform)
  (Webform module only required when also enabling webform_iban)

Installation
------------
1. Copy the entire webform_pager directory the Drupal 
  sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. (Optional) Edit the settings under "Administer" -> "Configuration" ->
   "IBAN" -> "IBAN"

4. Add a IBAN field to a bundle (content type) or webform.