<?php
/**
* Implements hook_preprocess_table().
*/
function strapped_preprocess_table(&$variables) {


  $variables['attributes']['class'][] = 'table-bordered';

}
