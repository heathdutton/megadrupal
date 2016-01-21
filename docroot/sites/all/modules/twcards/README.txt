Twitter Cards Lead Generation
-----------------------------
This module adds support for recording data from a Lead Generation Twitter
card.

Installation and Configuration
------------------------------
1. You may want to set a shared salt for verifying incoming POST requests. More
details on the salt can be found at This article with a very reasonable url: https://support.twitter.com/groups/58-advertising/topics/260-promoted-tweets/articles/20170770-setting-up-lead-generation-cards

To set the salt in Drupal adjust and add this line to your settings.php:

$conf['twcards_salt_key'] = 'the value from twitter';

Since the value is a shared-secret it should not be stored in Drupal's normal
variable storage. Putting it in settings.php is safer to prevent it being
accidentally disclosed.

2. By default the module only works with POST requests. If you want to allow GET
requests as well, you should add the following line to your settings.php:

$conf['twcards_allow_get_requests'] = TRUE;

API
---
By default this module will store lead data in a database table. If you want
to do something else you can implement hook_twcards_lead_inserted() to get the
data after it has been validated/normalized.

Credit
------
The initial development was by Greg Knaddison (greggles) in a project for
CARD.com (@card). See card.com/druplicon :)
