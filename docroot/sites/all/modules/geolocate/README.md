## Geolocate module provides a more mobile friendly user geolocation service for Drupal. Most other geolocation solutions in Drupal seem to use a stored field, which doesn't make a lot of sense on the real time web. (Eg. why would you need to update the user's profile each time their location changes?)

## To access geolocate lat/long in your own module, you just need to implement HOOK_geolocate_info. This will allow your module to access the cached session variables for latitude and longitude. 

## There is also a patch available for geofield module to enable Geolocate to be used an an origin source for Views geolocative queries.

### "I would have written less code, but I didn't have enough time."

