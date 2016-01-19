<?php

/**
 * @file
 * Admin settings form for uploading Capture screens
 */

/**
 * Returns the screen title from a filename.
 *
 * @param string $screen
 *   Screen name.
 *   @see _janrain_capture_get_screens()
 *
 * @return string
 *   Readable, translated screen name.
 */
function _janrain_capture_get_screen_title($screen) {
  $parts = explode('.', $screen);
  $filename = $parts[0];

  if ($filename == 'signin') {
    $filename = 'Sign in';
  }

  $screen_title = ucfirst(str_replace('-', ' ', $filename));
  return t($screen_title);
}

/**
 * Form callback for customizing the Janrain Capture screens.
 */
function janrain_capture_admin_screens_settings($form, &$form_state) {
  $screens = _janrain_capture_get_screens();

  $cache_directory = 'public://janrain_capture_screens/cache';
  // The remote screens are stored locally in our own dir to avoid running
  // drupal_http_request during pageload. We have to ensure that the directory
  // exists.
  if (!file_prepare_directory($cache_directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    watchdog('janrain_capture', 'Failed to create screen cache directory: %directory', array('%directory' => $cache_directory), WATCHDOG_ERROR);
  }

  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['janrain_capture_screen'] = array(
    '#type' => 'vertical_tabs',
  );

  foreach ($screens as $screen) {
    list($screen_category, $extension) = explode('.', $screen);
    list($screen_category) = explode('/', $screen_category);
    $screen_machine_name = _janrain_capture_get_screen_machine_name($screen);

    if (!isset($form['janrain_capture_screen_' . $screen_category])) {
      $form['janrain_capture_screen_' . $screen_category] = array(
        '#type' => 'fieldset',
        '#title' => _janrain_capture_get_screen_title($screen),
        '#group' => 'janrain_capture_screen',
        '#description' => t('If you want to customize a screen you can either upload a file or provide a URL where it can be found.'),
      );
    }

    $form['janrain_capture_screen_' . $screen_category]['janrain_capture_screen_' . $screen_machine_name] = array(
      '#type' => 'managed_file',
      '#title' => $screen,
      '#upload_location' => 'public://janrain_capture_screens/',
      '#upload_validators' => array('file_validate_extensions' => array($extension)),
      '#description' => t('Allowed extensions: %extension' , array('%extension' => $extension)),
      '#default_value' => variable_get('janrain_capture_screen_' . $screen_machine_name),
    );
    $form['janrain_capture_screen_' . $screen_category]['janrain_capture_screen_' . $screen_machine_name . '_remote_url'] = array(
      '#type' => 'textfield',
      '#title' => t('!screen - remote URL', array('!screen' => $screen)),
      '#default_value' => variable_get('janrain_capture_screen_' . $screen_machine_name . '_remote_url'),
      '#description' => t('Example: http://www.example.com/<path to file>/file.extension.'),
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Validates the Janrain Capture screen remote URLs.
 */
function janrain_capture_admin_screens_settings_validate($form, &$form_state) {
  $screens = _janrain_capture_get_screens();
  foreach ($screens as $screen) {
    $screen_machine_name = _janrain_capture_get_screen_machine_name($screen);
    $screen_remote_url = trim($form_state['values']['janrain_capture_screen_' . $screen_machine_name . '_remote_url']);

    if (!empty($screen_remote_url)) {
      if (!valid_url($screen_remote_url, TRUE)) {
        form_set_error('janrain_capture_screen_' . $screen_machine_name . '_remote_url', t('Please specify a valid URL for !screen.', array('!screen' => $screen)));
      }
      else {
        $response = drupal_http_request($screen_remote_url);
        if ($response->code != 200) {
          form_set_error('janrain_capture_screen_' . $screen_machine_name . '_remote_url', t('Could not retrieve !screen from @url.', array(
            '!screen' => $screen,
            '@url' => $screen_remote_url,
          )));
        }
        else {
          $form_state['storage']['janrain_capture_screen_' . $screen_machine_name . '_remote_url_content'] = $response->data;
        }
      }
    }
  }
}

/**
 * Form submit callback for saving the uploaded Janrain capture screens.
 *
 * @see http://api.drupal.org/api/drupal/developer%21topics%21forms_api_reference.html/7#managed_file
 */
function janrain_capture_admin_screens_settings_submit(&$form, &$form_state) {
  $screens = _janrain_capture_get_screens();
  foreach ($screens as $screen) {
    $screen_machine_name = _janrain_capture_get_screen_machine_name($screen);
    $variable_name = "janrain_capture_screen_${screen_machine_name}";

    variable_set($variable_name, $form_state['values'][$variable_name]);

    $file = file_load(variable_get($variable_name));
    if ($file) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'user', 'user', 1);
    }

    // In case the user has specified a remote URL for the screen.
    $screen_remote_url = trim($form_state['values'][$variable_name . '_remote_url']);
    variable_set($variable_name . '_remote_url', $screen_remote_url);
    $screen_content = isset($form_state['storage']['janrain_capture_screen_' . $screen_machine_name . '_remote_url_content']) ? $form_state['storage']['janrain_capture_screen_' . $screen_machine_name . '_remote_url_content'] : NULL;
    janrain_capture_screens_local_screen_data_refresh($screen, $screen_content);
  }
}
