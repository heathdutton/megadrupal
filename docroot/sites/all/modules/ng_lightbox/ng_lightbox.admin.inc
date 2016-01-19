<?php

/**
 * @file
 * Admin settings for NG Lightbox.
 */

/**
 * Form callback.
 */
function ng_lightbox_settings() {
  $form['container']['ng_lightbox_patterns'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('ng_lightbox_patterns', ''),
    '#description' => t('New line separated paths'),
  );
  $form['container']['ng_lightbox_skip_admin_paths'] = array(
    '#title' => t('Skip all admin paths'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('ng_lightbox_skip_admin_paths', TRUE),
    '#description' => t('This will exclude all admin pages from the lightbox. If you want some paths, see hook_ng_lightbox_ajax_path_alter().'),
  );
  $form['container']['ng_lightbox_disable_css'] = array(
    '#title' => t('Disable CSS'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('ng_lightbox_disable_css', FALSE),
    '#description' => t('Disable the default CSS provided by NG Lightbox.'),
  );
  return system_settings_form($form);
}
