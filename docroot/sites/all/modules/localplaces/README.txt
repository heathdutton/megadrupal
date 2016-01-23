
Module: Local Places
Author: Tracey Spencer <http://drupal.org/user/2604088>


Description
===========
Displays local places, using the Google Places API. Returns a maximum of 
60 places, as that is the maximum Google will allow.

Requirements
============

* Google Places API Key - see 
https://developers.google.com/places/documentation/#Authentication.


Installation
============
* Copy the 'local_places' module directory in to your Drupal
sites/all/modules directory as usual.

Usage
=====
In the admin/config/content/local-places enter your Google 
Places API Key.

Browse to /local-places/ and you should see some local places listed.

If you see "Sorry, no results found." the search has not returned any 
results. This could be becuase you entered an incorrect API key. It 
could also be because there are no places of the type you entered 
within the radius of your location. Try adding more types or 
increasing your radius.

You can change the type of place, the center point and the radius from 
the centre point in the configuration page.

Configuration Settings
=================
Title - The Title that appears on the first page (/local-places/) and
on the second and third results pages.

Description - Appears above the listings and map on the home page and
on the second and third results pages.

Google Places Base URL - The URL for Google Places API calls. DO NOT 
CHANGE THIS unles absolutely necesary. It has been made changable 
for future proofing, but should not require changing.

Google Maps Base URL - The URL to append the locations to, to display a
Google Map. DO NOT CHANGE THIS unles absolutely necesary. It has been
made changable for future proofing, but should not require changing.

Google Places API Key - This is vital. If you do not enter a valid key, no
places will be shown on your website.

Location - The co-ordinates to search from, in the format 
[latitude],[longitude]. A pointer will be diplayed at this point on the 
map and the map will be centered on this point. All searches will be 
carried out from this point. Defaults to 51.5073,0.1276 (Charing Cross, 
London).

Radius - The radius in meters, from your search start point (Location, above). 
Maximum radius is 50000m. Defaults to 50000m.

Place Types - The types of place you wish to search for, separated by a pipe
(|). See https://developers.google.com/places/documentation/supported_types for 
supported types. Defaults to cafe|resaurant.

Large Map Size - The size of the large map that apears on the results pages, 
with all places marked on it. In the format [width]x[height]. Defaults to 
600x600.

Small Map Size - The size of the small map that apears on the details page, with 
the individual place marked on it. In the format [width]x[height]. Defaults to 
600x600.

Large Map Zoom - The zoom level of the large map that apears on the 
results pages. 1 is furthest zoomed out and 22 closest possible zoom in.
Defaults to 15.

Small Map Zoom - The zoom level of the small map that apears on the details 
pages. 1 is furthest zoomed out and 22 closest possible zoom in.
Defaults to 16.
