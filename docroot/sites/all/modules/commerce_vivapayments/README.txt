Commerce Viva Payments module

Viva Payments proxypay system integration for the Drupal Commerce payment
and checkout system.

Dependencies
============

Drupal 7
Drupal Commerce
Drupal Commerce UI
Drupal Commerce Order
Drupal Commerce Payment

Install and Configure
=====================

1) Copy the Commerce Viva Payments folder to the modules folder in your
installation. Usually this is sites/all/modules. Or use the UI and install
it via admin/modules/install.

2) Configure Commerce Viva Payments module via
admin/commerce/config/payment-methods.
Edit Viva Payments payment method and then fill the appropriate fields
and the choose live or test environment.

Configure Viva Payment work environment
=======================================

The demo work environment can be used for all your development/demonstration
requirements. No actual payments or emails are made or sent from this
environment. A successful transaction can be simulated with the card number
4111 1111 1111 1111 with any valid date and 111 for the CVV2. All other card
numbers will result in a failed transaction. You can create demo accounts from
http://demo.vivapayments.com, all that is required is a valid email address.

Creator & Maintainer
======
Creator and Maintainer is netstudio, a Drupal development services company in
Athens, Greece.

Feedback and bug reports
======
If something is not working as expected, you can contact netstudio at
www.netstudio.gr/en/contact or open an issue at the project's issue queue.

Professional Support
======
If you need additional features, customization, drupal optimized hosting,
usability testing, or full site integration, you can get professional,
paid support by netstudio, at www.netstudio.gr/support/en by opening support
tickets or, for bigger implementations, you can contact netstudio at
www.netstudio.gr/en/contact or by phone at +30 210 8004447.
