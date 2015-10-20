<?php

/**
 * @file
 * Administration functionality for the Climate Tagger.
 */

/**
 * The Climate Tagger configuration form.
 */
function climate_tagger_admin_settings($form, &$form_state) {
  $conf = climate_tagger_get_config();

  $form['#submit'] = array('climate_tagger_admin_settings_submit');

  // Define the container for the vertical tabs.
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['server_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server'),
    '#group' => 'settings',
  );

  // Display any unfulfilled module requirement.
  $requirement_errors = powertagging_check_requirements();
  if (!empty($requirement_errors)) {
    $requirement_errors_markup = '';
    foreach ($requirement_errors as $requirement_error) {
      $requirement_errors_markup .= '<div class="messages error">' . $requirement_error . '</div>';
    }
    $form['server_settings']['requirements'] = array(
      '#type' => 'item',
      '#markup' => $requirement_errors_markup,
    );
  }

  $form['server_settings']['climate_tagger_reegle_api_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication token for the Climate Tagger API'),
    '#description' => t('A valid authentication token which has been generated in the Climate Tagger API dashboard.'),
    '#size' => 60,
    '#maxlength' => 60,
    '#default_value' => $conf->token,
    '#required' => TRUE,
  );

  $form['server_settings']['health_check'] = array(
    '#type' => 'button',
    '#value' => t('Health check'),
    '#ajax' => array(
      'callback' => 'climate_tagger_health_check',
      'wrapper' => 'health_info',
      'method' => 'replace',
      'effect' => 'slide',
    ),
  );

  $form['server_settings']['health_info'] = array(
    '#type' => 'item',
    '#markup' => '<div id="health_info">Click to check if the server is available.</div>',
  );

  // Tab: Project settings.
  $form['project_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project options'),
    '#group' => 'settings',
    '#tree' => TRUE,
  );

  $available_languages = array(
    LANGUAGE_NONE => (object) array(
      'language' => LANGUAGE_NONE,
      'name' => 'Language neutral',
      'enabled' => TRUE,
    ),
  );
  $available_languages = array_merge($available_languages, language_list());

  // Language mapping.
  $form['project_settings']['label'] = array(
    '#type' => 'hidden',
    '#value' => 'Climate Tagger API',
  );

  $climate_tagger_languages = array('de', 'pt', 'fr', 'en', 'es');
  $language_options = array();
  foreach ($climate_tagger_languages as $climate_tagger_language) {
    $language_options[$climate_tagger_language] = $climate_tagger_language;
  }

  $project_conf = array();
  if (isset($conf->config['projects'])) {
    $project_keys = array_keys($conf->config['projects']);
    $project_conf = $conf->config['projects'][$project_keys[0]];
  }

  foreach ($available_languages as $lang) {
    if ($lang->enabled) {
      $form['project_settings']['languages'][$lang->language] = array(
        '#type' => 'select',
        '#title' => $lang->name,
        '#options' => $language_options,
        '#empty_option' => '',
        '#default_value' => (isset($project_conf['languages'][$lang->language]) ? $project_conf['languages'][$lang->language] : (($lang->language == LANGUAGE_NONE) ? (in_array(language_default('language'), $climate_tagger_languages) ? language_default('language') : '') : (in_array($lang->language, $climate_tagger_languages) ? $lang->language : ''))),
      );
    }
  }

  // Taxonomy selection.
  $form['project_settings']['taxonomy_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal taxonomy'),
    '#description' => t('The name of the taxonomy where the extracted tags of the Climate Tagger API should be stored.</br >If the taxonomy does not exist, it will be created automatically.'),
    '#autocomplete_path' => 'powertagging/autocomplete_taxonomies',
    '#default_value' => isset($project_conf['taxonomy_name']) ? $project_conf['taxonomy_name'] : t('Climate Thesaurus'),
    '#required' => TRUE,
  );

  // Tab: Global limits
  $form['climate_tagger_global_limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global limits'),
    '#group' => 'settings',
  );

  powertagging_global_limits_form($form['climate_tagger_global_limits'], $conf->config['concepts_per_extraction'], $conf->config['concepts_threshold'], $conf->config['freeterms_per_extraction'], $conf->config['freeterms_threshold']);
  $form['climate_tagger_global_limits']['freeterms']['freeterms_per_extraction']['#description'] .= '<br />(' . t('Please note these are not currently in the Climate Thesaurus and can cause some tagging inconsistencies') . ')';

  if (isset($conf->powertagging_id)) {
    $field_instances = powertagging_get_tagging_field_instances($conf->powertagging_id, 'reegle_tagging_api');
    $content_type_options = climate_tagger_get_tagging_field_options_list($field_instances);

    // Search for all content types with the Climate Tagger field.
    if (!empty($field_instances)) {
      $form['climate_tagger_global_limits']['title'] = array(
        '#markup' => '<label>' . t('Content types with "Climate Tagger Tags" fields') . '</label>',
      );
      $form['climate_tagger_global_limits']['select_all_content_types'] = array(
        '#type' => 'checkbox',
        '#title' => t('Select all'),
        '#attributes' => array(
          'onclick' => 'jQuery(".form-item-overwrite-content-types").find("input").attr("checked", jQuery(this).attr("checked"));',
        ),
      );
      $form['climate_tagger_global_limits']['overwrite_content_types'] = array(
        '#type' => 'checkboxes',
        '#description' => t('Select the content types which ones you want to overwrite with the global limit values defined above.'),
        '#options' => $content_type_options,
      );
    }
  }

  // Tab: Batch Jobs.
  $form['batch_jobs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Batch jobs'),
    '#group' => 'settings',
    '#tree' => TRUE,
  );

  if (isset($conf->powertagging_id)) {
    if (!empty($field_instances)) {
      $form['batch_jobs']['content_types'] = array(
        '#title' => t('Entity types to be included in the batch process.'),
        '#type' => 'checkboxes',
        '#options' => $content_type_options,
      );

      $form['batch_jobs']['skip_tagged_content'] = array(
        '#title' => t('Skip already tagged content'),
        '#type' => 'radios',
        '#options' => array(
          '1' => t('Yes'),
          '0' => t('No'),
        ),
        '#default_value' => TRUE,
      );

      $form['batch_jobs']['entities_per_request'] = array(
        '#type' => 'textfield',
        '#title' => t('Entities per request'),
        '#description' => t('The number of entities, that get processed during one HTTP request. (Allowed value range: 1 - 100)') . '<br />' . t('The higher this number is, the less HTTP requests have to be sent to the server until the batch finished tagging ALL your entities, what results in a shorter duration of the bulk tagging process.') . '<br />' . t('Numbers too high can result in a timeout, which will break the whole bulk tagging process.') . '<br />' . t('If entities are configured to get tagged with uploaded files, a value of 5 or below is recommended.'),
        '#required' => TRUE,
        '#default_value' => '10',
      );

      $form['batch_jobs']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Index',
        '#validate' => array('climate_tagger_admin_settings_batch_validate'),
        '#submit' => array('climate_tagger_admin_settings_batch_submit'),
      );
    }
    else {
      $form['batch_jobs']['error'] = array(
        '#type' => 'item',
        '#markup' => '<div class="messages warning">' . t('No taggable content types found for the Climate Tagger API.') . '</div>',
      );
    }
  }
  else {
    $form['batch_jobs']['error'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages warning">' . t('The configuration needs to be saved before a batch process can be started.') . '</div>',
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/climate-tagger',
  );

  return $form;
}

