<?php

/**
 * Implements hook_form().
 */
function structured_data_admin_site_name_settings() {
  $form['structured_data_site_name_default'] = array(
    '#type' =>'checkbox',
    '#title' => t('Use the default site name'),
    '#default_value' => variable_get('structured_data_site_name_default', TRUE),
  );

  $form['structured_data_site_name_custom'] = array(
    '#type' =>'textfield',
    '#title' => t('Custom site name'),
    '#default_value' => variable_get('structured_data_site_name_custom', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="structured_data_site_name_default"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['structured_data_site_name_alternative'] = array(
    '#type' =>'textfield',
    '#title' => t('Alternative site name'),
    '#default_value' => variable_get('structured_data_site_name_alternative', ''),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form().
 */
function structured_data_admin_logo_settings() {
  $data = structured_data_get_setting('site_logo');
  $current_logo = array(
    'path' => $data['site_logo'],
    'attributes' => array()
  );

  $form['structured_data_site_logo_current'] = array(
    '#type' =>'item',
    '#title' => t('The current site logo'),
    '#markup' => theme_image($current_logo),
  );

  $form['structured_data_site_logo_default'] = array(
    '#type' =>'checkbox',
    '#title' => t('Use the default site logo'),
    '#default_value' => variable_get('structured_data_site_logo_default', TRUE),
  );

  $form['structured_data_site_logo_custom'] = array(
    '#type' => 'managed_file',
    '#title' => t('Custom site logo'),
    '#description' => t('Upload a file, allowed extensions: jpg, jpeg, png, gif'),
    '#upload_location' => 'public://structured_data/',
    '#default_value' => variable_get('structured_data_site_logo_custom', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="structured_data_site_logo_default"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'structured_data_admin_site_logo_settings_form_submit';
  return $form;
}

function structured_data_admin_site_logo_settings_form_submit($form, &$form_state) {
  global $user;
  $form['structured_data_site_logo_custom']['#file']->status = FILE_STATUS_PERMANENT;
  $file = file_save($form['structured_data_site_logo_custom']['#file']);
  file_usage_add($file, 'structured_data', 'site_logo', $user->uid);
}
