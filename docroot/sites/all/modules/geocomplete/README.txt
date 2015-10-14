The module integrates Geocomplete jQuery plugin with Search API Location Views module.

It provides a new input option for exposed location field. When the option is selected,
address suggestions are shown as autocomplete list, when users starts typing an address,
and geocode data for selected address submitted alongside with the form.


Installation
------------


* Download the Geocomplete jQuery plugin from http://ubilabs.github.io/geocomplete/ to
  geocomplete sub-directory in libraries directory (e.g. sites/all/libraries/geocomplete)
  and unpack it so that jquery.geocomplete.js file could be found in the root of aforementioned
  geocomplete directory.

* Install the module as normal drupal module.


Usage
-----

Add a filter to a view for a field indexed as search_api_location "Latitude/Longitude" field. In
filter setting form select "Geocomplete" as input option. Optionally select server-side geocoding
method as fallback behaviour and additional fields to collect by Geocomplete plugin.
