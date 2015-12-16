

ADDING MENU BLOCKS
------------------
To add new google weather block, you need to first add a location. Go to admin/config/google_weather/locations to do this. 

Google weather API works on a location search basis so type your search string in first, e.g. 'London' or (more specific) 'London, United Kingdom'. Then you can choose a display name for this location, this will be the default title of the block. The save as block checkbox is ticked as default as this is the common behaviour. Sae your block and you will now see a new location in your list.

Go to the Drupal blocks administration page; you should now have a new block available called 'Google weather: NAME_OF_YOUR_LOCATION'. Put this where you like and you're done!

To override the markup of the block, copy the google_weather_block.tpl.php file from the module directory and put in your theme.
