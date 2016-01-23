<?php

/**
 * Returns HTML for the date settings form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function twbs_system_date_time_settings($variables) {
  $form = $variables['form'];
  $header = array(
    t('Date type'),
    t('Format'),
    t('Operations'),
  );

  foreach (element_children($form['format']) as $key) {
    $delete_key = $key . '_delete';
    $row = array();
    $row[] = $form['format'][$key]['#title'];
    $form['format'][$key]['#title_display'] = 'invisible';
    $row[] = array('data' => drupal_render($form['format'][$key]));
    $row[] = array('data' => drupal_render($form['delete'][$delete_key]));
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'system-date-types')));
  $output .= drupal_render_children($form);

  return $output;
}