/**
 * Submit-handler for the Climate Tagger configuration form.
 */
function climate_tagger_admin_settings_submit($form, &$form_state) {
  $powertagging_id = climate_tagger_get_powertagging_id();

  // Create a form-state, that can be used to create or update the powertagging
  // configuration for the Climate Tagger API.
  $new_form_state = array();
  form_load_include($new_form_state, 'inc', 'powertagging', 'includes/powertagging.admin');
  $new_form_state['values'] = array_merge($form_state['values'], array(
    'title' => 'Climate Tagger API',
    'server_title' => 'Climate Tagger API Server',
    'url' => CLIMATE_TAGGER_API_URL,
    'name' => '',
    'pass' => '',
  ));

  // A Powertagging-configuration needs to be created first.
  if ($powertagging_id == FALSE) {
    // Create a new powertagging configuration over Drupal's Form API.
    drupal_form_submit('powertagging_form', $new_form_state);

    $errors = form_get_errors();
    if (!empty($errors)) {
      foreach ($errors as $error) {
        $msg = 'Error: %error';
        $var = array('%error' => $error);
        watchdog('climate_tagger', $msg, $var, WATCHDOG_ERROR);
      }
      return FALSE;
    }

    // Get the ID of the powertagging configuration we just created.
    $powertagging_id = climate_tagger_get_powertagging_id();

    // If powertagging-id is still false here, break everything (error).
    if ($powertagging_id == FALSE) {
      drupal_set_message(t('Could not create a Powertagging configuration for the Climate Tagger API'), 'error');
      return FALSE;
    }
  }

  // Update the powertagging-configuration.
  $powertagging_config = powertagging_config_load($powertagging_id);

  // Set the connection id.
  $new_form_state['values']['connection_id'] = $powertagging_config->connection_id;

  // Add Climate Tagger as a project.
  $new_form_state['values']['projects'] = array(
    'reegle_tagging_api' => $form_state['values']['project_settings'],
  );

  $new_form_state['build_info']['args'] = array($powertagging_config);
  drupal_form_submit('powertagging_form', $new_form_state);

  // Clear the status-messages of the powertagging-config-creation.
  drupal_get_messages('status');

  $errors = form_get_errors();
  if (!empty($errors)) {
    foreach ($errors as $error) {
      $msg = 'Error: %error';
      $var = array('%error' => $error);
      watchdog('climate_tagger', $msg, $var, WATCHDOG_ERROR);
    }
    return FALSE;
  }

  // Overwrite content type limits with the global limits for
  // each selected content type.
  if (isset($form_state['values']['overwrite_content_types'])) {
    $overwrite_content_types = array_filter($form_state['values']['overwrite_content_types']);
    if (!empty($overwrite_content_types)) {
      foreach ($overwrite_content_types as $content_type) {
        list($entity_type, $bundle_name, $field_name) = explode(' ', $content_type);
        $instance = field_info_instance($entity_type, $field_name, $bundle_name);
        $limits = array(
          'concepts_per_extraction' => array('value' => $form_state['values']['concepts_per_extraction']['value']),
          'concepts_threshold' => array('value' => $form_state['values']['concepts_threshold']['value']),
          'freeterms_per_extraction' => array('value' => $form_state['values']['freeterms_per_extraction']['value']),
          'freeterms_threshold' => array('value' => $form_state['values']['freeterms_threshold']['value']),
        );
        $instance['settings'] = array_merge($instance['settings'], $limits);
        field_update_instance(($instance));
      }
    }
  }

  // Powertagging configuration is saved --> now go on with Climate Tagger API specific
  // variables.
  variable_set('climate_tagger_reegle_api_token', $form_state['values']['climate_tagger_reegle_api_token']);
  drupal_set_message(t('Successfully saved the configuration of Climate Tagger.'));
}

