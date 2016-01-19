1. Enable the module as usual
2. Go to admin/structure/services
3. Create a new endpoint, or edit an existing one
4. Check the box next to "services_addressfield", this will check all sub boxes
5. Click Save
6. Flush all of Drupal's caches

USAGE
=====

# get all countries
POST: ?q=my_endpoint/services_addressfield/country_get_list.json

# get address format for a given country code
POST: ?q=my_endpoint/services_addressfield/get_address_format.json
data to send (example): { country_code: 'US' }

# get administrative areas a given country code
POST: ?q=my_endpoint/services_addressfield/get_administrative_areas.json
data to send (example): { country_code: 'CA' }

# get address format and administrative areas for a given country code
POST: ?q=my_endpoint/services_addressfield/get_address_format_and_administrative_areas.json
data to send (example): { country_code: 'GB' }

