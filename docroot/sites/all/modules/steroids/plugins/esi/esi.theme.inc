<?php

/**
 * Theming function for the esi settings table
 */
function theme_steroids_esi_table($variables) {
  $form = $variables['form'];

  $header = array(t('Name'), t('Status'), t('TTL'), t('Path'));
  $rows = array();

  $keys = element_children($form['blocks']);
  foreach ($keys as $key) {
    $row = array();
    $row[] = drupal_render($form['blocks'][$key]['name']);
    $row[] = drupal_render($form['blocks'][$key]['status']);
    $row[] = drupal_render($form['blocks'][$key]['ttl']);
    $row[] = drupal_render($form['blocks'][$key]['path']);
    $rows[$key] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;

}