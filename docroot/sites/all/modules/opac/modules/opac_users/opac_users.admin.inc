<?php
/**
 * @file
 * Confirguration forms for opac users module.
 */

/**
 * Form constructor for opac users settings form.
 *
 * @see opac_users_settings_form_submit()
 * @ingroup forms
 */
function opac_users_settings_form($form, $form_state) {
  global $user;
  $form['auth_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['auth_fieldset']['opac_users_auth_method'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('None, library authentification is disabled.', array(), array('context' => 'libraries')),
      1 => t('Use drupal account as library account.', array(), array('context' => 'libraries')),
      2 => t('Additional account for library.', array(), array('context' => 'libraries')),
    ),
    '#default_value' => variable_get('opac_users_auth_method', 0),
    '#title' => t('Which authentication method would you use?'),
    '#ajax' => array(
      'callback' => 'opac_users_settings_form_auth_method_callback',
      'wrapper' => 'auth_fields',
      'method' => 'replace',
    ),
  );

  // Descriptions, indexed by key.
  $descriptions = array(
    0 => t('Users could not view their holds or checkouts, hold or cancel items etc.', array(), array('context' => 'libraries')),
    1 => t('Drupal login and password are the same than library login and password. This avoid users to set additional account.', array(), array('context' => 'libraries')),
    2 => t('Means users could set an additional login/password for the libray.', array(), array('context' => 'libraries')),
  );

  foreach ($form['auth_fieldset']['opac_users_auth_method']['#options'] as $key => $label) {
    $form['auth_fieldset']['opac_users_auth_method'][$key]['#description'] = $descriptions[$key];
  }

  if (isset($form_state['values'])) {
    $opac_users_auth_method = $form_state['values']['opac_users_auth_method'];
  }
  else {
    $opac_users_auth_method = variable_get('opac_users_auth_method', 0);
  }

  $form['auth_fieldset']['fields'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="auth_fields">',
    '#suffix' => '</div>',
  );
  if ($opac_users_auth_method != 0) {
    $fields_options = array(
      '' => '-- ' . t('None') . ' --',
    );
    foreach (field_info_fields() as $field_name => $field) {
      if (isset($field['bundles']['user'])) {
        $instance = field_info_instance('user', $field_name, 'user');
        $fields_options[$field_name] = $instance['label'];
      }
    }
    $form['auth_fieldset']['fields']['opac_users_server_field'] = array(
      '#type' => 'select',
      '#title' => t('User field used to store OPAC server identifier', array(), array('context' => 'libraries')),
      '#options' => $fields_options,
      '#default_value' => variable_get('opac_users_server_field', ''),
      '#required' => TRUE,
    );
    if ($opac_users_auth_method == 2) {
      $form['auth_fieldset']['fields']['opac_users_login_field'] = array(
        '#type' => 'select',
        '#title' => t('User field used to store OPAC login', array(), array('context' => 'libraries')),
        '#options' => $fields_options,
        '#default_value' => variable_get('opac_users_login_field', ''),
        '#required' => TRUE,
      );
      $form['auth_fieldset']['fields']['opac_users_password_field'] = array(
        '#type' => 'select',
        '#title' => t('User field used to store OPAC password', array(), array('context' => 'libraries')),
        '#options' => $fields_options,
        '#default_value' => variable_get('opac_users_password_field', ''),
        '#required' => TRUE,
      );
    }
    $form['auth_fieldset']['opac_users_debug_mode'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('opac_users_debug_mode', 0),
      '#title' => t("Authentication debug mode"),
      '#description' => t('If checked, show a warning on each page of the site when user is not authenticated.'),
    );
    $form['auth_fieldset']['opac_users_redirect'] = array(
      '#type' => 'radios',
      '#options' => array(
        'user/login' => t('the login page', array(), array('context' => 'libraries')),
        'user/register' => t('the registration page', array(), array('context' => 'libraries')),
      ),
      '#default_value' => variable_get('opac_users_redirect', 'user/login'),
      '#title' => t('When a user is not connected, on which page should he be redirected to?'),
      '#required' => TRUE,
    );

  }

  $form['pages_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('User pages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // User pages.
  $pages = array(
    'holds' => array(
      'default_title' => 'My holds',
      'default_path' => 'opac-user/holds',
      'description' => 'Users can view their holds list in this page.',
    ),
    'checkouts' => array(
      'default_title' => 'My checkouts',
      'default_path' => 'opac-user/checkouts',
      'description' => 'Users can view their checkouts list in this page.',
    ),
    'issues_history' => array(
      'default_title' => 'My issues history',
      'default_path' => 'opac-user/issues-history',
      'description' => 'Users can view their issues history in this page.',
    ),
  );
  foreach ($pages as $id => $page) {
    $default_title = t($page['default_title'], array(), array('context' => 'libraries'));
    $description = t($page['description'], array(), array('context' => 'libraries'));
    $default_path = $page['default_path'];
    $form['pages_fieldset']["opac_users_${id}_page_allowed"] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get("opac_users_${id}_page_allowed", TRUE),
      '#title' => t("Enable <em>@default_title</em> page", array('@default_title' => $default_title), array('context' => 'libraries')),
      '#description' => $description,
    );
    $form['pages_fieldset']["opac_users_${id}_page_path"] = array(
      '#type' => 'textfield',
      '#title' => t('@default_title page path', array('@default_title' => $default_title), array('context' => 'libraries')),
      '#default_value' => variable_get("opac_users_${id}_page_path", $default_path),
      '#states' => array(
        'visible' => array(
          ":input[name='opac_users_${id}_page_allowed']" => array('checked' => TRUE),
        ),
      ),
    );
    $form['pages_fieldset']["opac_users_${id}_page_title"] = array(
      '#type' => 'textfield',
      '#title' => t('@default_title page title', array('@default_title' => $default_title), array('context' => 'libraries')),
      '#default_value' => variable_get("opac_users_${id}_page_title", $default_title),
      '#states' => array(
        'visible' => array(
          ":input[name='opac_users_${id}_page_allowed']" => array('checked' => TRUE),
        ),
      ),
    );
  }

  // Library registration page.
  $form['pages_fieldset']['opac_users_library_registration_allowed'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('opac_users_library_registration_allowed', 1),
    '#title' => t("Enable <em>Library registration page</em> page", array(), array('context' => 'libraries')),
    '#description' => t('Enable users to register to libraries.', array(), array('context' => 'libraries')),
  );
  $form['pages_fieldset']['opac_users_library_registration_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Library registration path', array(), array('context' => 'libraries')),
    '#default_value' => variable_get('opac_users_library_registration_path', 'opac-user/register'),
    '#states' => array(
      'visible' => array(
        ':input[name="opac_users_library_registration_allowed"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['pages_fieldset']['opac_users_library_registration_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Library registration page title', array(), array('context' => 'libraries')),
    '#default_value' => variable_get('opac_users_library_registration_title', t('Library registration', array(), array('context' => 'libraries'))),
    '#states' => array(
      'visible' => array(
        ':input[name="opac_users_library_registration_allowed"]' => array('checked' => TRUE),
      ),
    ),
  );
  return system_settings_form($form);
}

