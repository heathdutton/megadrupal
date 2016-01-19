<?php

/**
 * @file
 * PMB external search form template.
 */

$header = array(
  t('Sources'),
  t('Comments'),
);
$rows = array();

foreach (element_children($form['sources']) as $key) {
  drupal_render($form['sources'][$key]);
  if (is_array($form['sources'][$key])) {
    $row = array();
    if (!is_numeric($key)) {
      $row[] = array(
        'data' => $form['sources'][$key]['#value'],
        'class' => array('module'),
        'colspan' => 2,
      );
    }
    else {
      $row[] = array(
        'data' => drupal_render($form['checkboxes'][$key]),
        'class' => array('permission'),
      );
      $row[] = array(
        'data' => $form['sources'][$key]['#comment'],
        'class' => array('permission'),
      );
    }
  }
  $rows[] = $row;
}

$template .= drupal_render($form['search_query']);
$template .= drupal_render($form['search_fields']);
$template .= theme('table', array(
  'header' => $header,
  'rows' => $rows,
  'parameters' => array(
    'id' => 'sources',
)));
$template .= drupal_render_children($form);
