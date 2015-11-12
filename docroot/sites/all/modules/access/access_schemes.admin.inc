<?php
/**
 * @file
 * Access schemes administrative UI for the access control kit module.
 */

/**
 * Menu page callback; the access schemes overview page.
 */
function access_overview_schemes() {
  $schemes = access_scheme_get_schemes();
  $field_ui = module_exists('field_ui');
  $header = array(t('Name'), array(
    'data' => t('Operations'),
    'colspan' => $field_ui ? '5' : '3',
  ));
  $rows = array();

  foreach ($schemes as $key => $scheme) {
    $url = 'admin/structure/access/manage/' . str_replace('_', '-', $key);
    $row = array(theme('access_schemes_overview', array('scheme' => $scheme)));

    // The edit operation.
    $row[] = array('data' => l(t('edit'), $url));
    // The manage access handlers operation.
    $row[] = array('data' => l(t('manage access handlers'), $url . '/handlers'));

    // Field UI operations.
    if ($field_ui) {
      // Manage fields.
      $row[] = array('data' => l(t('manage fields'), $url . '/fields'));
      // Manage display.
      $row[] = array('data' => l(t('manage display'), $url . '/display'));
    }

    // The delete operation.
    $row[] = array('data' => l(t('delete'), $url . '/delete'));

    $rows[] = $row;
  }

  $build['scheme_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No access schemes available. <a href="@link">Add an access scheme</a>.', array('@link' => url('admin/structure/access/add'))),
  );

  return $build;
}

/**
 * Returns HTML for the Name column on the access scheme admin overview page.
 *
 * @param $variables
 *   An associative array containing:
 *   - scheme: The access scheme object.
 */
function theme_access_schemes_overview($variables) {
  $scheme = $variables['scheme'];

  $output = check_plain($scheme->name);
  $output .= ' <small>' . t('(Machine name: @type)', array('@type' => $scheme->scheme)) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($scheme->description) . '</div>';
  return $output;
}

/**
 * Form constructor for the access scheme add/edit form.
 *
 * @param $scheme
 *   (optional) The access scheme to edit.
 *
 * @see access_scheme_form_validate()
 * @see access_scheme_form_submit()
 */
function access_scheme_form($form, &$form_state, $scheme = NULL) {
  if (!isset($scheme->scheme)) {
    $scheme = access_scheme_set_defaults();
  }
  $form['#scheme'] = $scheme;
  $form['old_scheme'] = array('#type' => 'value', '#value' => $scheme->scheme);

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $scheme->name,
    '#description' => t('The human-readable name of this access scheme. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 28,
  );

  $form['scheme'] = array(
    '#type' => 'machine_name',
    '#default_value' => $scheme->scheme,
    '#maxlength' => 28,
    '#machine_name' => array(
      'exists' => 'access_scheme_load',
    ),
    '#description' => t('A unique machine-readable name for this access scheme. It must only contain lowercase letters, numbers, and underscores.'),
    '#disabled' => !empty($scheme->scheme),
  );

  // The realm type can only be set when creating a new scheme.
  if (empty($scheme->realm_type)) {
    $form['realm_type'] = array(
      '#title' => t('Realm type'),
      '#type' => 'select',
      '#options' => access_realm_options(),
      '#description' => t('The base object for dividing the site into access realms. For example, if the realm type is a list field, you will be able to grant users access to content based upon the values of that field.'),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'access_scheme_realm_settings_js',
        'wrapper' => 'realm-settings-wrapper',
        'effect' => 'fade',
      ),
    );

    $form['realm_settings'] = array(
      '#prefix' => '<div id="realm-settings-wrapper">',
      '#suffix' => '</div>',
      '#type' => 'container',
    );

    // Dynamically add the settings, if any, for the selected realm type.
    if (!empty($form_state['values']['realm_type']) && $form_state['values']['realm_type']) {
      $realm_type = $form_state['values']['realm_type'];
      $info = access_realm_info($realm_type);
      $form['realm_settings']['arguments'] = module_invoke($info['module'], 'access_realm_settings', $realm_type, FALSE);
      $form['realm_settings']['arguments']['#tree'] = TRUE;
    }
  }
  else {
    // For editing existing schemes, the realm type is unchangeable.
    $form['realm_type'] = array('#type' => 'value', '#value' => $scheme->realm_type);

    // Display a summary of the realm type.
    $options = access_realm_options();
    $form['realm_info'] = array(
      '#type' => 'item',
      '#title' => t('Realm type'),
      '#markup' => check_plain($options[$scheme->realm_type]),
    );
    // Show the current realm field settings, if any, as a list.
    $field = field_info_field('ack_' . $scheme->scheme);
    if (!empty($field['settings']['arguments'])) {
      $form['realm_info']['#description'] = '<ul>';
      foreach ($field['settings']['arguments'] as $key => $value) {
        $form['realm_info']['#description'] .= '<li>[' . check_plain($key) . '] ' . check_plain($value) . '</li>';
      }
      $form['realm_info']['#description'] .= '</ul>';
    }
  }

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $scheme->description,
    '#description' => t('A brief description of this access scheme.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($scheme->scheme) ? t('Save access scheme and add handlers') : t('Save access scheme'),
    '#weight' => 40,
  );
  if (!empty($scheme->scheme)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete access scheme'),
      '#weight' => 45,
    );
  }

  return $form;
}

