<?php

/**
 * Returns HTML for the form showing actions assigned to a trigger.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: The fieldset including all assigned actions.
 *
 * @ingroup themeable
 */
function twbs_trigger_display($variables) {
  $element = $variables['element'];

  $header = array();
  $rows = array();
  if (isset($element['assigned']) && count($element['assigned']['#value'])) {
    $header = array(array('data' => t('Name')), array('data' => t('Operation')));
    $rows = array();
    foreach ($element['assigned']['#value'] as $aid => $info) {
      $rows[] = array(
        check_plain($info['label']),
        $info['link']
      );
    }
  }

  if (count($rows)) {
    $output = theme('table', array('header' => $header, 'rows' => $rows)) . drupal_render_children($element);
  }
  else {
    $output = drupal_render_children($element);
  }
  return $output;
}
