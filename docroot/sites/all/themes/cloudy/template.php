<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * Cloudy theme.
 */

/**
 * Implements hook_form_alter().
 */
function cloudy_form_alter(&$form, $form_state, $form_id) {
  // Force certain forms to be stacked.
  if (in_array($form_id, array('user_pass', 'user_register_form', 'user_login', 'user_login_block'))) {
    $form['#attributes']['class'][] = 'form-stacked';
  }

  if ($form_id == 'search_api_saved_searches_save_form') {
    $form['notify_interval']['#default_value'] = 604800;  //Weekly is default.
  }
}

/**
 * Implements hook_css_alter().
 */
function cloudy_css_alter(&$css) {
  unset($css[drupal_get_path('module', 'addressfield') . '/addressfield.css']);
  unset($css[drupal_get_path('module', 'date_api') . '/date.css']);
}