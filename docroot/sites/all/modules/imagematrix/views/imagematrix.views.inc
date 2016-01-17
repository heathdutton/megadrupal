<?php

/**
 * @file
 * Views style plugin definition for Image Matrix module.
 */

/**
 * Implements hook_views_style_plugins().
 */
function imagematrix_views_plugins() {
  return array(
    'style' => array(
      'imagematrix' => array(
        'title' => t('Image Matrix'),
        'help' => t('Displays rows as an image matrix.'),
        'handler' => 'views_plugin_style_imagematrix',
        'path' => IMAGEMATRIX_PATH . '/views',
        'theme' => 'views_view_imagematrix',
        'theme file' => 'theme.inc',
        'theme path' => IMAGEMATRIX_PATH . '/theme',
        'uses row plugin' => TRUE,
        'uses row class' => FALSE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function imagematrix_form_views_ui_edit_display_form_alter(&$form, &$form_state) {
  if (!empty($form['options']['style_options']['#imagematrix'])) {
    $form['buttons']['flush'] = array(
      '#type' => 'submit',
      '#value' => t('Flush generated images'),
      '#submit' => array('imagematrix_flush_style'),
    );
  }
}
