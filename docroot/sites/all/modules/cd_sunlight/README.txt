


*** SUNLIGHT CONGRESSIONAL DISTRICTS ***



** ABOUT **

This module 
- Fetches data about the current members of Congress using the Sunlight Congress API.
    http://services.sunlightlabs.com/docs/Sunlight_Congress_API/
- Provides an API for interfacing with Sunlight.
- Exposes the members of Congress to the Views module.
- With one of the contact integration modules (currently just CiviCRM Congressional Districts
  module) the congressional district of your contacts can be automatically determined.  
- Provides a form to search for members of Congress by ZIP.


** INSTALL **

1)Unpack the modules files just like any other Drupal module.  Probably at something like:
    sites/all/modules/contrib/cd_sunlight

2)Enable the Sunlight Congressional Districts module at
    admin/build/modules
  Do not enable CiviCRM Congressional Districts module at this time.

3)Configure the module at
    /admin/settings/cd_sunlight
  Specifically the Sunlight API key.

4)Visit the status report to see if the module is reporting any problems.
    /admin/reports/status

5)Proceed to the cd_civicrm/README.txt if you wish to integrate with CiviCRM.



** VIEWS INTEGRATION **

To show information about members of Congress on your site you need to install the Views module.
  http://drupal.org/project/views
You can find the default view called "Congress" at
  /admin/build/views
You can either customize it or create a new view of the type "Congress".



** USING THE API **

The API should only be used in situations where Views cannot be used.  In the future the Views 
integration will likely see more tending than the API.  Here are the API functions.  See the
docbook comments for more info.

cd_sunlight_cd_parse_state($cd)
cd_sunlight_cd_parse_district($cd)
cd_sunlight_api_lookup_contact($contact)
cd_sunlight_api_lookup($method, $params, &$error_message)
cd_sunlight_contact_enqueue($storage_system, $contact_id)
cd_sunlight_contact_dequeue($storage_system, $contact_id)
cd_sunlight_bioguide_image_url($bioguide_id)



** DEVELOPED BY **

Advomatic LLC
http://advomatic.com



** SPONSORED BY **

Democrats.com
http://democrats.com
