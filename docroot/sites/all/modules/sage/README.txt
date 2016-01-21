sage module
------------


About
-----
API module able to create integration with the SAGE tools
(http://www.sageworld.com/) and mainly with the DataStream XML API.
Drupal just displays products, categories etc and nothing is installed on the
Drupal database.

By default this module provides an admin page for several SAGE settings,
some drupal pages to display the data as also as a lot of functions to get data
from the DataStream API the way you want.


SAGE API resources
-------------------
DataStream XML API: http://sageworld.com/supplier/xml-datastream.php
Official Documentation: http://www.sagemember.com/kb/kb.dll/ViewArticle?ID=10288


SAGE API available data (within Drupal)
---------------------------------------
- Single Product (drupal page, sage/product/SAGE_PRODUCT_ID)
- Category (drupal page, sage/category/SAGE_CATEGORY_ID)
- Search (drupal page, sage/search)
- SAGE Settings (drupal admin page, admin/config/services/sage)
- Category list overview (drupal admin page,
admin/config/services/sage/reports-categories)
- Theme list overview (drupal admin page,
admin/config/services/sage/reports-themes)
- Supplier data (php array from SAGE XML data)
- Supplier list (php array from SAGE XML data)

Notice that many functions (especially for data display) are "project specific"
so you can ignore them.


Documentation
--------------
For the moment please check the /includes/sage.functions.inc file.
The API functions in this file are self described and with proper comments.

1) You can *use this modules API functions* to get php arrays from SAGE
DataStream XML API.

Example:

<?php
$default_option = sage_default_search_option();
$myoptions = array(
  // Get results for SAGE Category with ID 123456.
  "category" => "123456",
);
$option = array_intersect_key($myoptions, $default_option);
$option = array_filter($option);
// Get the php array from XML SAGE API.
$results = sage_load_search($option);
?>

2) You can only *import the SAGE API product or category ids* on Drupal and
then display the results on Drupal using the available Drupal functions.

Example:

<?php
// Category_id is already on Drupal.
$category_id = $node->field_sage_category['und'][0]['value'];

$default_option = sage_default_search_option();
$myoptions = array(
  "category" => $category_id,
);
$option = array_intersect_key($myoptions, $default_option);
$option = array_filter($option);
// Get the php array from XML SAGE API.
$results = sage_load_search($option);
?>


Sponsorship
------------
This module was sponsored by Logismico (http://logismico.com) a 25+ years
software building company with specialization in advanced Drupal projects
and not only.
