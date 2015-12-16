
-- SUMMARY --

This module allows you to compare products.
This is a familiar feature on commerce website.

To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/2033985


-- REQUIREMENTS --

* Drupal Commerce


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* Go to 'admin/structure/block', place the block "Compare list" where you want

-- CONFIGURATION --

* Go to the "Manage display" page of your product variation type
  and set in the view mode "Compare page"
  the fields you want to be able to compare.
  Set hidden if you don't want a field to show in the comparison table.

* Extra : If you have two product variation types (product types),
  you can map which field is comparable with other
  here => 'admin/commerce/config/compare'.

  You can also enable filters on the compare page.


-- FAQ --

Q: When the administration menu module is enabled,
   I can't see what's in my compare list. Why?

A: Because you have to set where the block (compare list) should be displayed.

Q: When I add products to the compare list,
   I have the following message "Can not be compared with current selection".
   Why?

A: Because products in the compare list must have at least one field in common
   (FIELD, not property like SKU, status for example).


-- CONTACT --

Current maintainers:
* Yohan TILLIER (garnett2125) - http://drupal.org/user/1281666
