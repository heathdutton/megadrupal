<?php

/**
 * Process webmoney form to show all purces as list
 */
function theme_pg_webmoney_list($variables) {
  $variables = $variables['form'];
  $form = $variables['form'];

  $header = array(
    theme('table_select_header_cell'),
    t('WMType'),
    t('Purse'),
    t('Secret key'),
    t('Rate'),
    '',
  );

  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();
    if (is_array($form[$key]['purse'])) {
      $row[]  = drupal_render($form['check'][$key]);
      $row[]  = drupal_render($form[$key]['edit']);
      $row[]  = drupal_render($form[$key]['purse']);
      $row[]  = drupal_render($form[$key]['secret_key']);
      $row[]  = drupal_render($form[$key]['rate']);
      $row[]  = drupal_render_children($form[$key]['configure']);
      $rows[] = $row;
    }
  }
  
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}
