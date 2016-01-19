-------------------------------------------------------------------------------
Sticky columns for Drupal 7.x
	by ADCI, LLC team - www.adcillc.com
-------------------------------------------------------------------------------

Description:
Sticky columns module allows you to add table sticky columns for Views table (by admin area) and for any table (by API).

Installation:
1) Install the module.
2) Go to the edit page of the needed view.
3) Select "Settings" of "Table" format.
4) Choose any field to use as Sticky Column.

API:
To use Sticky Column for any table add "sticky-columns" class for the table and "sticky-column" class for each cell of the needed column. 
Then add script and styles:
  drupal_add_js(drupal_get_path('module', 'sticky_columns') . '/sticky-columns.js');
  drupal_add_css(drupal_get_path('module', 'sticky_columns') . '/sticky-columns.css');