The module suppresses the display of error messages 
(usually from validations) on Forms.

You can configure the form ids on which you would want the errors 
to be suppressed.

This does not mean that the form is submitted skipping validation. 
Drupal's validation still holds good. It is just that the display of error 
messages is suppressed.

In fact the error classes that Drupal adds to the form fields when the
form-reloads still exist.

WARNING: YOU WOULD NOT NEED THIS MODULE USUALLY. 
If you are using this module, 
you EITHER completely understand what you are doing 
OR you are building your Drupal site the wrong way.