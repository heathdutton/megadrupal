<?php 
/**
 * @file
 * Admin settings form for Default File Settings.
 */

/**
 * Custom settings for the Default File Settings module.
 */
function default_file_settings_settings_form() {
  $types = array('image', 'file');

  foreach ($types as $type) {

    $section_title = ucwords($type);
    $form[$type] = array(
      '#title' => check_plain($section_title),
      '#type' => 'fieldset',
    );

    if ($type == 'image') {
      $form[$type]['dfs_default_image'] = array(
        '#title' => t('Default image'),
        '#type' => 'managed_file',
        '#default_value' => variable_get('dfs_default_image', ''),
        '#description' => t('Default image file ID (ex: 5).'),
      );

      $min_resolution = variable_get('dfs_min_resolution', array());
      $form[$type]['dfs_min_resolution'] = array(
        '#title' => t('Minimum resolution'),
        '#type' => 'fieldset',
      );
      $form[$type]['dfs_min_resolution']['dfs_min_resolution_x'] = array(
        '#title' => t('Width'),
        '#type' => 'textfield',
        '#default_value' => isset($min_resolution['x']) ? $min_resolution['x'] : '',
        '#description' => t('Default min width (ex: 1024).'),
      );
      $form[$type]['dfs_min_resolution']['dfs_min_resolution_y'] = array(
        '#title' => t('Height'),
        '#type' => 'textfield',
        '#default_value' => isset($min_resolution['y']) ? $min_resolution['y'] : '',
        '#description' => t('Default min width (ex: 768).'),
      );

      $max_resolution = variable_get('dfs_max_resolution', array());
      $form[$type]['dfs_max_resolution'] = array(
        '#title' => t('Maximum resolution'),
        '#type' => 'fieldset',
      );
      $form[$type]['dfs_max_resolution']['dfs_max_resolution_x'] = array(
        '#title' => t('Width'),
        '#type' => 'textfield',
        '#default_value' => isset($max_resolution['x']) ? $max_resolution['x'] : '',
        '#description' => t('Default min width (ex: 1024).'),
      );
      $form[$type]['dfs_max_resolution']['dfs_max_resolution_y'] = array(
        '#title' => t('Height'),
        '#type' => 'textfield',
        '#default_value' => isset($max_resolution['y']) ? $max_resolution['y'] : '',
        '#description' => t('Default min width (ex: 768).'),
      );
    }

    $form[$type]['dfs_' . $type . '_require_filesize'] = array(
      '#title' => t('Require max filesize'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('dfs_' . $type . '_require_filesize', FALSE),
    );

    $form[$type]['dfs_' . $type . '_max_filesize'] = array(
      '#title' => t('Max filesize'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dfs_' . $type . '_max_filesize', ''),
      '#description' => t('Default file maximum size (ex: 50 MB).'),
    );

    $form[$type]['dfs_' . $type . '_file_extensions'] = array(
      '#title' => t('File extensions'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dfs_' . $type . '_file_extensions', 'png, gif, jpg, jpeg'),
      '#description' => t('Default file extensions.'),
    );

  }

  $form['#submit'][] = 'default_file_settings_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Custom settings for the Default File Settings module.
 */
function default_file_settings_settings_form_submit($form, &$form_state) {
  $min_resolution = array(
    'x' => $form_state['values']['dfs_min_resolution_x'],
    'y' => $form_state['values']['dfs_min_resolution_y']
  );
  $max_resolution = array(
    'x' => $form_state['values']['dfs_max_resolution_x'],
    'y' => $form_state['values']['dfs_max_resolution_y']
  );
  variable_set('dfs_min_resolution', $min_resolution);
  variable_set('dfs_max_resolution', $max_resolution);
}
