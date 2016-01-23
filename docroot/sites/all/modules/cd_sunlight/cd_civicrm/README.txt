


*** CIVICRM CONGRESSIONAL DISTRICTS ***



** ABOUT **

This module provides a way to automatically get congressional district information about your
contacts into CiviCRM using the Sunlight Congress API.
  http://services.sunlightlabs.com/docs/Sunlight_Congress_API/



** INSTALL **

1)Enable the CiviCRM Congressional Districts module at
    admin/build/modules

2)Navigate to
    /admin/reports/status
  To get instructions on how to configure your settings.php to use the module.  Specifically you
  must have prefixes for several CiviCRM tables in settings.php.  

3)Link with CiviCRM fields at
    /admin/settings/cd_sunlight/cd_civicrm
  Follow the links to create new fields in CiviCRM.

4)Return to the status report to see if the module is reporting any problems
    /admin/reports/status

5)If you wish you can enqueue all CiviCRM contacts to have their congressional district retrieved
  during the next cron run at
    /admin/settings/cd_sunlight/batch

6)Edit a contact and change the postal code to enqueue the contact for lookup.  The Congressional
  District will be retrieved the next time cron is run.  When a contact is edited from a CiviCRM 
  profile (ex. /user/12345/edit/[civicrm profile name]) the congressional district will be 
  immediately retrieved.



** VIEWS INTEGRATION **

You can find the default view called "Congress" view is extended with new displays to limit the
list of congress to only those members in the current user's district.



** USING THE API **

There are some API functions for your use in other modules.  See the
docbook comments for more info.

cd_civicrm_contact_get_cd($contact_id)
cd_civicrm_contact_get_state($contact_id)
cd_civicrm_contact_get_district($contact_id)
cd_civicrm_user_get_cd($uid)
cd_civicrm_user_get_state($uid)
cd_civicrm_user_get_district($uid)
cd_civicrm_contact_link($text, $contact_id)



** DEVELOPED BY **

Advomatic LLC
http://advomatic.com



** SPONSORED BY **

Democrats.com
http://democrats.com
