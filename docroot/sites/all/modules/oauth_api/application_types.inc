<?php

/**
 * @file
 * Application editing UI.
 */

/**
* Displays the oauth types admin overview page.
*/
function oauth_overview_types() {
  $types = oauth_type_get_types();
  $names = oauth_type_get_names();
  $field_ui = module_exists('field_ui');
  $header = array(t('Name'), t('Version'), array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2'));
  $rows = array();

  foreach ($names as $key => $name) {
    $type = $types[$key];
    if (oauth_hook($type->type, 'form')) {
      $type_url_str = str_replace('_', '-', $type->type);
      $row = array(theme('oauth_admin_overview', array('name' => $name, 'type' => $type)));
      // Set the version column.
      // TODO: Image: OAuth 1, OAuth 2.
      $row[] = array('data' => $type->version);

      // Set the edit column.
      $row[] = array('data' => l(t('edit'), 'admin/structure/oauth/manage/' . $type_url_str));

      if ($field_ui) {
        // Manage fields.
        $row[] = array('data' => l(t('manage fields'), 'admin/structure/oauth/manage/' . $type_url_str . '/fields'));

        // Display fields.
        $row[] = array('data' => l(t('manage display'), 'admin/structure/oauth/manage/' . $type_url_str . '/display'));
      }

      // Set the delete column.
      if ($type->custom) {
        $row[] = array('data' => l(t('delete'), 'admin/structure/oauth/manage/' . $type_url_str . '/delete'));
      }
      else {
        $row[] = array('data' => '');
      }

      $rows[] = $row;
    }
  }

  $build['oauth_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No application types defined. <a href="@link">Add application type</a>.', array('@link' => url('admin/structure/oauth/add'))),
  );

  return $build;
}

/**
 * Generates the application type editing form.
 */
function oauth_type_form($form , &$form_state, $type = NULL) {
  if (!isset($type->type)) {
    // This is a new type. OAuth module managed types are custom and unlocked.
    $type = oauth_type_set_defaults(array('custom' => 1, 'locked' => 0, 'version' => '1.0'));
  }

  // Make the type object available to implementations of hook_form_alter.
  $form['#oauth_type'] = $type;

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $type->name,
    '#description' => t('The human-readable name of this application type. This text will be displayed as part of the list on the <em>Add new application</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => $type->type,
    '#maxlength' => 32,
    '#disabled' => $type->locked,
    '#machine_name' => array(
      'exists' => 'oauth_type_load',
    ),
    '#description' => t('A unique machine-readable name for this application type. It must only contain lowercase letters, numbers, and underscores. This name will be used for constructing the URL of the %oauth-add page, in which underscores will be converted into hyphens.', array(
      '%oauth-add' => t('Add new application'),
    )),
  );

  // TODO: hardcoded !
  foreach (array('1.0', '2.0') as $version) {
    $options[] = oauth_version_title($version);
  }

  $form['version'] = array(
    '#title' => t('Version of the protocol'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $type->version,
    '#description' => t('Select the version of the protocol used by this application. This will define the basic structure of the application such as API keys, client ID, and secret.'),
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $type->description,
    '#description' => t('Describe this application type. The text will be displayed on the <em>Add new application</em> page.'),
  );

  $form['submission'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission form settings'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['submission']['help']  = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#default_value' => $type->help,
    '#description' => t('This text will be displayed at the top of the page when creating or editing application of this type.'),
  );

  $form['old_type'] = array(
    '#type' => 'value',
    '#value' => $type->type,
  );
  $form['orig_type'] = array(
    '#type' => 'value',
    '#value' => isset($type->orig_type) ? $type->orig_type : '',
  );
  $form['base'] = array(
    '#type' => 'value',
    '#value' => $type->base,
  );
  $form['custom'] = array(
    '#type' => 'value',
    '#value' => $type->custom,
  );
  $form['locked'] = array(
    '#type' => 'value',
    '#value' => $type->locked,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save application type'),
    '#weight' => 40,
  );

  if ($type->custom) {
    if (!empty($type->type)) {
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete application type'),
        '#weight' => 45,
      );
    }
  }

  return $form;
}

/**
* Form validation handler for oauth_type_form().
*
* @see oauth_type_form_submit()
*/
function oauth_type_form_validate($form, &$form_state) {
  $type = new stdClass();
  $type->type = trim($form_state['values']['type']);
  $type->name = trim($form_state['values']['name']);

  // Work out what the type was before the user submitted this form
  $old_type = trim($form_state['values']['old_type']);

  $types = oauth_type_get_names();

  if (!$form_state['values']['locked']) {
    // 'theme' conflicts with theme_oauth_form().
    // '0' is invalid, since elsewhere we check it using empty().
    if (in_array($type->type, array('0', 'theme'))) {
      form_set_error('type', t("Invalid machine-readable name. Enter a name other than %invalid.", array('%invalid' => $type->type)));
    }
  }

  $names = array_flip($types);

  if (isset($names[$type->name]) && $names[$type->name] != $old_type) {
    form_set_error('name', t('The human-readable name %name is already taken.', array('%name' => $type->name)));
  }
}

