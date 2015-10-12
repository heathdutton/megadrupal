<?php

/**
 * @file
 * Provides Admin interface to osCaddie GCS.
 */

/**
 * Implements hook_admin().
 */
function oscaddie_gcs_admin($form, &$form_state) {
  $errors = FALSE;
  $openssl = extension_loaded('openssl');
  $directory = 'private://';
  $private_directory = file_prepare_directory($directory, 'FILE_EXIST');
  $info = libraries_load('google-api-php-client');

  if (!$openssl || !$private_directory || (isset($info['loaded']) && !$info['loaded'])) {
    $errors = TRUE;
    $form['oscaddie_gcs_warning_details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configuration Issues'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $documentation_link = l(t('documentation'), OSCADDIE_GCS_ONLINE_DOCUMENTATION_LINK);
    $form['oscaddie_gcs_warning_details']['general'] = array(
      '#type' => 'item',
      '#markup' => t('We have detected issues with the requirements of the
        modules installation. Please fix the issues below before continuing
        with the configuration of the module. Check the online !documentation
        and/or readme.txt located in the module folder.',
        array('!documentation' => $documentation_link)),
    );
    $error_messages = array();
    if (isset($info['loaded']) && !$info['loaded']) {
      $error_messages[] = t('Unable to load the Google API PHP Library.
        Please download and install the library.');
    }
    if (!$openssl) {
      $error_messages[] = t('Unable to detect the OpenSSL PHP module.
        Ensure this module is installed on your server.');
    }
    if (!$private_directory) {
      $file_system_link = l(t('here'), 'admin/config/media/file-system');
      $error_messages[] = t('The private directory is not writable.
        Please configure this directory !here.', array('!here' => $file_system_link));
    }
    $list = theme('item_list', array(
      'items' => $error_messages,
      'title' => t('Issues Found'),
      'type' => 'ul',
    ));
    $form['oscaddie_gcs_warning_details']['errors'] = array(
      '#type' => 'item',
      '#markup' => $list,
    );
  }

  $form['oscaddie_gcs_connection_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection Details'),
    '#collapsible' => TRUE,
    '#collapsed' => $errors,
  );

  $form['oscaddie_gcs_connection_details']['oscaddie_gcs_public_key_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Public Key'),
    '#default_value' => variable_get('oscaddie_gcs_public_key'),
    '#description' => t('The public p12 key that allows access to
      your Google Cloud Storage account.'),
    '#required' => TRUE,
    '#upload_validators' => array(
      'file_validate_extensions' => array('p12'),
    ),
    '#upload_location' => 'private://oscaddie_gcs/',
    '#process' => array('oscaddie_gcs_admin_public_key_file_element_process'),
  );

  $form['oscaddie_gcs_connection_details']['oscaddie_gcs_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Service Account Client ID'),
    '#default_value' => variable_get('oscaddie_gcs_client_id'),
    '#description' => t('The service account client ID credential. Should
      follow this pattern "xxxxxxxxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.apps.googleusercontent.com"'),
    '#required' => TRUE,
  );

  $form['oscaddie_gcs_connection_details']['oscaddie_gcs_service_account_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service Account Name'),
    '#default_value' => variable_get('oscaddie_gcs_service_account_name'),
    '#description' => t('The service account name credential. This is the email
      address that should look like xxxxxxxxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx@developer.gserviceaccount.com'),
    '#required' => TRUE,
  );

  $form['oscaddie_gcs_connection_details']['oscaddie_gcs_bucket_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Bucket Name'),
    '#default_value' => variable_get('oscaddie_gcs_bucket_name'),
    '#description' => t('The bucket name within Google Cloud Storage. This
      module only supports one bucket currently.'),
    '#required' => TRUE,
  );

  $form['oscaddie_gcs_connection_details']['oscaddie_gcs_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Folder Name'),
    '#default_value' => variable_get('oscaddie_gcs_folder'),
    '#description' => t('The folder name within Google Cloud Storage.
      Required in order to separate out environments. For example, local,
      dev, staging, prod etc.'),
    '#required' => TRUE,
  );

  $form['oscaddie_gcs_library_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library Options'),
    '#collapsible' => TRUE,
    '#collapsed' => $errors,
  );

  $form['oscaddie_gcs_library_options']['oscaddie_gcs_signed_url_expiry'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiry'),
    '#default_value' => variable_get('oscaddie_gcs_signed_url_expiry'),
    '#description' => t('The amount of time in seconds that the
      signed url will expire. (Example: 300 = 5 minutes)'),
    '#required' => TRUE,
  );

  if (variable_get('oscaddie_gcs_public_key', NULL) != NULL) {
    $form['test_connection'] = array(
      '#type' => 'submit',
      '#value' => t('@test', array('@test' => OSCADDIE_GCS_ADMIN_TEST_CONNECTION_BUTTON)),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('@save', array('@save' => OSCADDIE_GCS_ADMIN_SUBMIT_BUTTON)),
    '#submit' => array('oscaddie_gcs_admin_submit'),
  );

  return $form;
}

/**
 * Hides the default upload button.
 *
 * @param array $element
 *   The remove element.
 * @param array $form_state
 *   The form state.
 * @param array $form
 *   The form array.
 *
 * @return mixed
 *   the form element.
 */
function oscaddie_gcs_admin_public_key_file_element_process(array $element, array &$form_state, array $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element['upload_button']['#access'] = FALSE;
  if (isset($element['remove_button']['#submit'])) {
    $element['remove_button']['#submit'][] = 'oscaddie_gcs_delete_key_file';
  }
  return $element;
}

/**
 * Validates the admin form.
 *
 * Tests the credentials the user entered and responds with a message.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form_state array.
 */
function oscaddie_gcs_admin_validate(array $form, array &$form_state) {
  if (isset($form_state['clicked_button']) && $form_state['clicked_button']['#value'] == OSCADDIE_GCS_ADMIN_TEST_CONNECTION_BUTTON) {
    $info = libraries_load('google-api-php-client');
    if ($info['loaded']) {
      $response = oscaddie_gcs_test_connection();
      if (empty($response)) {
        form_set_error('oscaddie_gcs_public_key_file', t('Error authenticating. Please check the Service Account Name, Key file and Bucket Name.'));
      }
      if ($response['access_token'] && $response['bucket']) {
        drupal_set_message(t('Success authenticating! We were able to connect to your Google Cloud Storage App.'));
      }
    }
    else {
      form_set_error('form', t("The Google API PHP Client library was unable to be loaded."));
    }
  }
}

/**
 * Submit handler for admin form.
 *
 * Saves the variables and p12 key file. Also creates a PEM file for later use
 * in regards to signed urls.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form_state array.
 */
function oscaddie_gcs_admin_submit(array $form, array &$form_state) {
  if ($form_state['clicked_button']['#value'] == OSCADDIE_GCS_ADMIN_SUBMIT_BUTTON) {
    variable_set('oscaddie_gcs_bucket_name', $form_state['values']['oscaddie_gcs_bucket_name']);
    variable_set('oscaddie_gcs_folder', $form_state['values']['oscaddie_gcs_folder']);
    variable_set('oscaddie_gcs_client_id', $form_state['values']['oscaddie_gcs_client_id']);
    variable_set('oscaddie_gcs_service_account_name', $form_state['values']['oscaddie_gcs_service_account_name']);
    variable_set('oscaddie_gcs_signed_url_expiry', $form_state['values']['oscaddie_gcs_signed_url_expiry']);
    oscaddie_gcs_admin_save_key_file($form, $form_state);
  }
}

/**
 * Saves the key file.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form_state array.
 */
function oscaddie_gcs_admin_save_key_file(array &$form, array &$form_state) {
  // Load the file.
  $file = file_load($form_state['values']['oscaddie_gcs_public_key_file']);
  // Change status to permanent.
  $file->status = FILE_STATUS_PERMANENT;
  $file->replace = FILE_EXISTS_REPLACE;
  $uploaded = file_save($file);
  global $user;
  $account = $user;
  // Record that the module is using the file.
  file_usage_add($file, 'oscaddie_gcs', 'admin', $account->uid);
  if ($uploaded) {
    $real_path = drupal_realpath($file->uri);
    variable_set('oscaddie_gcs_public_key', $file->fid);
    variable_set('oscaddie_gcs_public_key_file_path', $real_path);
    oscaddie_gcs_admin_save_pem_file($file, $real_path);
  }
}

/**
 * Creates and saves a PEM file.
 *
 * @param object $file
 *   The file object.
 * @param string $real_path
 *   The path to the file.
 */
function oscaddie_gcs_admin_save_pem_file($file, $real_path) {
  $path_to_pem = str_replace($file->filename, '', $real_path);
  $path_to_pem .= 'google_key.pem';
  $uri_to_pem = "private://oscaddie_gcs/google_key.pem";

  if (!file_exists($path_to_pem)) {
    $password = OSCADDIE_GCS_PROVIDED_KEY;
    $pkey_result = array();
    $pkcs12_read = openssl_pkcs12_read(file_get_contents($real_path), $pkey_result, $password);
    if ($pkcs12_read) {
      $new_password = OSCADDIE_GCS_PROVIDED_KEY;
      $pem_result = NULL;
      $pem_export = openssl_pkey_export($pkey_result['pkey'], $pem_result, $new_password);
      if ($pem_export) {
        // Save the file.
        file_put_contents($path_to_pem, $pem_result);

        global $user;
        // Create a record in managed files.
        $file = new stdClass();
        $file->fid = NULL;
        $file->uri = $uri_to_pem;
        $file->filename = drupal_basename($uri_to_pem);
        $file->filemime = file_get_mimetype($file->uri);
        $file->uid      = $user->uid;
        $file->status   = FILE_STATUS_PERMANENT;
        $file->replace = FILE_EXISTS_REPLACE;
        $file->size = filesize($path_to_pem);
        file_save($file);

        // Save information for later use.
        variable_set('oscaddie_gcs_pem_file_path', $path_to_pem);
        variable_set('oscaddie_gcs_pem_fid', $file->fid);
      }
    }
  }
}

/**
 * Remove the PEM file. Called when the user deletes the p12 key.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form_state array.
 */
function oscaddie_gcs_delete_key_file(array $form, array &$form_state) {
  $pem_fid = variable_get('oscaddie_gcs_pem_fid');
  if ($pem_fid) {
    $file = file_load($pem_fid);
    if ($file) {
      file_delete($file);
      variable_del('oscaddie_gcs_pem_fid');
      variable_del('oscaddie_gcs_pem_file_path');
    }
    unset($file);
  }
  $p12_fid = variable_get('oscaddie_gcs_public_key');
  if ($p12_fid) {
    $file = file_load($p12_fid);
    if ($file) {
      file_usage_delete($file, 'oscaddie_gcs');
      file_delete($file);
      variable_del('oscaddie_gcs_public_key');
      variable_del('oscaddie_gcs_public_key_file_path');
    }
    unset($file);
  }
  // Just in case, migrating sometimes leaves files behind.
  db_delete('file_managed')->condition('filename', '886355e5443f9575c7ddc9740191a7b43bafd5aa-privatekey.p12');
  db_delete('file_managed')->condition('filename', 'google_key.pem');
}
