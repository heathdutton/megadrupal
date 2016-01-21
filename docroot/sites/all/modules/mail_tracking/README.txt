Mail Tracking
=============

The Mail Tracking module tracks the number of recipients that read your e-mail
message, click on a link in it or unsubscribe (if used in combination with Simplenews).

Dependencies
------------
There are no required dependencies, but the module is best used in combination
with the Mime Mail 1.x module to properly track the read count and to hide
rewritten (tracked) URL's for your recipients.

Configuration
-------------

The default configuration will do fine for most websites, but if desired you can
optionally change the statistics grouping period and token retention time at
http://yoursite.com/admin/config/system/mail-tracking

The statistics can be found at http://yoursite.com/admin/reports/mail-tracking
Please not that you have to set the appropriate permissions at
http://yoursite.com/admin/people/permissions for your users so they can access
the statistics.

Important notes
---------------

- When sending plain text messages any links to the homepage won't be rewritten
  (and tracked), because it will give a weird/undesired effect if you just want
  to add your website address to the e-mail message.
- It's not possible to track the read count for plain text messages, because
  the module needs to add/rewrite an image for that.
