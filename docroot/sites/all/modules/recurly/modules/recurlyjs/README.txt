Description
-----------
This module provides FormAPI elements for subscriptions, updating billing
information, and one-time payments through Recurly.com.

EXTREMELY IMPORTANT CAVEAT: While this module provides convienent FormAPI
elements such as $element['#type'] = 'recurlyjs_subscribe', these elements
operate entirely through JavaScript and sensitive information never touches
your server (thus circumventing PCI compliance requirements). This comes with
the following caveats:

- The recurlyjs elements cannot be on the same page as other elements that need
  validation, because the credit card will be charged client-side. Validation
  errors done server-side would be *after* the credit card has been charged.
  It is recommended to build forms that only contain the recurly element, or
  use multiple-step forms with only the recurly element on the last page by
  itself.
- Users must have JavaScript enabled in their browser to submit payment.
- Recurly.js is compatible with IE7 and higher.
- Recurly.js recommends jQuery 1.5.2 or higher. Drupal 7 comes with jQuery 1.4.4
  out of the box, however we are not aware of any issues with using Recurly.js
  with this older version of jQuery.

Requirements
------------
Drupal 7.x
Recurly
Libraries API

API Information
---------------
This module provides 3 FormAPI elements that help manage credit cards through
the Recurly service.

Subscription forms:

$form['subscribe'] = array(
  '#type' => 'recurlyjs_subscribe',
  '#plan_code' => 'gold',
);

Billing update forms:

$form['billing_update'] = array(
  '#type' => 'recurlyjs_update',
  '#account_code' => 'hash',
);

And one-time payment forms:

$form['subscribe'] = array(
  '#type' => 'recurlyjs_payment',
  '#amount' => 3000,
  '#description' => t('Description to show up on invoice'),
);

There are several other options available for each type, but these basics are
the most common. For more information see the recurlyjs_element_info() function
in the recurlyjs.module file.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/recurly?categories=All

WE DO NOT PROVIDE SUPPORT ON CUSTOM CODING. Please use the issue queue for
issues that are believed to be bugs or for requesting features.