/**
* Form submission handler for oauth_type_form().
*
* @see oauth_type_form_validate()
*/
function oauth_type_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $type = oauth_type_set_defaults();

  $type->type = trim($form_state['values']['type']);
  $type->name = trim($form_state['values']['name']);
  $type->orig_type = trim($form_state['values']['orig_type']);
  $type->old_type = isset($form_state['values']['old_type']) ? $form_state['values']['old_type'] : $type->type;

  $type->description = $form_state['values']['description'];
  $type->help = $form_state['values']['help'];

  $type->base = !empty($form_state['values']['base']) ? $form_state['values']['base'] : 'oauth_application';
  $type->custom = $form_state['values']['custom'];
  $type->locked = $form_state['values']['locked'];
  if (isset($form['#oauth_type']->module)) {
    $type->module = $form['#oauth_type']->module;
  }
  // TODO: hardcoded !
  $type->version = $form_state['values']['version'] ? '2.0' : '1.0';

  if ($op == t('Delete application type')) {
    $form_state['redirect'] = 'admin/structure/oauth/manage/' . str_replace('_', '-', $type->old_type) . '/delete';
    return;
  }

  $variables = $form_state['values'];

  // Remove everything that's been saved already - whatever's left is assumed
  // to be a persistent variable.
  foreach ($variables as $key => $value) {
    if (isset($type->$key)) {
      unset($variables[$key]);
    }
  }

  unset($variables['form_token'], $variables['op'], $variables['submit'], $variables['delete'], $variables['reset'], $variables['form_id'], $variables['form_build_id']);

  // Save or reset persistent variable values.
  foreach ($variables as $key => $value) {
    $variable_new = $key . '_' . $type->type;
    $variable_old = $key . '_' . $type->old_type;

    if (is_array($value)) {
      $value = array_keys(array_filter($value));
    }
    variable_set($variable_new, $value);

    if ($variable_new != $variable_old) {
      variable_del($variable_old);
    }
  }

  // Saving the content type after saving the variables allows modules to act
  // on those variables via hook_oauth_type_insert().
  $status = oauth_type_save($type);

  oauth_types_rebuild();
  menu_rebuild();
  $t_args = array('%name' => $type->name);

  if ($status == SAVED_UPDATED) {
    drupal_set_message(t('The application type %name has been updated.', $t_args));
  }
  elseif ($status == SAVED_NEW) {
    drupal_set_message(t('The application type %name has been added.', $t_args));
    watchdog('oauth', 'Added application type %name.', $t_args, WATCHDOG_NOTICE, l(t('view'), 'admin/structure/oauth'));
  }

  $form_state['redirect'] = 'admin/structure/oauth';
  return;
}

function oauth_add_application_validate($form, &$form_state) {
  entity_form_field_validate('oauth', $form, $form_state);
}

function oauth_add_application_submit($form, &$form_state) {
  $oauth = $form_state['values']['oauth'];
  $oauth->name = $form_state['values']['name'];
  drupal_write_record('oauth', $oauth);
  entity_form_submit_build_entity('oauth', $oauth, $form, $form_state);
  field_attach_submit('oauth', $oauth, $form, $form_state);
  field_attach_insert('oauth', $oauth);
  drupal_set_message(t('Application @name for protocol OAuth @version successfully added.',
    array('@name' => $oauth->name, '@version' => oauth_version_title($form_state['values']['version'])))
  );
}

/**
* Returns HTML for a node type description for the oauth type admin overview page.
*
* @param $variables
*   An associative array containing:
*   - name: The human-readable name of the application type.
*   - type: An object containing the 'type' (machine name) and 'description' of
*     the content type.
*
* @ingroup themeable
*/
function theme_oauth_admin_overview($variables) {
  $name = $variables['name'];
  $type = $variables['type'];

  $output = check_plain($name);
  $output .= ' <small>' . t('(Machine name: @type)', array('@type' => $type->type)) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($type->description) . '</div>';
  return $output;
}

/**
 * Menu callback; delete a single application type.
 */
function oauth_type_delete_confirm($form, &$form_state, $type) {
  $form['type'] = array('#type' => 'value', '#value' => $type->type);
  $form['name'] = array('#type' => 'value', '#value' => $type->name);

  $message = t('Are you sure you want to delete the application type %type?', array('%type' => $type->name));
  $caption = '';

  $num_applications = db_query("SELECT COUNT(*) FROM {oauth} WHERE type = :type", array(':type' => $type->type))->fetchField();
  if ($num_applications) {
    $caption .= '<p>' . format_plural($num_applications, '%type is used by 1 application on your site. If you remove this application type, you will not be able to edit the %type application and it may not display correctly.', '%type is used by @count applications on your site. If you remove %type, you will not be able to edit the %type application and it may not display correctly.', array('%type' => $type->name)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/oauth', $caption, t('Delete'));
}

/**
* Process application type delete confirm submissions.
*/
function oauth_type_delete_confirm_submit($form, &$form_state) {
  oauth_type_delete($form_state['values']['type']);

  variable_del('oauth_preview_' . $form_state['values']['type']);
  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The application type %name has been deleted.', $t_args));
  watchdog('menu', 'Deleted application type %name.', $t_args, WATCHDOG_NOTICE);

  oauth_types_rebuild();
  menu_rebuild();

  $form_state['redirect'] = 'admin/structure/oauth';
  return;
}