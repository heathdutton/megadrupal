Introduction
============

It builds new entity (user_address) using an Address Field
field as its default data structure.

Requirements:
- Address Field;
- Views.

=========================================================

Drupal Commerce (Checkout) integration
======================================
Tip : Make sure both User address and customer profile have the same entity structure (same fields/data)
"User Addressbook Commerce" submodule in the project package.
The customer user addresses from User addressbook could be used as customer profiles in checkout process.
And viceversa
The customer profiles in the checkout process could be saved in the User addressbook.
This data "exchange" between entities is done with Rules, so it could be adapted to everyone needs (custom entities structure).

How it works:

- New checkout page : Addresses, with its own addressbook checkout pane.
  - No previous/continue buttons.
  - Use AJAX for checkout page (pane) form
  - Addresses (sub)pages - STEP - for every customer profiles.
- Disable customer profiles checkout panes.
- View of existing user addresses with selection button.
- Posibility to add new customer profile for order and save it to addressbook.
- Posibility to return directly to addresses checkout for changing customer profile data from Review page.
