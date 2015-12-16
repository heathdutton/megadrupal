Geckoboard Push API client

Description
-----------

This module is for developers who want to show custom statistics on a Geckoboard
dashboard using the Geckoboard Push API. The Push API has two main benefits:

1. It provides SSL so you can send your stats over an encrypted connection,
   which is safer
2. It doesn't require you to expose a URL on your website, you send the data
   directly to Geckoboard

Features
--------

* Uses hooks to allow other modules to supply data for sending to Geckoboard
* An administration page that allows you to configure your Geckoboard API key
  and Widget Keys
* A drush command to send these statistics
* A cron hook to send these statistics (which can be disabled)

This module also includes a working example that sends two random numbers to
Geckoboard so you can see how to use it.


Installation
------------

This guide assumes you are installing the module in sites/all/modules/ - if not,
please adjust accordingly

* Extract this module to your sites/all/modules/ folder
* Install the module by visiting http://{yoursitedomain}/admin/modules
  look for "Geckoboard Push"
* Visit http://{yoursitedomain}/admin/config/services/geckoboard-push and
  configure the module

Configuration
-------------

To send data to a custom Geckoboard widget you must first create that widget on
Geckoboard. Geckoboard have instructions on how to do this here:

http://docs.geckoboard.com/custom-widgets/push.html

Once the Push widget is created you will see a Widget Key - this is a unique key
for this widget. Copy that key and then visit:

http://{yoursitedomain}/admin/config/services/geckoboard-push

Paste the widget key into the 'widget key' text field for the dataset you'd like
to use, tick the Enable box and save the form.

Usage
-----

The preferred way to use this module is via drush - use "drush geckoboard-push"
or "drush gbp" to send data to Geckoboard.

This module also has a cron hook so even if you don't use drush the module will
still send stats when cron runs. If you prefer to only use drush you can
disable this hook from the administration page.

Finally, you can manually call the geckoboard_push_send_data() function to
trigger a data send.


Thanks
------

This module was sponsored by New Zealand Post and built by Catalyst IT Ltd.
