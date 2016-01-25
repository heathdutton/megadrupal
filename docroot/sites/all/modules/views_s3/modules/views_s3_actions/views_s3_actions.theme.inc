<?php

/**
 * @file
 * Theme imlpementation file
 */

/**
 * Theme implementation for theme_views_s3_actions_list
 */
function theme_views_s3_actions_list($variables) {
  return theme('item_list', array('items' => $variables['actions']));
}

/**
 * Theme implementations for theme_views_s3_actions_default_action
 */
function theme_views_s3_actions_default_action($variables) {
  $action = $variables['action'];
  if (!empty($action['#theme'])) {
    return array(
      '#theme' => $action['#theme'],
      '#action' => $action,
      '#values' => $variables['values'],
    );
  }

  // Replace arguments (case insensitive)
  foreach (array_keys($variables['values']) as $key) {
    if (preg_match("#/%{$key}/?#i", $action['#href'])) {
      if (strtolower($key) == 'key') {
        $variables['values'][$key] = urlencode($variables['values'][$key]);
      }
      $action['#href'] = preg_replace("#%{$key}#i", $variables['values'][$key], $action['#href']);
    }
  }
  return l($action['#title'], $action['#href'], !empty($action['#options']) ? $action['#options'] : array());
}