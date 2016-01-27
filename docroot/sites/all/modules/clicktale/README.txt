ClickTale - custom experience analytics
---------------------------------------
ClickTale is used to collect user experience analytic data, which can be used to
enhance the users experience of the site. ClickTale records videos of the users
movement on the site based on the users mouse movements. ClickTale also generates
heatmaps based on where the user clicks and hover their mouse on the pages.

The Drupal Clicktale module maintainers have no affiliation with ClickTale Ltd.
and are therefore not responsible for any services provided by ClickTale or the
handling of data collected by ClickTale services.


Requirements
------------
This module requires that you have a ClickTale user account at
http://www.clicktale.com/. You can sign-up for a free account, which have a
limited amount of pageviews per month (see http://www.clicktale.com/pricing/plans).

The module is dependent on the libraries module, which can be found at
http://drupal.org/project/libraries, to locate the ClickTale integration PHP 
library.

Installation
------------
Download the ClickTale integration PHP library from
http://www.clicktale.com/integration/ClickTalePHPIntegrationModule_latest.zip
and unpack it into sites/all/libraries/clicktale. For security you should delete
the install folder located in the ClickTale library. Make sure that the
web-server has write permissions to the file config.php inside the ClickTale
library folder.

To install the module unpack it into your module folder and enable the ClickTale
module.


Configuration
-------------
Go to Administer -> Site configuration -> Clicktale to configure ClickTale. Note
that all paths under /admin and user 1 is never tracked for security reasons. The
tracking code is not inserted if these conditions are true.


Varnish cache
-------------
If you are running varnish in front of drupal, you have to make some
modifications to the ClickTale configuration. First varnish will be bypassed as
the ClickTale integration library set a cookie to keep track of user movements
across pages to generate the tracking videos.

You will have to modify the integration library's config.php file, so it
contains the IP's of the varnish proxy. If it runs on the same host add
localhost IP as below:

  $config['AllowedAddresses'] = "75.125.82.64/26, 127.0.0.1";

This will allow ClickTale services to gatherer cached page information used to
create videos, if the varnish IP is not added the videos will be empty.
ClickTale uses this cache to ensure, that when it generates videos they contain
what the user saw and not how the site looks now.
