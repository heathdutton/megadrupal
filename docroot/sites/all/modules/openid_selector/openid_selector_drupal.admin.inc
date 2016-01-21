<?php

/**
 * @file
 * OpenID Selector for Drupal administration menu items.
 *
 */

function openid_selector_drupal_settings() {
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['openid_selector_drupal_large'] = array(
    '#type' => 'fieldset',
    '#title' => t('Large button settings'),
  );
  $form['openid_selector_drupal_large']['openid_selector_drupal_large_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Display mode'),
    '#options' => array(
      t('Site name'),
      t('Custom name'),
      t('Site logo'),
      t('Custom logo')
    ),
    '#description' => t('What to display on large button'),
    '#default_value' => variable_get('openid_selector_drupal_large_mode', 0),
  );
  $form['openid_selector_drupal_large']['openid_selector_drupal_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom name'),
    '#description' => t('Use this in case default site name is too long and flows out the boundaries of large button'),
    '#default_value' => variable_get('openid_selector_drupal_site_name', ''),
  );
  $form['openid_selector_drupal_large']['openid_selector_drupal_logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom logo'),
    '#description' => t('Path to custom logo. Must not exceed 100x60px'),
    '#default_value' => variable_get('openid_selector_drupal_logo', ''),
  );
  $form['openid_selector_drupal_large']['openid_selector_drupal_logo_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload custom logo'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload custom logo.")
  );
  $form['openid_selector_drupal_small'] = array(
    '#type' => 'fieldset',
    '#title' => t('Small button settings'),
  );
  $form['openid_selector_drupal_small']['openid_selector_drupal_small_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Display mode'),
    '#options' => array(
      t('Site favicon'),
      t('Custom favicon'),
    ),
    '#description' => t('What to display on small button'),
    '#default_value' => variable_get('openid_selector_drupal_small_mode', 0),
  );
  $form['openid_selector_drupal_small']['openid_selector_drupal_favicon'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom favicon'),
    '#description' => t('Path to custom favicon. Must not exceed 16x16px'),
    '#default_value' => variable_get('openid_selector_drupal_favicon', ''),
  );
  $form['openid_selector_drupal_small']['openid_selector_drupal_favicon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload custom favicon'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload custom favicon.")
  );
  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'openid_selector_drupal_settings_submit');
  $form['#validate'][] = 'openid_selector_drupal_settings_validate';
  return $form;
}

function openid_selector_drupal_settings_validate($form, &$form_state) {
  $file = file_save_upload('openid_selector_drupal_logo_upload', array('file_validate_is_image' => array(), 'file_validate_image_resolution' => array('100x60')));
  if (isset($file)) {
    if ($file) {
      $form_state['values']['openid_selector_drupal_logo_upload'] = $file;
    }
    else {
      form_set_error('openid_selector_drupal_logo_upload', t('The logo could not be uploaded.'));
    }
  }
  $file = file_save_upload('openid_selector_drupal_favicon_upload', array('file_validate_is_image' => array(), 'file_validate_image_resolution' => array('16x16')));
  if (isset($file)) {
    if ($file) {
      $form_state['values']['openid_selector_drupal_favicon_upload'] = $file;
    }
    else {
      form_set_error('openid_selector_drupal_favicon_upload', t('The favicon could not be uploaded.'));
    }
  }
  
  if ($form_state['values']['openid_selector_drupal_logo']) {
    $path = _openid_selector_drupal_settings_validate_path($form_state['values']['openid_selector_drupal_logo']);
    if (!$path) {
      form_set_error('openid_selector_drupal_logo', t('The custom logo path is invalid.'));
    }
  }
  if ($form_state['values']['openid_selector_drupal_favicon']) {
    $path = _openid_selector_drupal_settings_validate_path($form_state['values']['openid_selector_drupal_favicon']);
    if (!$path) {
      form_set_error('openid_selector_drupal_favicon', t('The custom favicon path is invalid.'));
    }
  }
}

function _openid_selector_drupal_settings_validate_path($path) {
  if (drupal_realpath($path)) {
    return $path;
  }
  $uri = 'public://' . $path;
  if (file_exists($uri)) {
    return $uri;
  }
  return FALSE;
}

function openid_selector_drupal_settings_submit($form, &$form_state) {
  $values = &$form_state['values'];
  if ($file = $values['openid_selector_drupal_logo_upload']) {
    unset($values['openid_selector_drupal_logo_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['openid_selector_drupal_large_mode'] = 3;
    $values['openid_selector_drupal_logo'] = $filename;
  }
  if ($file = $values['openid_selector_drupal_favicon_upload']) {
    unset($values['openid_selector_drupal_favicon_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['openid_selector_drupal_small_mode'] = 1;
    $values['openid_selector_drupal_favicon'] = $filename;
  }
  if (!empty($values['openid_selector_drupal_logo'])) {
    $values['openid_selector_drupal_logo'] = _openid_selector_drupal_settings_validate_path($values['openid_selector_drupal_logo']);
  }
  if (!empty($values['openid_selector_drupal_favicon'])) {
    $values['openid_selector_drupal_favicon'] = _openid_selector_drupal_settings_validate_path($values['openid_selector_drupal_favicon']);
  }
}
