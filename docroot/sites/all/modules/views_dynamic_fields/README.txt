SUMMARY
-------

The Views Dynamic Fields module provides a filter for use with Views module.
This filter allows the user to pick and choose which fields to display for a
rendered instance of a view for that user. This provides a customized view
instance for each user.

This module provides additional value when used with the Views Excel Export
module (http://drupal.org/project/views_export_xls) to generate an xls file from
a view. The xls file will only display the fields you have chosen on the
rendered view instance in browser.


REQUIREMENTS
------------

This module depends on

 * Views: http://drupal.org/project/views
 * Chaos Tools Suite: http://drupal.org/project/ctools


INSTALLATION
------------

 1. Place the entirety of this directory in:
      /sites/all/modules/views_dynamic_fields
 2. Navigate to Administration > Modules.
 3. Enable "Views Dynamic Fields" and "Views UI".


CONFIGURATION
-------------

None.


USAGE
-----

  1.  Create a view by navigating to Administration > Structure > Views and
      click 'Add new view' (or edit an existing view), chose between Content
      (nodes) or Users (entities)
  2.  In the view use the 'Table' display format
  3.  Add fields and other items to your view
  4.  Click on 'Add' near Filters to add a filter
  5.  Choose 'Dynamic Fields' for nodes or users
  6.  Select fields you want to make selectable and which should be selected
      by default
  7.  Click 'Apply and continue'
  8.  Choose 'Expose' (this filter is to be used as an exposed filter)
  9.  Click 'Apply'
  10. Click 'Save' to save your view to the database
  11. Navigate to the view. You should now see a list of fields of that view
      with a checkbox next to each.
  12. Select the list of fields that you want to include/exclude (see step 4)
      in the current displayed instance
  13. Click 'Apply' to see the view displayed with only the fields chosen


TROUBLESHOOTING
---------------

 * If you are not seeing the filter with the list of fields with checkboxes,
   make sure you have set the 'Dynamic Fields' filter to 'exposed'.
 * If you notice any other issues, please report them here:
   https://drupal.org/project/issues/views_dynamic_fields


SPONSOR
-------

 * The 7.x development is sponsored by Parlamentwatch e.V. (NGO).
   https://www.abgeordnetenwatch.de/


CONTACT
-------

 * Girish Nair (girishmuraly) - http://drupal.org/user/61249
 * A. Schoedon (donSchoe) - https://drupal.org/user/1017700