/**
 * Ajax callback function for checking the connection to the Climate Tagger API.
 */
function climate_tagger_health_check($form, $form_state) {
  $available = '<div id="health_info" class="available">' . t('The server is available.') . '</div>';
  $not_available = '<div id="health_info" class="not-available">' . t('The server is not available or the credentials are incorrect.') . '</div>';

  if (isset($form_state['values']['climate_tagger_reegle_api_token']) && !empty($form_state['values']['climate_tagger_reegle_api_token'])) {
    // Save the previous token.
    $previous_token = variable_get('climate_tagger_reegle_api_token', '');
    variable_set('climate_tagger_reegle_api_token', $form_state['values']['climate_tagger_reegle_api_token']);

    $connection = SemanticConnector::getConnection('pp_server');
    $connection->setURL(CLIMATE_TAGGER_API_URL);
    $pp_api = $connection->getAPI('PPX');
    $availability = $pp_api->available();

    // Restore the previous token.
    variable_set('climate_tagger_reegle_api_token', $previous_token);

    if (isset($availability['message']) && !empty($availability['message'])) {
      return '<div id="health_info" class="not-available">' . $availability['message'] . '</div>';
    }
    else {
      return $availability['success'] ? $available : $not_available;
    }
  }

  return $not_available;
}

/**
 * Validation-handler for the batch process in the Climate Tagger config form.
 */
function climate_tagger_admin_settings_batch_validate($form, &$form_state) {
  $valid_content_types = array();
  foreach ($form_state['values']['batch_jobs']['content_types'] as $content_type) {
    if ($content_type) {
      $valid_content_types[] = $content_type;
    }
  }

  if (empty($valid_content_types)) {
    form_set_error('batch_jobs][content_types', t('Select at least one content type to be included in the batch process.'));
  }

  $entities_per_request = $form_state['values']['batch_jobs']['entities_per_request'];
  if (empty($entities_per_request) || !ctype_digit($entities_per_request) || (int) $entities_per_request == 0 || (int) $entities_per_request > 100) {
    form_set_error('batch_jobs][entities_per_request', t('Only values in the range of 1 - 100 are allowed for field "Entities per request"'));
  }
}

/**
 * Submit-handler for the batch process in the Climate Tagger config form.
 */
function climate_tagger_admin_settings_batch_submit($form, &$form_state) {
  $content_types = array();
  foreach ($form_state['values']['batch_jobs']['content_types'] as $content_type) {
    if ($content_type) {
      $content_types[] = $content_type;
    }
  }

  powertagging_indexProject(climate_tagger_get_config(), 'reegle_tagging_api', array(
    'content_types' => $content_types,
    'skip_tagged_content' => $form_state['values']['batch_jobs']['skip_tagged_content'],
    'entities_per_request' => $form_state['values']['batch_jobs']['entities_per_request'],
  ), 'climate_tagger_batch_finished');
}
