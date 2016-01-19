<?php

/**
 * Returns Contextly settings URL of specified type.
 *
 * @param string $type
 * @param null|string $access_token
 *   Is used to build direct login URL when specified.
 *
 * @return string
 */
function _contextly_settings_url($type) {
  $query = array(
    'type' => $type,
    'blog_url' => $GLOBALS['base_url'],
    'blog_title' => variable_get('site_name', 'Drupal'),
    'cms_settings_page' => url('admin/config/content/contextly/set-api-key', array(
      'query' => array(
        'token' => drupal_get_token(_contextly_settings_set_api_key_token_value()),
      ),
      'absolute' => TRUE,
    )),
  );

  // Try to get the auth token and use direct login.
  try {
    $token = ContextlyDrupalKit::getInstance()
      ->newApi()
      ->getAccessToken();
    $query += array(
      'contextly_access_token' => $token,
    );
  }
  catch (Exception $e) {
    // Just silently fail.
  }

  return url(_contextly_server_url('cp') . 'cms-redirect/', array('query' => $query));
}

/**
 * Settings form.
 */
function contextly_settings_form($form, &$state) {
  $key = variable_get('contextly_api_key', array());

  $form['module'] = array(
    '#type' => 'fieldset',
    '#title' => t('Module settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($key),
  );
  $form['module']['link'] = array(
    '#type' => 'item',
    '#markup' => t('In order to communicate securely, we use a shared secret key. You can find your secret API key on !home_link. Copy and paste it below.', array(
      '!home_link' => l(t("Contextly's settings panel"), 'admin/config/content/contextly/cp-tour', array(
        'query' => array(
          'token' => drupal_get_token(_contextly_settings_cp_token_value('tour')),
        ),
        'attributes' => array(
          'target' => '_blank',
        )
      )),
    )),
  );
  $form['module']['contextly_api_key'] = array(
    '#required' => TRUE,
    '#type' => 'textfield',
    '#title' => t('Secret API key'),
    '#default_value' => implode('-', $key),
  );
  $form['module']['contextly_kit_cdn'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load Kit resources from the Contextly CDN'),
    '#default_value' => variable_get('contextly_kit_cdn', TRUE),
  );
  $form['module']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array(
      'system_settings_form_submit',
      '_contextly_settings_reset_shared_token',
    ),
  );

  $form['service'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service settings'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($key),
  );
  $form['service']['notice'] = array(
    '#type' => 'item',
    '#markup' => t('The majority of the settings for Contextly are handled outside of Drupal. Click the settings button to securely login to your settings panel. If this fails, please email us at <a href="@url">info@contextly.com</a>.', array(
      '@url' => 'mailto:info@contextly.com',
    )),
  );
  $form['service']['go'] = array(
    '#type' => 'submit',
    '#value' => t('Settings'),
    '#attributes' => array(
      'class' => array('contextly-settings-button'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'contextly') . '/js/contextly-settings-form.js',
      ),
    ),
    '#submit' => array('contextly_settings_form_submit_contextly_settings'),

    // Empty validation to avoid unnecessary default form validation. Required
    // field needs limit_validation_errors anyway.
    '#validate' => array(),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

function _contextly_settings_validate_api_key($key) {
  // Parse string key into an array.
  $key = explode('-', $key, 2);
  if (count($key) !== 2) {
    return array(FALSE, t('API key is incorrect.'));
  }

  // Convert it into associative array for later use.
  $key = array_combine(array('appID', 'appSecret'), $key);

  try {
    // Check API key. Create API client with isolated session.
    $settings = ContextlyDrupalKit::getDefaultSettings();
    foreach ($key as $name => $value) {
      $settings->{$name} = $value;
    }
    $kit = new ContextlyDrupalKit($settings);
    $session = $kit->newApiSessionIsolated();
    $api = $kit->newApi($session);

    // Failed authorization should throw an exception at this point.
    $api->testCredentials();

    return array(TRUE, $key);
  }
  catch (Exception $e) {
    return array(FALSE, t('Test API request failed.'));
  }
}

/**
 * Validation for the settings form.
 */
function contextly_settings_form_validate($form, &$state) {
  $key = trim($state['values']['contextly_api_key']);
  list($success, $result) = _contextly_settings_validate_api_key($key);
  if ($success) {
    // Save parsed key back to values.
    $state['values']['contextly_api_key'] = $result;
  }
  else {
    form_set_error('contextly_api_key', $result);
  }
}

function _contextly_settings_reset_shared_token() {
  ContextlyDrupalKit::getInstance()
    ->newApiSession()
    ->removeSharedToken();
}

/**
 * Submit handler for the "Contextly settings" button of the settings form.
 *
 * Redirects to Contextly service settings page.
 */
function contextly_settings_form_submit_contextly_settings($form, &$state) {
  unset($_GET['destination']);
  $state['redirect'] = _contextly_settings_url('settings');
}

function _contextly_settings_cp_token_value($type) {
  return "contextly/cp/$type";
}

function _contextly_settings_set_api_key_token_value() {
  return "contextly/set-api-key";
}

function contextly_settings_cp_tour_redirect() {
  // Cleanup destination in any case.
  unset($_GET['destination']);

  if (empty($_GET['token'])) {
    return MENU_NOT_FOUND;
  }
  $token = $_GET['token'];

  $token_value = _contextly_settings_cp_token_value('tour');
  if (!drupal_valid_token($token, $token_value)) {
    drupal_set_message(t('Something went wrong. Please try again.'), 'warning');
    drupal_goto('admin/config/content/contextly');
  }

  drupal_goto(_contextly_settings_url('tour'));
}

function contextly_settings_set_api_key_redirect() {
  // Cleanup destination in any case.
  unset($_GET['destination']);

  if (empty($_GET['token']) || empty($_GET['api_key'])) {
    // Silently redirect to the settings page.
    drupal_goto('admin/config/content/contextly');
  }
  $token = $_GET['token'];
  $api_key = $_GET['api_key'];

  $token_value = _contextly_settings_set_api_key_token_value();
  if (!drupal_valid_token($token, $token_value)) {
    drupal_set_message(t('Something went wrong. Please try again.'), 'warning');
    drupal_goto('admin/config/content/contextly');
  }

  list($success, $result) = _contextly_settings_validate_api_key($api_key);
  if ($success) {
    variable_set('contextly_api_key', $result);
    _contextly_settings_reset_shared_token();
    drupal_set_message(t('Your API key has been successfully updated.'));
  }
  else {
    drupal_set_message($result, 'error');
  }
  drupal_goto('admin/config/content/contextly');
}

/**
 * Contextly-enabled node types settings form.
 */
function contextly_settings_node_types_form($form, &$state) {
  $types = node_type_get_names();
  $types = array_map('check_plain', $types);

  $form['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled node types'),
  );

  $form['types']['info'] = array(
    '#type' => 'item',
    '#markup' => t("Integration with Contextly will be enabled for selected node types only."), // TODO Move to help
  );

  $form['types']['contextly_all_node_types'] = array(
    '#type' => 'checkbox',
    '#title' => t('All node types.'),
    '#default_value' => variable_get('contextly_all_node_types', TRUE),
  );

  $form['types']['contextly_node_types'] = array(
    '#title' => t('Enabled types'),
    '#type' => 'checkboxes',
    '#options' => $types,
    '#default_value' => variable_get('contextly_node_types', array()),
    '#element_validate' => array('_contextly_multi_value_element_cleanup'),
    '#states' => array(
      'visible' => array(
        'input[name="contextly_all_node_types"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form = system_settings_form($form);

  // Custom handler to clear fields cache on submit.
  $form['#submit'][] = 'contextly_settings_node_types_form_submit_clear_cache';

  return $form;
}

/**
 * Submit handler for the node types settings form to clear field cache.
 *
 * Field cache must be cleared for Contextly widget to appear/disappear on the
 * node display management UI.
 */
function contextly_settings_node_types_form_submit_clear_cache($form, &$state) {
  field_cache_clear();
}

/**
 * Removes empty (not selected) values from multi-value form elements.
 */
function _contextly_multi_value_element_cleanup($element, &$state, $form) {
  $filtered_value = array();
  if (!empty($element['#value'])) {
    $filtered_value = array_filter($element['#value']);
  }
  form_set_value($element, $filtered_value, $state);
}

/**
 * Returns the list of field instances on Contextly-enabled node types.
 *
 * @param array $field_types
 *
 * @return array
 *   List of field names indexed by node type.
 */
function _contextly_settings_get_available_fields(array $field_types) {
  $enabled_types = _contextly_get_enabled_types();
  $available_fields = array();
  foreach ($enabled_types as $node_type) {
    $node_type_fields = _contextly_get_node_type_fields($node_type, $field_types);
    if (!empty($node_type_fields)) {
      $available_fields[$node_type] = $node_type_fields;
    }
  }
  return $available_fields;
}

function contextly_settings_tags_form($form, $state) {
  $form['tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy terms'),
  );

  $form['tags']['info'] = array(
    '#type' => 'item',
    '#markup' => t('Terms from selected fields will be sent to the Contextly.'), // TODO Move to help.
  );

  $form['tags']['contextly_all_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('All term reference fields'),
    '#default_value' => variable_get('contextly_all_tags', TRUE),
  );

  $available_fields = _contextly_settings_get_available_fields(array('taxonomy_term_reference'));
  $node_types = node_type_get_types();
  foreach ($available_fields as $type => $fields) {
    $options = array();
    foreach ($fields as $field_name) {
      $instance = field_info_instance('node', $field_name, $type);
      $options[$field_name] = check_plain($instance['label']);
    }

    $node_type = $node_types[$type];
    $form['tags']['contextly_tags__' . $type] = array(
      '#type' => 'checkboxes',
      '#title' => check_plain($node_type->name),
      '#options' => $options,
      '#default_value' => variable_get('contextly_tags__' . $type, array()),
      '#element_validate' => array('_contextly_multi_value_element_cleanup'),
      '#states' => array(
        'visible' => array(
          'input[name="contextly_all_tags"]' => array(
            'checked' => FALSE,
          ),
        ),
      ),
    );
  }

  return system_settings_form($form);
}

function contextly_settings_featured_images_form($form, &$state) {
  $available_fields = _contextly_settings_get_available_fields(array('image'));
  if (empty($available_fields)) {
    drupal_set_message(t('Add some image fields to Contextly-enabled node types to use featured images.'), 'warning');
    return $form;
  }

  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Featured images'),
  );

  $count = count($available_fields);
  $form['images']['info'] = array(
    '#type' => 'item',
    '#markup' => t('!these_fields you control what images will be used to create the thumbnails for Contextly recommendations. The first image from the selected field below will be used to create the thumbnail.', array(
      '!these_fields' => format_plural($count, 'This field lets', 'These fields let'),
    )), // TODO Move to help.
  );

  $node_types = node_type_get_types();
  foreach ($available_fields as $type => $fields) {
    $options = array();
    foreach ($fields as $field_name) {
      $instance = field_info_instance('node', $field_name, $type);
      $options[$field_name] = $instance['label'];
    }

    $node_type = $node_types[$type];
    $form['images']['contextly_featured_image__' . $type] = array(
      '#type' => 'select',
      '#title' => check_plain($node_type->name),
      '#options' => $options,
      '#empty_value' => '',
      '#default_value' => variable_get('contextly_featured_image__' . $type, ''),
    );
  }

  return system_settings_form($form);
}
