<?php

/**
 * Returns HTML for a locale date format form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function twbs_locale_date_format_form($variables) {
  $form = $variables['form'];
  $header = array(
    t('Date type'),
    t('Format'),
  );

  foreach (element_children($form['date_formats']) as $key) {
    $row = array();
    $row[] = $form['date_formats'][$key]['#title'];
    unset($form['date_formats'][$key]['#title']);
    $row[] = array('data' => drupal_render($form['date_formats'][$key]));
    $rows[] = $row;
  }

  $output = drupal_render($form['language']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}
