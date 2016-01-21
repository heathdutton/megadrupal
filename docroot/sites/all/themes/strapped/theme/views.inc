<?php
/**
 * @file
 * Views related theme functions.
 */


/**
 * Implements hook_preprocess_views_exposed_form().
 *
 * @param $variables
 */
function strapped_preprocess_views_exposed_form(&$variables) {

  // Loop through the exposed widgets and transfer across the label display settings.
  foreach ($variables['widgets'] as $key => $widget) {

    // we need to find the original fapi definition
    $id = str_replace('edit-', '', $widget->id);


    $label = $widget->label;
    $widget->label = array('element' => array());
    $widget->label['element']['#title'] = $label;
    $widget->label['element']['#id'] = $widget->id;

    if (isset($variables['form'][$id])) {
      $original = $variables['form'][$id];
      $widget->label['element']['#title_display'] = $original['#title_display'];
      $widget->label['element']['#attributes'] = $original['#attributes'];
    }


  }
}
