DESCRIPTION
============
This module makes a "Same As" component available for use on your webforms.
This component allows you to add a "Same as" checkbox to a form allowing you to
indicates that one set of fields should have the same value as another set of
fields. For example, if you had fields for both a Billing and Shipping address
on your webform, you could add the Same As component to allow users to only
fill out their billing address if their shipping address is the same.


FEATURES
========
* Allows either Hiding or Disabling the target fields when the
  same-as checkbox is checked.
* Copies the data from the source fields to the target fields during the
  form submission so that both sets of fields are populated with the same
  information.
* Will allow mapping multiple field pairs in the future
  (current version only allows 1 mapping)


LIMITATIONS
===========
* You can only copy values between 1 pair of fields.
* There is no validation to ensure that the source and destination fields are
  the same type. Make sure you are not trying to copy data from components of
  different types.
* Source and target fields must be on the same page of a multi-page webform.



REQUIREMENTS
============
* Webform 3.x


INSTALLATION
============
* Download to your site's modules directory.
* Enable the module.
* Add the "Same As" component to your webform.


CONFIGURATION
=============
When you add the "Same As" component to your webform, there are only a couple
configuration options available:

Source Field:
-------------
Select the field that contains the data which should be copied to the
target field.


Target Field:
-------------
Select the field that will contain the same data as the source field.


Display Mode:
-------------
Select how the target field should be displayed when the "Same-As"
checkbox is checked.
