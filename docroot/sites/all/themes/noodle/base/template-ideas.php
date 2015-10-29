<?php


/**
* Add additional features to default drupal page.
*/
function noodle_preprocess_page(&$variables) {

  // equal heights for main comumns
    drupal_add_js('jQuery(document).ready(function(){equalHeight(jQuery("#main .column"));});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 2));
  ## print_r ($node->field_collaboration);	


  // equal heights for header blocks
  if (block_get_blocks_by_region('header_blocks_1')|block_get_blocks_by_region('header_blocks_2')|block_get_blocks_by_region('header_blocks_3')) {

    drupal_add_js('jQuery(document).ready(function(){equalHeight(jQuery(".region-header-blocks .region"));});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 3));
  }

  // equal heights for footer blocks
  if (block_get_blocks_by_region('footer_blocks_1')|block_get_blocks_by_region('footer_blocks_2')|block_get_blocks_by_region('footer_blocks_3')) {

    drupal_add_js('jQuery(document).ready(function(){equalHeight(jQuery(".region-footer-blocks .region"));});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 4));
  }
}

/**
* Preprocess html with more options then default
*/

function noodle_preprocess_html(&$variables) {
  
// We need to remove 'no-sidebars' class as drupal only can deal with it with default 2 sidebars
foreach ($variables['classes_array'] as $key => $val) {
  if ($val == 'no-sidebars') {
    unset($variables['classes_array'][$key]);
  }
}

//adding classes if we have some blocks in sidebar
if (!empty($variables['page']['sidebar_first'])) {
  $variables['classes_array'][] = 'one-sidebar sidebar-first';
}
elseif (!empty($variables['page']['sidebar_second'])) {
  $variables['classes_array'][] = 'one-sidebar sidebar-second';
}
elseif (!empty($variables['page']['sidebar_accordion'])) {
  $variables['classes_array'][] = 'one-sidebar sidebar-accordion';
}
else {
  $variables['classes_array'][] = 'no-sidebars';
}
  

}