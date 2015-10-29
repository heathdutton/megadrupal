<?php

/**
 * Implements hook_preprocess_item_list().
 */
function semantic_ui_omega_preprocess_item_list(&$variables) {
  if(isset($variables['attributes']) && isset($variables['attributes']['class'])){
    $class = $variables['attributes']['class'];
    $class =  is_array($class) ? $class : array($class);
    $result = array_merge((array)$class, array('ui', 'list'));
    $variables['attributes']['class'] = $class;
  }else{
    $variables['attributes']['class'] = array('ui', 'list');
  }
}