/**
 * AJAX callback called when selecting an authentication method.
 */
function opac_users_settings_form_auth_method_callback($form, $form_state) {
  return $form['auth_fieldset']['fields'];
}

/**
 * Form submission handler for opac users settings form.
 */
function opac_users_settings_form_submit($form, $form_state) {
  drupal_set_message($form_state['values']['opac_users_auth_method']);
}

/**
 * Form constructor for the user users mapping overview form.
 *
 * @param string $serv_id
 *   ILS server identifier mappinf is for.
 *
 * @see opac_users_mapping_overview_form_submit()
 * @ingroup forms
 */
function opac_users_mapping_overview_form($form, $form_state, $serv_id) {
  module_load_include('inc', 'opac_users', "opac_users.db");

  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $serv_id,
  );

  // Get mapped and unmapped fields for the current server.
  $server = opac_server_load($serv_id);
  $connector = opac_get_instance($server);
  $fields = $connector->patronFields();
  $mapping = opac_users_get_fields_mapping($serv_id);
  $form['#tree'] = TRUE;
  foreach ($fields as $field_name => $field) {
    $form[$field_name]['patron_field'] = array(
      '#markup' => check_plain($field['label']) . " ($field_name)",
    );
    $label_display = isset($mapping[$field_name]) ? $mapping[$field_name]['label_display'] : 'above';
    $form[$field_name]['label_display'] = array(
      '#type' => 'select',
      '#title' => t('Label display'),
      '#title_display' => 'invisible',
      '#options' => array(
        'above' => t('Above'),
        'inline' => t('Inline'),
        'hidden' => t('<Hidden>'),
      ),
      '#default_value' => $label_display,
    );
    $form[$field_name]['edit'] = array(
      '#type' => 'link',
      '#title' => t('Configure'),
      '#href' => "admin/config/opac/opac_users/fields/edit/$serv_id/" . $field_name,
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form submission handler for opac_users_mapping_overview_form().
 */
function opac_users_mapping_overview_form_submit($form, $form_state) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];
  foreach ($values as $key => $value) {
    if (is_array($value) && array_key_exists('label_display', $value)) {
      $mapping = array(
        'serv_id' => $serv_id,
        'patron_field' => $key,
        'label_display' => $value['label_display'],
      );
      opac_users_field_mapping_mod($mapping);
    }
  }
  drupal_set_message(t('Configuration saved.'));
}

