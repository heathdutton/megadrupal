<?php
/**
 * @file
 * This file contains the configuration pages used by Tadaa!
 */

/**
 * Page callback for the environment overview.
 *
 * This will display a table with the environments, and links to configure them.
 */
function tadaa_environment_configuration_page() {
  // Load the environments.
  $environments = variable_get('tadaa_environments', array());

  if (count($environments) < 1) {
    // No environments has been added yet.
    return array(
      '#markup' => t('No environments has been added.'),
    );
  }

  // The header for the table.
  $header = array(
    t('Environment'),
    array(
      'data' => t('Operations'),
      'colspan' => 2,
    ),
  );

  // Build one row per environment.
  $rows = array();
  $environments = variable_get('tadaa_environments', array());
  foreach ($environments as $name => $title) {
    // Build the row.
    $rows[] = array(
      $title,
      l(t('edit'), 'admin/config/development/tadaa/configure/environments/' . $name . '/edit'),
      l(t('delete'), 'admin/config/development/tadaa/configure/environments/' . $name . '/delete'),
    );
  }

  // Return a renderable table array.
  $table = array(
    '#type' => 'table',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $table;
}

/**
 * Configuration form for an environment.
 */
function tadaa_environment_form($form, &$form_state, $environment_name = NULL) {
  module_load_include('inc', 'tadaa', 'tadaa.callbacks');

  if (isset($environment_name)) {
    // We're editing an environment, load the title.
    $environments = variable_get('tadaa_environments', array());
    if (!isset($environments[$environment_name])) {
      // Failed to load the specified environment. Display an error message to
      // the user, and return a page not found.
      // TODO: Is this the proper way to return a page not found?
      drupal_set_message(t('%name is not a valid environment.', array('%name' => $environment_name)), 'error');
      drupal_not_found();
      die();
    }
    $environment_title = $environments[$environment_name];
  }

  // Set the page title.
  if (isset($environment_title)) {
    drupal_set_title(t('Edit @title', array('@title' => $environment_title)));
  }
  else {
    drupal_set_title(t('Add environment'));
  }

  // Add a value element that indicates if we're editing or not.
  $form['editing'] = array(
    '#type' => 'value',
    '#value' => isset($environment_name),
  );

  // Add a textfield for the title.
  $form['environment_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment title'),
    '#required' => TRUE,
    '#default_value' => isset($environment_title) ? $environment_title : '',
    '#description' => t('Enter a name that describes the environment, e.g. Development or Production.'),
  );

  // The machinreadable name.
  $form['environment_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#machine_name' => array(
      'exists' => 'tadaa_environment_name_exists',
      'source' => array('environment_title'),
    ),
    '#default_value' => $environment_name,
    '#disabled' => isset($environment_name),
  );

  // Action buttons.
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (isset($environment_name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('tadaa_environment_form_delete'),
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * Form validation for the environment form.
 *
 * Checks if there already exists an environment with the specified title. We
 * don't want multiple environments with the same title.
 */
function tadaa_environment_form_validate($form, &$form_state) {
  if (is_array($form_state['clicked_button']['#limit_validation_errors']) && !in_array('environment_title', $form_state['clicked_button']['#limit_validation_errors'])) {
    return;
  }
  $title = $form_state['values']['environment_title'];
  $existing = variable_get('tadaa_environments', array());

  // Remove this environment from the environments we're comparing against.
  unset($existing[$form_state['values']['environment_name']]);

  if (in_array($title, $existing)) {
    // An environment with that title already exists.
    form_set_error('environment_title', t('An environment named %title already exists.', array('%title' => $title)));
  }
}

/**
 * Form submission for the environment form.
 *
 * Adds the newly created environment to the other environments using a
 * variable.
 */
function tadaa_environment_form_submit($form, &$form_state) {
  // Get the submitted values.
  $title = $form_state['values']['environment_title'];
  $machine_name = $form_state['values']['environment_name'];

  $environments = $environments = variable_get('tadaa_environments', array());
  if ($form_state['values']['editing']) {
    // Update this environment.
    $environments[$machine_name] = $title;
  }
  else {
    // Add this to the existing environments.
    $environments += array($machine_name => $title);
  }

  // Save the environments.
  variable_set('tadaa_environments', $environments);

  // Send a message to the user, and redirect to the environments overview.
  if ($form_state['values']['editing']) {
    drupal_set_message(t('%title has been updated.', array('%title' => $title)));
  }
  else {
    drupal_set_message(t('%title has been created.', array('%title' => $title)));
  }
  $form_state['redirect'] = 'admin/config/development/tadaa/configure/environments';
}

/**
 * Delete submission for an environment.
 *
 * This will redirect the user to the delete page.
 */
function tadaa_environment_form_delete($form, &$form_state) {
  // Get the environment name.
  $environment_name = $form['environment_name']['#value'];

  // Redirect to the delete page.
  $form_state['redirect'] = 'admin/config/development/tadaa/configure/environments/' . $environment_name . '/delete';
}

/**
 * Delete environment confirmation form.
 */
function tadda_environment_delete_confirm($form, &$form_state, $environment_name) {
  // Load the title.
  $environments = variable_get('tadaa_environments', array());
  if (!isset($environments[$environment_name])) {
    // Failed to load the specified environment. Display an error message to
    // the user, and return a page not found.
    // TODO: Is this the proper way to return a page not found?
    drupal_set_message(t('%name is not a valid environment.', array('%name' => $environment_name)), 'error');
    drupal_not_found();
    die();
  }
  $environment_title = $environments[$environment_name];

  // Add this environment name to the form.
  $form['environment_name'] = array(
    '#type' => 'value',
    '#value' => $environment_name,
  );
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $environment_title)),
    'admin/config/development/tadaa/configure/environments',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submission for the delete form.
 *
 * Deletes an environment and any reference to it.
 */
function tadda_environment_delete_confirm_submit($form, &$form_state) {
  // Load the title of this environment, and delete it from the existing
  // environments.
  $environments = variable_get('tadaa_environments', array());
  $environment_title = $environments[$form_state['values']['environment_name']];
  unset($environments[$form_state['values']['environment_name']]);

  // Save the environments.
  variable_set('tadaa_environments', $environments);

  // Delete the module and variable configurations.
  variable_del('tadaa_' . $form_state['values']['environment_name'] . '_modules');
  variable_del('tadaa_' . $form_state['values']['environment_name'] . '_variables');

  // Send a message to the user, and redirect to the environments overview.
  drupal_set_message(t('%title has been deleted.', array('%title' => $environment_title)));
  $form_state['redirect'] = 'admin/config/development/tadaa/configure/environments';
}

/**
 * Page callback for the module configuration.
 *
 * This will display a table with every module available, and the ability to add
 * each one to several environments.
 */
function tadaa_module_configuration_form($form, $form_state) {
  // Load the environments.
  $environments = variable_get('tadaa_environments', array());

  if (count($environments) < 1) {
    // No environments has been added yet.
    $form['#prefix'] = t('No environments has been added. Go to !page to add an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/environments/add')));
    return $form;
  }

  // Load the environments.
  $environments = variable_get('tadaa_environments', array());

  // The header for the table.
  $header = array(
    t('Module'),
  );
  // Add one column per environment.
  foreach ($environments as $name => $title) {
    $header[] = $title;
  }

  // Load every available module.
  $modules = array();
  $result = db_query("SELECT name FROM system WHERE type = 'module' AND name <> 'tadaa' ORDER BY name ASC");
  foreach ($result as $row) {
    // Load info about the module.
    $module_info = drupal_parse_info_file(drupal_get_path('module', $row->name) . '/' . $row->name . '.info');
    // Add this module to the modules array if it's visible.
    if (!isset($module_info['hidden']) || !$module_info['hidden']) {
      $package = isset($module_info['package']) ? $module_info['package'] : 'Other';
      $modules[$package][$row->name] = isset($module_info['name']) ? $module_info['name'] : $row->name;
    }
  }

  $form['#prefix'] = t("Select for which environments a module should be enabled. If no environments are selected for a specific module, Tadaa! will ignore that module.");

  $form['modules'] = array('#tree' => TRUE);

  // Build one row per module.
  $rows = array();
  foreach ($modules as $package => $modules) {
    $form['modules'][$package] = array(
      '#type' => 'fieldset',
      '#title' => t($package),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#theme' => 'tadaa_module_fieldset',
      '#header' => $header,
      // Ensure that the "Core" package fieldset comes first.
      '#weight' => $package == 'Core' ? -10 : NULL,
    );

    foreach ($modules as $module => $title) {
      $form['modules'][$package][$module] = array(
        'title' => array('#markup' => $title),
      );
      // Add one checkbox for each environment.
      foreach ($environments as $environment => $title) {
        // Get the current configuration for this environment, and determnine
        // wheter or not this module is activated.
        $active_modules = variable_get('tadaa_' . $environment . '_modules');
        $active = isset($active_modules[$module]) && $active_modules[$module];
        $form['modules'][$package][$module][$environment] = array(
          '#type' => 'checkbox',
          '#default_value' => $active,
        );

        // If this module is activated for this environment, expand the
        // fieldset.
        if ($active) {
          $form['modules'][$package]['#collapsed'] = FALSE;
        }
      }
    }

  }

  // Lastly, sort all fieldsets by title.
  uasort($form['modules'], 'element_sort_by_title');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit callback for the module configuration form.
 */
function tadaa_module_configuration_form_submit($form, $form_state) {
  foreach ($form_state['values']['modules'] as $package => $modules) {
    foreach ($modules as $module => $configuration) {
      if (in_array(TRUE, $configuration)) {
        foreach ($configuration as $environment => $status) {
          $environments[$environment][$module] = $status;
        }
      }
    }
  }

  foreach ($environments as $environment => $configuration) {
    variable_set('tadaa_' . $environment . '_modules', $configuration);
  }
}

/**
 * Page callback for the variable overview.
 *
 * This will display one table for each environment, and the configured
 * variables.
 */
function tadaa_variable_configuration_page() {
  // Load the environments.
  $environments = variable_get('tadaa_environments', array());

  if (count($environments) < 1) {
    // No environments has been added yet.
    return array(
      '#markup' => t('No environments has been added. Go to !page to add an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/environments/add'))),
    );
  }

  $output = '';

  // The header for the tables.
  $header = array(
    t('Variable'),
    t('Value'),
    array(
      'data' => t('Operations'),
      'colspan' => 2,
    ),
  );

  // Load the environments, and create a table for each environment.
  $environments = variable_get('tadaa_environments', array());
  foreach ($environments as $name => $title) {
    // Get the variables for this environment.
    $variables = variable_get('tadaa_' . $name . '_variables', array());
    ksort($variables);
    // Add one variable per row.
    $rows = array();
    foreach ($variables as $variable => $value) {
      $rows[] = array(
        $variable,
        wordwrap(serialize($value), 75, '<br />', TRUE),
        l(t('edit'), 'admin/config/development/tadaa/configure/variables/' . $name . '/edit/' . $variable),
        l(t('delete'), 'admin/config/development/tadaa/configure/variables/' . $name . '/delete/' . $variable),
      );
    }

    // Add this table to the output.
    $table = array(
      'caption' => $title,
      'header' => $header,
      'rows' => $rows,
      'empty' => t('No variables have been configured for this environment.'),
    );
    $output .= theme('table', $table);
    $output .= '<p>' . l(t('Add variable'), 'admin/config/development/tadaa/configure/variables/add/' . $name) . '</p>';
  }

  return $output;
}