/**
 * Form validation handler for access_scheme_form().
 *
 * @see access_scheme_form_submit()
 */
function access_scheme_form_validate($form, &$form_state) {
  $scheme = new stdClass();
  $scheme->scheme = trim($form_state['values']['scheme']);
  $scheme->name = trim($form_state['values']['name']);

  // Get the original machine name, before the form was submitted.
  $old_scheme = trim($form_state['values']['old_scheme']);

  // 'theme' conflicts with theme_access_scheme_form().
  // '0' is invalid because it wouldn't pass an empty() check.
  if (in_array($scheme->scheme, array('0', 'theme'))) {
    form_set_error('scheme', t('Invalid machine-readable name. Enter a name other than %invalid.', array('%invalid' => $scheme->scheme)));
  }

  // Check for duplicate human-readable names.
  $names = array_flip(access_scheme_get_names());
  if (isset($names[$scheme->name]) && $names[$scheme->name] != $old_scheme) {
    form_set_error('name', t('The human-readable name %name is already taken.', array('%name' => $scheme->name)));
  }
}

/**
 * Form submission handler for access_scheme_form().
 *
 * @see access_scheme_form_validate()
 */
function access_scheme_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $scheme = access_scheme_set_defaults();
  $scheme->scheme = trim($form_state['values']['scheme']);
  // @todo Eliminate the "old_scheme" convention entirely? We already don't allow changing the machine name through the UI, and we don't have a good way of renaming the access field to match if the machine name did change.
  $scheme->old_scheme = isset($form_state['values']['old_scheme']) ? $form_state['values']['old_scheme'] : $scheme->scheme;
  $scheme->name = trim($form_state['values']['name']);
  $scheme->realm_type = $form_state['values']['realm_type'];
  $scheme->description = $form_state['values']['description'];
  if (isset($form_state['values']['arguments'])) {
    $scheme->realm_settings = $form_state['values']['arguments'];
  }

  if ($op == t('Delete access scheme')) {
    $form_state['redirect'] = 'admin/structure/access/manage/' . str_replace('_', '-', $scheme->old_scheme) . '/delete';
    return;
  }

  $status = access_scheme_save($scheme);
  menu_rebuild();

  $t_args = array('%name' => $scheme->name);
  if ($status == SAVED_UPDATED) {
    drupal_set_message(t('The access scheme %name has been updated.', $t_args));
    $form_state['redirect'] = 'admin/structure/access';
  }
  elseif ($status == SAVED_NEW) {
    drupal_set_message(t('The access scheme %name has been added.', $t_args));
    watchdog('access', 'Added access scheme %name.', $t_args, WATCHDOG_NOTICE, l(t('view'), 'admin/structure/access'));
    $form_state['redirect'] = 'admin/structure/access/manage/' . str_replace('_', '-', $scheme->scheme) . '/handlers';
  }
}

/**
 * Form constructor for the access scheme delete confirmation form.
 *
 * @see access_scheme_delete_confirm_submit()
 */
