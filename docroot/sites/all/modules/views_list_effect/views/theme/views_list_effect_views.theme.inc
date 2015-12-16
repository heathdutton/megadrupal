<?php
/**
 * @file
 * Theme callbacks.
 */

/**
 * Preprocess function to build views list effect.
 */
function template_preprocess_views_list_effect_views(&$vars) {
  template_preprocess_views_view_list($vars);

  $items = array();
  foreach ($vars['view']->result as $key => $value) {
    $items[] = $vars['rows'][$key];
  }

  $output = array(
    '#theme' => 'views_list_effect',
    '#items' => $items,
    '#mode' =>  $vars['options']['mode'],
    '#duration' => $vars['options']['duration'],
    '#delay' => $vars['options']['delay'],
    '#random' => $vars['options']['random'],
    '#waypoints' => $vars['options']['waypoints'],
  );
  $vars['views_list_effect'] = render($output);
}