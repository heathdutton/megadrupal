--Mobile Subdomain--

INSTALLATION
- Place the module in your working directory under 'sites/all/modules'
+ Add the jQuery file from http://detectmobilebrowsers.com to your
+ libraries folder under jQuery.browser.mobile/detectmobilebrowser.js
- Enable the module using Drupal's module administration.

CONFIGURATION
- Visit 'admin/settings/mobile-subdomain' to configure the subdomain prefix.

HOW IT WORKS
- The included javascript file will detect a mobile device.  If a device
  is detected, an Ajax callback will ask for the mobile subdomain prefix.
  Using the prefix, the URL of the current page is re-written and the
  user is redirected to the mobile site. Any relative links will follow
  the proper path. If an absolute link is visited, the redirect will 
  fire again and the user will remain on the mobile subdomain of the site.
  
CREDITS
- Module created by brantwynn
- jQuery.browser.mobile by Chad Smith
- Development sponsored by PrometSource
