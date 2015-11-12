<?php

/**
 * @file
 * Simple URL redirector administration interface.
 */

/**
 * Menu callback for generating administration form.
 *
 * @see vburls_admin_settings_submit()
 */
function vburls_admin_settings() {
  $form = array();

  $form['vbulletin'] = array(
    '#type' => 'fieldset',
    '#title' => t('vBulletin'),
    '#collapsible' => FALSE,
  );
  $form['vbulletin']['vburls_vbulletin_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect vBulletin URLs'),
    '#description' => t('If checked <code>showforum.php</code> and <code>showthread.php</code> will be redirected to Drupal URLs.'),
    '#default_value' => variable_get('vburls_vbulletin_redirect', TRUE),
  );
  $form['vbulletin']['vburls_vbulletin_subdir'] = array(
    '#type' => 'textfield',
    '#title' => t('vBulletin sub-directory'),
    '#description' => t('The sub-directory vBulletin is stored inside, for example: <em>forums</em>. <strong>Do not include a trailing slash</strong>.'),
    '#default_value' => variable_get('vburls_vbulletin_subdir', ''),
  );

  $form['photopost'] = array(
    '#type' => 'fieldset',
    '#title' => t('Photopost'),
    '#collapsible' => FALSE,
  );
  $form['photopost']['vburls_photopost_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect Photopost URLs'),
    '#description' => t('If checked <code>showphoto.php</code> and <code>showgallery.php</code> will be redirected to Drupal URLs.'),
    '#default_value' => variable_get('vburls_photopost_redirect', TRUE),
  );
  $form['photopost']['vburls_photopost_subdir'] = array(
    '#type' => 'textfield',
    '#title' => t('Photopost sub-directory'),
    '#description' => t('The sub-directory Photopost is stored inside, for example: <em>photos</em>. <strong>Do not include a trailing slash</strong>.'),
    '#default_value' => variable_get('vburls_photopost_subdir', ''),
  );

  $form['#submit'][] = 'vburls_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Administration form submit handler.
 */
function vburls_admin_settings_submit($form, &$form_state) {
  menu_rebuild();
}
