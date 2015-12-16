DESCRIPTION
-----------

Commerce User Profile Pane is a module that allows you to capture a Drupal
Users profile fields directly in the Commerce checkout pages. This is similar
to the Commerce Billing Profile or Commerce Shipping Profile panes, but instead
of storing extra fields in the Commerce Profile entity, this will allow you
to store it directly in the Drupal core User entity. This is useful when you
want a quick registration process without the user having to fill a long user
registration form, but you still want to retain the option to complete the User
profile info in the checkout phase.

HOW IT WORKS
------------

This pane generate a form with the fields that are configured to be captured
here. After installing this module you will find a new settings in the User
profile fields configuration to chose if display a field on commerce user
profile pane form so the user can fill that information directly in the checkout
process instead of the default user registration form.

INSTALLATION
------------

- Enable this module as usual.

- Go to admin/config/people/accounts/fields and edit the configuration "Display
  on commerce user profile pane form" for each field that you want to capture in
  the User Profile Pane form. Also if you want the field to be mandatory, enable
  the option "Required field on commerce user profile pane form".

- Finally go to admin/commerce/config/checkout and define which checkout page
  the Commerce User Profile Pane should display.

CREDITS
-------

- The development of this module was sponsored by `BlueSpark <http://www.bluespark.com>`_
