<?php

/**
 * @file
 * Administration pages and forms callbacks.
 */

/**
 * Module settings form callback.
 */
function dsb_portal_admin_settings_form($form, $form_state) {
  // Check that a private file system is configured. If it isn't, refuse to let
  // the administrator configure the module.
  if (!file_stream_wrapper_get_instance_by_uri('private://')) {
    drupal_set_message(t("You must configure a private file system in order to configure dsb Portal. You will be requested to upload a private key, which should never be stored in a web-accessible directory. You can configure the private file system under !link.", array(
      '!link' => l('admin/config/media/file-system', 'admin/config/media/file-system'),
    )), 'error');
    return array();
  }

  // Make sure dependencies are loaded before configuring the module.
  if (
    !dsb_portal_check_libraries(TRUE) ||
    !dsb_portal_check_module_dependency(TRUE)
  ) {
    drupal_set_message(t("Please correct the dependency errors before configuring dsb Portal."), 'error');
    return array();
  }

  // API settings.
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t("REST API settings"),
    '#collapsible' => TRUE,
    '#collapsed' =>
      variable_get('dsb_portal_api_username', 0) &&
      variable_get('dsb_portal_api_key_fid', 0) &&
      variable_get('dsb_portal_api_url', 0) &&
      (empty($form_state['triggering_element']['#name']) || $form_state['triggering_element']['#name'] !== 'test_connection'),
  );

  $form['api']['dsb_portal_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t("API URL"),
    '#default_value' => variable_get('dsb_portal_api_url', 'https://dsb-api.educa.ch/v2'),
    '#required' => TRUE,
  );

  $form['api']['dsb_portal_api_version'] = array(
    '#type' => 'radios',
    '#title' => t("API version"),
    '#options' => array(
      2 => '2.x',
    ),
    '#default_value' => variable_get('dsb_portal_api_version', 2),
    '#required' => TRUE,
  );

  $form['api']['dsb_portal_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t("API username"),
    '#default_value' => variable_get('dsb_portal_api_username', ''),
    '#required' => TRUE,
  );

  $form['api']['dsb_portal_api_key_fid'] = array(
    '#type' => 'managed_file',
    '#title' => t("API authentication private key"),
    '#description' => t("Upon registering for access to the national catalog, you received a username and a private/public RSA key pair. Upload the private key here."),
    '#default_value' => variable_get('dsb_portal_api_key_fid', ''),
    '#upload_location' => 'private://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('pem key private txt'),
    ),
  );

  $form['api']['dsb_portal_api_key_passphrase_tmp'] = array(
    '#type' => 'password',
    '#title' => t("API authentication private key passphrase"),
    '#description' => t("If your private key uses a passphrase, provide it here. This field will be left empty upon saving for security reasons. You do not need to provide it if it did not change."),
  );

  $form['api']['test_connection'] = array(
    '#type' => 'button',
    '#value' => t("Test connection"),
    '#name' => 'test_connection',
  );

  // Search form settings.
  $form['search_form'] = array(
    '#type' => 'fieldset',
    '#title' => t("Search form settings"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['search_form']['dsb_portal_search_form_expose_form_in_block'] = array(
    '#type' => 'checkbox',
    '#title' => t("Expose the search form in a block ?"),
    '#description' => t("If checked, the search form will not be shown on the search page, but exposed in a new block. Make sure to enable the block in order to display the form."),
    '#default_value' => variable_get('dsb_portal_search_form_expose_form_in_block', 0),
  );

  // Search settings.
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t("Search settings"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['search']['dsb_portal_search_limit'] = array(
    '#type' => 'select',
    '#title' => t("Maximum number of items to show on search requests"),
    '#options' => array(
      10 => 10,
      20 => 20,
      30 => 30,
      40 => 40,
      50 => 50,
    ),
    '#default_value' => variable_get('dsb_portal_search_limit', 20),
  );

  $form['search']['dsb_portal_search_sort_by'] = array(
    '#type' => 'select',
    '#title' => t("Sort results by"),
    '#description' => t("When choosing <em>Relevance / last modified date</em>, results will be sorted by relevance first, and then by date. Relevance is only taken into account for full-text searches. Be aware that, when choosing <em>Random</em>, the results will not be paginated."),
    '#options' => array(
      'relevance' => t("Relevance / last modified date"),
      'random' => t("Random"),
    ),
    '#default_value' => variable_get('dsb_portal_search_sort_by', 'relevance'),
  );

  $form['search']['dsb_portal_search_only_current_lang'] = array(
    '#type' => 'checkbox',
    '#title' => t("Filter search results based on user's browsing session language ?"),
    '#description' => t("If checked, a user browsing the site in French will also see French results. If not checked, the user will see results in all languages."),
    '#default_value' => variable_get('dsb_portal_search_only_current_lang', 0),
  );

  $form['search']['dsb_portal_search_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t("Enable debug mode ?"),
    '#description' => t("If checked, all requests to the API will be logged. This can quickly overwhelm the logs, so don't activate unless necessary."),
    '#default_value' => variable_get('dsb_portal_search_debug_mode', 0),
  );

  $form['#submit'][] = 'dsb_portal_admin_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Validate callback for dsb_portal_admin_settings_form().
 */
function dsb_portal_admin_settings_form_validate($form, $form_state) {
  if ($form_state['triggering_element']['#name'] === 'test_connection') {
    $test_connection = TRUE;
    foreach (array(
      'dsb_portal_api_url',
      'dsb_portal_api_version',
      'dsb_portal_api_username',
      'dsb_portal_api_key_fid',
    ) as $key) {
      if (empty($form_state['values'][$key])) {
        form_set_error($key);
        $test_connection = FALSE;
      }
    }

    if ($test_connection) {
      $api_version = $form_state['values']['dsb_portal_api_version'];
      $api_url = $form_state['values']['dsb_portal_api_url'];
      $username = $form_state['values']['dsb_portal_api_username'];
      $key_fid = $form_state['values']['dsb_portal_api_key_fid'];
      $key_passphrase = $form_state['values']['dsb_portal_api_key_passphrase_tmp'];

      if (empty($key_passphrase)) {
        $key_passphrase = variable_get('dsb_portal_api_key_passphrase', '');
      }

      try {
        $client = dsb_portal_get_client(
          $api_version,
          $api_url,
          $username,
          $key_fid,
          $key_passphrase
        );

        $client->authenticate();

        drupal_set_message(t("Authentication was successful."));
      }
      catch (Exception $e) {
        drupal_set_message(t("The connection failed. Error message: @message", array(
          '@message' => $e->getMessage(),
        )), 'error');
      }
    }
    else {
      drupal_set_message(t("Could not test connection. Please provide all necessary connection information."), 'error');
    }
  }
  else {
    // Other validation.
  }
}

/**
 * Submit callback for dsb_portal_admin_settings_form().
 */
function dsb_portal_admin_settings_form_submit($form, $form_state) {
  global $user;

  // Store the private key permanently.
  $file = file_load($form_state['values']['dsb_portal_api_key_fid']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  file_usage_add($file, 'user', 'user', $user->uid);

  // Store the passphrase, if it was set.
  if (!empty($form_state['values']['dsb_portal_api_key_passphrase_tmp'])) {
    variable_set('dsb_portal_api_key_passphrase', $form_state['values']['dsb_portal_api_key_passphrase_tmp']);
  }
}
