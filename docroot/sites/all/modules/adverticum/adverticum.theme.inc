<?php

/**
 * @file
 * The theme system, which controls the output of the Adverticum module.
 */

/**
 * Themes the output of the administration settings page.
 */
function theme_adverticum_admin_settings_zones($variables) {
  $form = &$variables['form'];
  $header = array(t('Zone ID'), t('Name'), t('Tag'), t('Active'), array('data' =>t('Operations'), 'colspan' => 2));
  foreach (element_children($form) as $key) {
    $rows[] = array(
      drupal_render($form[$key]['id']),
      drupal_render($form[$key]['name']),
      drupal_render($form[$key]['tag']),
      drupal_render($form[$key]['active']),
      drupal_render($form[$key]['delete']),
      drupal_render($form[$key]['configure']),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}