
-- SUMMARY --

This module enables Drupal to remember entered values from selected fields
upon new node submission, stores them in the database and then pre-fills
appropriate fields on the next node-add form. This is useful when a user
needs to enter many nodes at a time and a few of the values are repeating.


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Go to Content management >> Content types >> Edit content-type and select
the fields which you want to be remembered from the list under
"Remember values" fieldset.

* You can switch off the module temporarily by navigating to
Configuration >> Content authoring >> Remember values.
Also you can set an amount of time after which the values
will not be remembered anymore.
