Pickadate
=========

The mobile-friendly, responsive, and lightweight jQuery date & time input picker.

How to Use
==========

First, you will need to download the Pickadate.js library. At the time of this writing,
version 3.2.2 is supported. You will also need jQuery Update for jQuery version 1.8 or 1.7.

Place the pickadate folder into your libraries folder.

Pickadate.js on GitHub: https://github.com/amsul/pickadate.js

FormAPI Element and Overriding Settings
=======================================

To add Pickadate to your form or alter how an existing field acts:

In your Form API code or a hook_form_alter, set a field type to 'pickadate_date' or 'pickadate_time'. After that,
give it a title, and most importantly, add a #settings key and call pickadate_date_default_settings() or pickadate_time_default_settings()
appropriately.

This function populates the form element with default settings for the JS plugin. You can pass in an array to these functions to override
the defaults to your liking.

Example:

$form['foo'] = array(
  '#type' => 'pickadate_date',
  '#title' => t('Date'),
  '#settings' => pickadate_date_default_settings(),
);

$form['foo'] = array(
  '#type' => 'pickadate_time',
  '#title' => t('Time'),
  '#settings' => pickadate_time_default_settings(),
);

$form['foo'] = array(
  '#type' => 'pickadate_date',
  '#title' => t('Date'),
  '#settings' => pickadate_date_default_settings(array('selectMonths' => TRUE, 'disable' => array(1,3,5), 'format' => 'mmmm d, yyyy'),
);

$form['foo'] = array(
  '#type' => 'pickadate_date',
  '#title' => t('Date'),
  '#settings' => pickadate_date_default_settings('interval' => 60),
);

Note that, being a custom form element, there is really no validation provided. It is assumed you will set most of the validation
upfront by using the 'disable' setting option to prevent certain date entries. Beyond that, you will need to handle the validation
in your own validate handler if you have additional business logic to check when the form is processed.

The same goes for submission. If the value needs formatting for Drupal, that part will be upon you in a submit handler to massage that
data before letting Drupal get ahold of it.

More Examples
=============

To see the wide array of options at your disposal, check out the demonstration site for Pickadate.js.

http://amsul.ca/pickadate.js/

Bugs / Requests
===============

Please file bugs in the Drupal issue queue for Pickadate.js.

Development sponsored by Inclind, Inc.

Website: http://www.inclind.com
Twitter: @inclind