/**
 * Configuration form for a variable.
 */
function tadaa_variable_form($form, &$form_state, $environment_name, $variable_name = NULL) {
  module_load_include('inc', 'tadaa', 'tadaa.callbacks');
  $environments = variable_get('tadaa_environments', array());
  $environment_title = $environments[$environment_name];

  // Set the page title.
  if (isset($variable_name)) {
    drupal_set_title(t('Edit @variable', array('@variable' => $variable_name)));
  }
  else {
    drupal_set_title(t('Add variable'));
  }

  // Add the current environment to the form values.
  $form['environment_name'] = array(
    '#type' => 'value',
    '#value' => $environment_name,
  );

  // Add a textfield for the variables name.
  $form['variable_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable'),
    '#required' => TRUE,
    '#default_value' => isset($variable_name) ? $variable_name : '',
    '#autocomplete_path' => 'tadaa/variable/autocomplete',
    '#description' => !isset($variable_name) ? t("Enter the name of the variable you wish to configure. If it can't be found automatically, you may enter any value, and it will be created.") : '',
    '#disabled' => isset($variable_name),
  );

  // Add the value configuration fieldset.
  $form['environment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
  );

  // Get the configuration for this environment if we're editing a variable.
  if (isset($variable_name)) {
    $configuration = variable_get('tadaa_' . $environment_name . '_variables', array());
    $default_value = isset($configuration[$variable_name]) ? serialize($configuration[$variable_name]) : '';
  }

  $form['environment']['variable_value'] = array(
    '#type' => 'textarea',
    '#title' => t('Value'),
    '#prefix' => '<div id="' . $environment_name . '-value-wrapper">',
    '#suffix' => '</div>',
    '#default_value' => isset($default_value) ? $default_value : '',
    '#description' => t("Enter the desired value for the %environment environment. If you're configuring a variable that already exists, you may load the current value automatically.", array('%environment' => $environment_title)),
    '#required' => TRUE,
  );
  $form['environment']['load'] = array(
    '#type' => 'button',
    '#value' => t('Load current value'),
    '#name' => $environment_name,
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'tadaa_variable_load_value',
      'wrapper' => $environment_name . '-value-wrapper',
    ),
  );

  // Action buttons.
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (isset($variable_name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('tadaa_variable_form_delete'),
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * Form submission for the variable configuration.
 */
function tadaa_variable_form_submit($form, &$form_state) {
  // Get the form values.
  $environment_name = $form_state['values']['environment_name'];
  $variable_name = $form_state['values']['variable_name'];
  $variable_value = $form_state['values']['variable_value'];

  // Try to unserialize the provided value, because variable_set() will
  // automatically serialize it.
  $variable_value = @unserialize($variable_value) !== FALSE ? unserialize($variable_value) : $variable_value;

  // Load the current configuration.
  $current_configuration = variable_get('tadaa_' . $environment_name . '_variables', array());

  // Merge the provided value with the current configuration.
  $new_configuration = array_merge($current_configuration, array($variable_name => $variable_value));

  // Save the new configuration.
  variable_set('tadaa_' . $environment_name . '_variables', $new_configuration);

  // Send a message to the user, and redirect to the variables overview.
  drupal_set_message(t('%variable has been configured for the %environment environment.', array('%variable' => $variable_name, '%environment' => $environment_name)));
  $form_state['redirect'] = 'admin/config/development/tadaa/configure/variables';
}

/**
 * Delete submission for a variable.
 *
 * This will redirect the user to the delete page.
 */
function tadaa_variable_form_delete($form, &$form_state) {
  // Get the variable and environment names.
  $environment_name = $form['environment_name']['#value'];
  $variable_name = $form['variable_name']['#value'];

  // Redirect to the delete page.
  $form_state['redirect'] = 'admin/config/development/tadaa/configure/variables/' . $environment_name . '/delete/' . $variable_name;
}

/**
 * Delete variable confirmation form.
 */
function tadda_variable_delete_confirm($form, &$form_state, $environment_name, $variable_name) {
  // Add the environment and variable to the form values.
  $form['environment_name'] = array(
    '#type' => 'value',
    '#value' => $environment_name,
  );
  $form['variable_name'] = array(
    '#type' => 'value',
    '#value' => $variable_name,
  );

  // Load the title.
  $environments = variable_get('tadaa_environments', array());
  if (!isset($environments[$environment_name])) {
    // Failed to load the specified environment. Display an error message to
    // the user, and return a page not found.
    // TODO: Is this the proper way to return a page not found?
    drupal_set_message(t('%name is not a valid environment.', array('%name' => $environment_name)), 'error');
    drupal_not_found();
    die();
  }
  $environment_title = $environments[$environment_name];

  return confirm_form($form,
    t('Are you sure you want to delete the configuration for %variable?', array('%variable' => $variable_name)),
    'admin/config/development/tadaa/configure/variables',
    isset($environment_name) ? t('This will delete the configuration for the %title environment.', array('%title' => $environment_title)) : t('This will delete the configuration for every environment.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submission for the delete form.
 *
 * Deletes a configuration for a variable. If a specific environment has been
 * chosen, only the configuration for that environment is deleted.
 */
function tadda_variable_delete_confirm_submit($form, &$form_state) {
  // Get the variable and environment names.
  $variable_name = $form_state['values']['variable_name'];
  $environment_name = $form_state['values']['environment_name'];
  $environments = variable_get('tadaa_environments', array());
  $environment_title = $environments[$environment_name];

  // Load the current configuration, and remove this variable before saving the
  // new value.
  $configuration = variable_get('tadaa_' . $environment_name . '_variables', array());
  unset($configuration[$variable_name]);
  variable_set('tadaa_' . $environment_name . '_variables', $configuration);

  // Send a message to the user, and redirect to the variables overview.
  drupal_set_message(t('%variable has been deleted from the %environment environment.', array('%variable' => $variable_name, '%environment' => $environment_title)));
  $form_state['redirect'] = 'admin/config/development/tadaa/configure/variables';
}