function access_scheme_delete_confirm($form, &$form_state, $scheme) {
  $form['scheme'] = array('#type' => 'value', '#value' => $scheme->scheme);
  $form['name'] = array('#type' => 'value', '#value' => $scheme->name);

  $message = t('Are you sure you want to delete the access scheme %scheme?', array('%scheme' => $scheme->name));
  $caption = '';

  $num_grants = db_query('SELECT COUNT(*) FROM {access_grant} WHERE scheme = :scheme', array(':scheme' => $scheme->scheme))->fetchField();
  if ($num_grants) {
    $caption .= '<p>' . format_plural($num_grants, '%scheme is used by 1 access grant on your site. If you remove this scheme, you will not be able to edit the %scheme grant and users may not be able to exercise permissions based on roles assigned by that grant.', '%scheme is used by @count access grants on your site. If you remove this scheme, you will not be able to edit the %scheme grants and users may not be able to exercise permissions based on roles assigned by those grants.', array('%scheme' => $scheme->name)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/access', $caption, t('Delete'));
}

/**
 * Form submission handler for access_scheme_delete_confirm().
 */
function access_scheme_delete_confirm_submit($form, &$form_state) {
  access_scheme_delete($form_state['values']['scheme']);
  menu_rebuild();

  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The access scheme %name has been deleted.', $t_args));
  watchdog('access', 'Deleted access scheme %name.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/structure/access';
  return;
}

/**
 * Form constructor for managing a scheme's attached access handlers.
 */
function access_scheme_handlers_form($form, &$form_state, $scheme) {
  $form['#scheme'] = $scheme;

  // Get information on all available access handlers.
  $info = module_invoke_all('access_info');
  if (empty($info)) {
    return array('empty' => array('#markup' => t('No access handlers available. In order to control access to content, menu items, or other Drupal objects, you must first <a href="@link">enable one or more modules</a> that provide Access Control Kit handlers, such as the ACK node module.', array('@link' => url('admin/modules')))));
  }

  // Get the scheme's realm settings, for use below by handlers.
  $field = field_info_field('ack_' . $scheme->scheme);
  $realm_settings = isset($field['settings']['arguments']) ? $field['settings']['arguments'] : array();

  // Display each supported object type as a vertical tab.
  $form['objects'] = array('#type' => 'vertical_tabs');
  foreach ($info as $object => $object_data) {
    $form['objects'][$object] = array(
      '#type' => 'fieldset',
      '#title' => $object_data['label'],
      '#tree' => TRUE,
    );

    // Construct an ID for the AJAX settings form (below) in a manner similar to
    // drupal_html_id(), but without the uniqueness logic, so that it doesn't
    // increment on a rebuild.
    $id = 'handler-settings-wrapper-' . $object;
    $id = strtr(drupal_strtolower($id), array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
    $id = preg_replace('/[^A-Za-z0-9\-_]/', '', $id);
    $id = preg_replace('/\-+/', '-', $id);

    // Handler selector.
    $form['objects'][$object]['handler'] = array(
      '#type' => 'select',
      '#title' => t('@object access realms are determined by', array('@object' => $object_data['label'])),
      '#options' => array(),
      '#default_value' => isset($scheme->handlers[$object]) ? get_class($scheme->handlers[$object]) : '',
      '#empty_option' => t('- Not managed -'),
      '#ajax' => array(
        'callback' => 'access_handler_settings_js',
        'wrapper' => $id,
        'effect' => 'fade',
      ),
    );
    // Only show handlers that match the scheme's realm type.
    foreach ($object_data['handlers'] as $handler => $handler_data) {
      if (in_array($scheme->realm_type, $handler_data['realm types'])) {
        $form['objects'][$object]['handler']['#options'][$handler] = $handler_data['label'];
      }
    }

    $form['objects'][$object]['settings'] = array(
      '#prefix' => '<div id="' . $id . '">',
      '#suffix' => '</div>',
      '#type' => 'container',
    );

    // Dynamic settings form for the handler.
    $selected_handler = NULL;
    if (!empty($form_state['values'][$object]['handler'])) {
      // The user has selected a new handler and we're rebuilding via AJAX.
      // Check whether the new selection is the currently saved handler.
      if (isset($scheme->handlers[$object]) && $form_state['values'][$object]['handler'] == get_class($scheme->handlers[$object])) {
        // Use the already configured object.
        $selected_handler = $scheme->handlers[$object];
      }
      else {
        // Use an unconfigured instance of the selected handler class.
        $class = $form_state['values'][$object]['handler'];
        $selected_handler = new $class();
      }
    }
    elseif (isset($scheme->handlers[$object])) {
      // On initial build, show the currently active handler's settings.
      $selected_handler = $scheme->handlers[$object];
    }
    // If we were able to determine a handler above, add its settings form.
    if (isset($selected_handler)) {
      $form['objects'][$object]['settings']['arguments'] = $selected_handler->settingsForm($scheme->realm_type, $realm_settings);
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form submission handler for access_scheme_handlers_form().
 */
function access_scheme_handlers_form_submit($form, &$form_state) {
  $scheme = $form['#scheme'];
  $modules = module_implements('access_info');
  foreach ($modules as $module) {
    $info = module_invoke($module, 'access_info');
    foreach ($info as $object => $object_data) {
      if (!empty($form_state['values'][$object]['handler'])) {
        $settings = empty($form_state['values'][$object]['settings']['arguments']) ? array() : $form_state['values'][$object]['settings']['arguments'];
        access_handler_save($scheme->scheme, $object, $form_state['values'][$object]['handler'], $module, $settings);
      }
    }
  }
}
