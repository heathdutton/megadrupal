CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------
This module adds the ability to get videos from waywire.


REQUIREMENTS
------------
 * This module requires the Feeds module and Feeds HTTPFetcher Append Headers
   modules


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Create a feed as you would normally do.
 * Under Fetcher choose Append Headers HTTP Fetcher
 * Under HTTP Fetcher settings add:
     X-Magnify-Key|[your api key]
 * Under Parser choose Waywire parser


MAINTAINERS
-----------
Current maintainer:
 * Albert Jankowski (albertski) - https://www.drupal.org/u/albertski
 * Yevgeny Ananin (yevgeny.ananin) - https://www.drupal.org/user/1774624
