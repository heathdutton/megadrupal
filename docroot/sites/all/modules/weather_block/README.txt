INSTALLATION INSTRUCTIONS
-------------------------

1) Install the module as usual (installation might take a moment as it will import the weather locations from the text files into the taxonomies).

2) Install and configure the Smart IP module from http://drupal.org/project/smart_ip

2) Check the default settings at the configuration page for Weather block at /admin/config/weather_block

3) Visit the weather locations management page or the taxonomy edit pages for maintaining the weather locations in each taxonomy; you can also use the weather locations manager at /admin/config/weather_block/taxonomies

4) If necessary, modify the weather locations available at the weather locations (local) and weather locations (world) taxonomies. Good services for finding the weather ID's:

   Yahoo Weather -> http://woeid.rosselliot.co.nz
   Weather.com -> http://aspnetresources.com/tools/WeatherIdResolver (not all cities have a weather station so use the nearest location)
   World Weather Online -> No need for a locating service, just add the weather location ID in the format "City, Country"

After installation, the user functionality of the module can be found at /weather and /weather_world and of course at the weather block as well :)