/**
 * Form constructor for the editing/adding field form.
 *
 * @param string $serv_id
 *   ILS server identifier mappinf is for.
 *
 * @param string $patron_field
 *   The ILS patron field to map.
 *
 * @see opac_users_admin_field_edit_form_validate()
 * @see opac_users_admin_field_edit_form_submit()
 * @ingroup forms
 */
function opac_users_admin_field_edit_form($form, $form_state, $serv_id, $patron_field) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  module_load_include('inc', 'opac', "includes/opac.db");

  $mapping = opac_users_get_field_mapping($serv_id, $patron_field);

  // Build the form.
  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $serv_id,
  );
  $form['patron_field'] = array(
    '#type' => 'hidden',
    '#value' => $patron_field,
  );
  $form['label_display'] = array(
    '#type' => 'select',
    '#title' => t('Label display'),
    '#options' => array(
      'above' => t('Above'),
      'inline' => t('Inline'),
      'hidden' => t('<Hidden>'),
    ),
    '#default_value' => isset($mapping['label_display']) ? $mapping['label_display'] : 'above',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form submission handler for opac_users_admin_field_edit_form().
 *
 * @see opac_users_admin_field_edit_form_validate()
 */
function opac_users_admin_field_edit_form_submit($form, $form_state) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  $values = $form_state['values'];

  opac_users_field_mapping_mod($values);

  drupal_set_message(t('Configuration saved.'));
  drupal_goto('admin/config/opac/servers/manage/' . $values['serv_id'] . '/patron-mapping');
}

/**
 * Form for patron fields.
 */
function opac_users_fields_form($form, &$form_state, $server_id) {
  $explanation = 'In this page you can configure which fields are visible and '
  . 'in which order they will appear.<br />'
  . 'Check/Uncheck the box under "Visible" column to show/hide corresponding '
  . 'field.<br /> Drag and drop table rows to re-order them.';
  $form['explanation'] = array(
    '#markup' => t($explanation),
  );

  $server = opac_server_load($server_id);
  $connector = opac_get_instance($server);
  $opac_users_fields = variable_get('opac_users_fields', array());
  $values = array();
  if (array_key_exists($server_id, $opac_users_fields)) {
    $values = $opac_users_fields[$server_id];
  }

  // Fields info by region. Used to build the form.
  $fields_info = array(
    'checkouts' => array(
      'title' => t('Checkouts fields'),
      'fields' => $connector->patronCheckoutsFields(),
    ),
    'holds' => array(
      'title' => t('Holds fields'),
      'fields' => $connector->patronHoldsFields(),
    ),
  );
  if ($connector->has('issues_history')) {
    $fields_info['issues_history'] = array(
      'title' => t('Issues history fields'),
      'fields' => $connector->patronIssuesHistoryFields(),
    );
  }

  // Build the form elements.
  foreach ($fields_info as $region => $info) {
    $form[$region] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => $info['title'],
      '#collapsible' => TRUE,
      '#theme' => 'opac_users_fields_tabledrag',
      '#header' => array(t('Name'), t('Visible'), t('Weight')),
      '#region' => $region,
    );
    foreach ($info['fields'] as $name => $field) {
      $default = isset($values[$region][$name]) ? $values[$region][$name] : NULL;
      $default_weight = isset($default) ? $default['weight'] : 0;
      $default_visible = isset($default) ? $default['visible'] : 1;
      $form[$region][$name] = array(
        'name' => array(
          '#markup' => check_plain($field['label']),
        ),
        'visible' => array(
          '#type' => 'checkbox',
          '#default_value' => $default_visible,
        ),
        'weight' => array(
          '#type' => 'weight',
          '#title' => t('Weight for @label', array('@label' => $field['label'])),
          '#title_display' => 'invisible',
          '#default_value' => $default_weight,
        ),
      );
    }
  }

  $form['server_id'] = array(
    '#type' => 'hidden',
    '#value' => $server_id,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit callback for opac_users_fields_form.
 */
function opac_users_fields_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $opac_users_fields = variable_get('opac_users_fields', array());
  $server_id = $values['server_id'];
  // Reset value for this server.
  $opac_users_fields[$server_id] = array();
  // Take all form elements that are arrays.
  // This way we avoid saving keys like 'form_id', 'submit', etc.
  foreach ($values as $key => $value) {
    if (is_array($value)) {
      $opac_users_fields[$server_id][$key] = $value;
    }
  }
  variable_set('opac_users_fields', $opac_users_fields);

  // Empty user pages cache.
  ctools_include('object-cache');
  ctools_object_cache_clear_all('opac_users_page', 'checkouts');
  ctools_object_cache_clear_all('opac_users_page', 'holds');
}
