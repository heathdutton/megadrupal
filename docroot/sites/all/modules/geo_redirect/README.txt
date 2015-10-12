--------------------------------------------README------------------------------------------------------

GeoRedirect

Allows country-wise redirect.

No dependancies.

Library files downloaded from: http://shockmarketer.com/geoip-country-location-php-script/

You can download latest IP-country database file (GeoIP.dat) from above link and
replace it with /includes/GeoIP.dat

----------------------INSTALLATION------------------------------------------------------------------------

1. Extract the module into /sites/all/modules directory.

2. Enable GeoRedirect module at admin/modules.

----------------------USAGE-------------------------------------------------------------------------------

1. Go to admin/config and click on link 'GeoRedirect URLs' to add Redirect URLs or
simply go to admin/config/geo-redirect/list.

2. Click on Add Redirect URL link to add redirect URLs.

3. Add the country and redirect url. Also dont give your site's internal URLs like www.example.com/node/3.
This module is only to redirect users to external URLs.

4. 'Allow /user path' box should be checked if you are adding your country to redirect list.
It will give you access to /user path from where you can login to site.

4. Checking this path box is the only way to access the site's login page,
or you need to change the code in hook_init().

-----------------------Limitations------------------------------------------------------------------------

This module doesn't allow internal redirects like Path Redirect (http://drupal.org/project/path_redirect)
or Redirect (http://drupal.org/project/redirect). It redirects to other site for specific country.

Written by: Tanmay Khedekar.
