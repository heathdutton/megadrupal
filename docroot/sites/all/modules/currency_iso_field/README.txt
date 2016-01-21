INTRODUCTION
------------

This module provides a field type ("Currency ISO code") with the Currency ISO
Codes as select list.

This module does not add any additional functionality apart from saving the
Currency code entered by the end user and display it back to the end user.

The list of currency ISO codes is taken from
http://www.currency-iso.org/dam/downloads/table_a1.xml


REQUIREMENTS
------------

No special requirements.


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

This module does not have any specific configuration. However when the module
is enabled, if you go to the manage fields page of the content types at
admin/structure/types/manage/<content_type>/fields, you will see a new field
type "Currency ISO code" in the Field type selection drop down.

Once you select the field type as "Currency ISO code", in the Widget drop down
field, you will get the following two widgets.

    1) Currency ISO Code :- The node forms with this widget will have as select
       list with a short currency ISO code of the form USD, AUD etc.
    2) Currency ISO Code with country name :- The node forms with this widget
       will have as select list with the currency Iso code and the country name
       eg: USD - United States.
