
TWITTER WEB INTENTS MODULE FOR DRUPAL 7.x
-----------------------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Requirements
   * Installation
   * Support
   * Credits


DESCRIPTION
-----------
This module allows you to add extra Twitter fields and area handlers into
"Twitter message" views. These fields are links to Twitter actions, like
replying, retweeting, adding to favorites, following... with available
extra behaviors (i.e. adding hashtags, adding "via" text...).

Specifically, right now it adds these fields:
  * Reply link field (a tweet given by the current row).
  * Retweet link field (a tweet given by the current row).
  * Favorite link field (a tweet given by the current row).
  * Follow link field (an account given by the row's current tweet).
  * Follow link area (Twiter account to follow, by ID or screen name).

In each field, all links can be configured as described in Twitter Web Intents
documentation page: https://dev.twitter.com/docs/intents, :
  * Reply link field.
    * Link text.
    * URL.
    * Via.
    * Reply text.
    * Hashtags.
    * Related.
  * Retweet link field.
    * Link text.
    * Related.
  * Favorite link field.
    * Link text.
    * Related.
  * Follow link field.
    * Link text.
  * Follow link area.
    * Author input mode (ID / Screen name).
    * Author's link by ID (When Author input mode = ID).
    * Author's link by Screen name (When Author input mode = Screen name).

For all of them, link attributes are also configurable, with this options:
  * Title ("title" attribute).
  * Name ("name" attribute).
  * Relationship ("rel" attribute).
  * Classes ("class" attribute).
  * Style ("style" attribute).
  * Target ("target" attribute, it's only effective when navigator's JS
    is disabled).

Additional javascript is added from Twitter API:
http://platform.twitter.com/widgets.js
This script is added just when necessary (when rendering some of the given
fields), not in every page.


REQUIREMENTS
------------

Views 3.x
Twitter 7.x-4.x (not tested yet in previous versions)


INSTALLATION
------------

Just download and enable the module, and a set of Views fields and Views areas
("fields" for "Header", "Footer" and "Empty" configuration regions) will appear,
when using "Twitter message" Views type, in "Twitter" section. You can identify
them because of their names, in the form: "Twitter: Twitter Web Intents: ...".

You can configure each field as described in Twitter Web Intents documentation
page: https://dev.twitter.com/docs/intents.


SUPPORT
-------

Donation is possible by contacting me via grisendo@gmail.com


CREDITS
-------

7.x-1.x Developed and maintained by grisendo
