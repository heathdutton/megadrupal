# Google Places for Drupal

VERY IMPORTANT
==============

Before using this module, make sure you understand the terms and conditions
Google has set forth for its Places data and how you can use it:

https://developers.google.com/places/policies#pre-fetching_caching_or_storage_of_content
https://developers.google.com/maps/terms#section_10_1_3

INSTALLATION
============

0. Download the module to sites/all/modules as usual.

1. Install this library:

https://github.com/anthony-mills/Google-Places---PHP-

Download its zip file, and extract it to:

sites/all/libraries/google_places_php

So this file and others exist:

sites/all/libraries/google_places_php/googlePlaces.php

2. Now enable the Google Places module as usual.

3. Go to admin/config/services/gp and enter your Google Places API Key.

4. Add a non-required address field to your content type. Be sure to select no
countries so they are all available, and check the
'Address form (country-specific)' and 'Make all fields optional' checkboxes
under the 'Format handlers' settings.

5. Add a non-required geo field to your content type, use the
'Latitude / Longitude' widget.

6. Add a non-required telephone field to your content type.

7. Add 3 non-required url fields to your content type to hold the place's
website URL, Google Plus URL and Google Maps Icon URL.

8. Add a non-required, unlimited value taxonomy term reference field to hold
onto the place's types. It is recommend to create a new vocabulary for this,
e.g. "Google Places Types".

9. Add a non-required integer field to your content type.

10. Add a non-required URL text field with a length of 512 to your content type
to hold onto the place photo reference(s). You can specify a single value, multi
value or unlimited value (10 max) field here to decide how many photo(s) you'd
like to save a reference to.

11. IMPORTANT: Go to 'Manage Display' for your content type, and hide all of
these fields from public diplay, unless they are displayed in conjunction with
a Google Map, per the terms and conditions of the Google Places API.

12. Add a single value google place field to your content type. When setting up
this field, specify the various fields you created above as the places to
cache the data.

# USAGE

As the developer, you decide when to create/edit a node and insert a place id
into the field. Once you save the node, this module performs a call to the
Google Places API, fetches the results then saves them into the various fields
within the node.

