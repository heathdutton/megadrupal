<?php
/**
 * @file
 * This file contains the status pages used by Tadaa!
 */

/**
 * Page callback for the module statuses.
 *
 * This will display a table with every module and the current status.
 */
function tadaa_module_status_page() {
  // Load the environments.
  $environments = variable_get('tadaa_environments', array());
  if (count($environments) < 1) {
    // No environments has been added yet.
    return array(
      '#markup' => t('No environments has been added. Go to !page to add an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/environments/add'))),
    );
  }

  // Verify that we have an active environment.
  if (!TADAA_ENVIRONMENT) {
    return array(
      '#markup' => t('No environment has been selected. Go to !page to activate an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/status/environments'))),
    );
  }

  // Load the modules.
  $modules = tadaa_get_modules(TADAA_ENVIRONMENT);
  if (count($modules) < 1) {
    // No environments has been added yet.
    return array(
      '#markup' => t('No modules has been configured yet. Go to !page to set up some modules.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/modules'))),
    );
  }

  // The header for the table.
  $header = array(
    '',
    t('Module'),
    t('Environment configuration'),
    t('Real status'),
    t('Operations'),
  );

  // Build one row per module.
  $rows = array();
  foreach ($modules as $module => $configuration) {
    // Get the real module name.
    $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
    // Get the real status of the module.
    $real_status = module_exists($module);
    // Get the path to the icon that we should use,
    $status_icon = $configuration == $real_status ? 'misc/message-16-ok.png' : 'misc/message-16-error.png';
    // Set human readable texts for the configuration.
    $configuration_text = $configuration ? t('Enable') : t('Disable');
    $real_status_text = $real_status ? t('Enabled') : t('Disabled');
    // Build the row.
    $rows[] = array(
      theme('image', array('path' => $status_icon)),
      $module_info['name'],
      $configuration_text,
      $real_status_text,
      $configuration != $real_status ? l(t('Fix'), "admin/config/development/tadaa/status/modules/$module/fix") : '',
    );
  }

  // Load the title for the current environment that we'll use as the caption.
  $environment_title = isset($environments[TADAA_ENVIRONMENT]) ? $environments[TADAA_ENVIRONMENT] : t('none');

  // Return a renderable table array.
  $table = array(
    '#type' => 'table',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#caption' => t('Current environment: @environment', array('@environment' => $environment_title)),
  );

  return $table;
}

/**
 * Confirmation form when fixing a module.
 *
 * @param string $module
 *   The module that we'd like to fix.
 */
function tadaa_module_status_fix($form, &$form_state, $module) {
  // Load the modules for the active environment.
  $modules = tadaa_get_modules(TADAA_ENVIRONMENT);

  // Store the module and its configured status as internal form values, as
  // these will be used by the submit callback.
  $form['module'] = array(
    '#type' => 'value',
    '#value' => $module,
  );
  $form['status'] = array(
    '#type' => 'value',
    '#value' => $modules[$module],
  );

  // Get information about the module and every environment in order to get
  // their names.
  $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
  $environments = variable_get('tadaa_environments', array());

  // Set a different question and description based on what the status of the
  // module should be.
  $params = array('%module' => $module_info['name'], '%environment' => $environments[TADAA_ENVIRONMENT]);
  if ($modules[$module]) {
    $question = t('Are you sure you want to enable %module?', $params);
    $description = t('%module should be enabled for the current environment, which is %environment.', $params);
  }
  else {
    $question = t('Are you sure you want to disable %module?', $params);
    $description = t('%module should be disabled for the current environment, which is %environment.', $params);
  }

  // Return a confirmation form.
  return confirm_form($form, $question, 'admin/config/development/tadaa/status/modules', $description);
}

/**
 * Submit callback for the module fix form.
 */
function tadaa_module_status_fix_submit($form, &$form_state) {
  // Get the form values and the module info.
  $values = $form_state['values'];
  $module_info = drupal_parse_info_file(drupal_get_path('module', $values['module']) . '/' . $values['module'] . '.info');

  // Enable or disable the module, depending on the configuration for the
  // current environment.
  $params = array('%module' => $module_info['name']);
  if ($values['status']) {
    // Enable the module and set a message.
    module_enable(array($values['module']));
    drupal_set_message(t('%module has been enabled.', $params));
  }
  else {
    // Disable the module and set a message.
    module_disable(array($values['module']));
    drupal_set_message(t('%module has been disabled.', $params));
  }

  // Redirect the user back to the module status page.
  $form_state['redirect'] = 'admin/config/development/tadaa/status/modules';
}

/**
 * Page callback for the variable statuses.
 *
 * This will display a table with every variable and the current status.
 */
function tadaa_variable_status_page() {
  // Load the environments.
  $environments = variable_get('tadaa_environments', array());
  if (count($environments) < 1) {
    // No environments has been added yet.
    return array(
      '#markup' => t('No environments has been added. Go to !page to add an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/environments/add'))),
    );
  }

  // Verify that we have an active environment.
  if (!TADAA_ENVIRONMENT) {
    return array(
      '#markup' => t('No environment has been selected. Go to !page to activate an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/status/environments'))),
    );
  }

  // Load the variables.
  $variables = tadaa_get_variables(TADAA_ENVIRONMENT);
  if (count($variables) < 1) {
    // No environments has been added yet.
    return array(
      '#markup' => t('No variables has been configured yet. Go to !page to set up some values.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/variables'))),
    );
  }

  // The header for the table.
  $header = array(
    '',
    t('Variable'),
    t('Environment configuration'),
    t('Real value'),
    t('Operations'),
  );

  // Build one row per variable.
  $rows = array();
  foreach ($variables as $variable => $configuration) {
    // Get the real variable value.
    $real_value = variable_get($variable);
    // Get the path to the icon that we should use,
    $status_icon = $configuration == $real_value ? 'misc/message-16-ok.png' : 'misc/message-16-error.png';
    // Build the row.
    $rows[] = array(
      theme('image', array('path' => $status_icon)),
      $variable,
      wordwrap(serialize($configuration), 45, '<br />', TRUE),
      wordwrap(serialize($real_value), 45, '<br />', TRUE),
      $configuration != $real_value ? l(t('Fix'), "admin/config/development/tadaa/status/variables/$variable/fix") : '',
    );
  }

  // Load the title for the current environment that we'll use as the caption.
  $environment_title = isset($environments[TADAA_ENVIRONMENT]) ? $environments[TADAA_ENVIRONMENT] : t('none');

  // Return a renderable table array.
  $table = array(
    '#type' => 'table',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#caption' => t('Current environment: @environment', array('@environment' => $environment_title)),
  );

  return $table;
}

/**
 * Confirmation form when fixing a variable.
 *
 * @param string $variable
 *   The variable that we'd like to fix.
 */
function tadaa_variable_status_fix($form, &$form_state, $variable) {
  // Load the variables for the active environment.
  $variables = tadaa_get_variables(TADAA_ENVIRONMENT);

  // Store the variable and its configured value as internal form values, as
  // these will be used by the submit callback.
  $form['variable'] = array(
    '#type' => 'value',
    '#value' => $variable,
  );
  $form['value'] = array(
    '#type' => 'value',
    '#value' => $variables[$variable],
  );

  // Setup the question and description for the confirm form.
  $environments = variable_get('tadaa_environments', array());
  $params = array('%variable' => $variable, '%value' => serialize($variables[$variable]), '%environment' => $environments[TADAA_ENVIRONMENT]);
  $question = t('Are you sure you want to change the value for %variable?', $params);
  $description = t('%variable should be set to %value for the current environment, which is %environment.', $params);

  // Return a confirmation form.
  return confirm_form($form, $question, 'admin/config/development/tadaa/status/variables', $description);
}

/**
 * Submit callback for the variable fix form.
 */
function tadaa_variable_status_fix_submit($form, &$form_state) {
  // Get the form values.
  $values = $form_state['values'];

  // Set the variable to the configured value, and set a message.
  variable_set($values['variable'], $values['value']);
  drupal_set_message(t('%variable has been changed.', array('%variable' => $values['variable'])));

  // Redirect the user back to the variable status page.
  $form_state['redirect'] = 'admin/config/development/tadaa/status/variables';
}

/**
 * The form to switch environments.
 */
function tadaa_environment_status_form($form, &$form_state) {
  $environments = variable_get('tadaa_environments', array());

  if (count($environments) > 0) {
    $form['environment'] = array(
      '#type' => 'radios',
      '#title' => t('Select an environment'),
      '#options' => $environments,
      '#default_value' => TADAA_ENVIRONMENT,
      '#required' => TRUE,
      '#description' => 'To switch environment from outside the site, go to ' . url('tadaa/environment/switch/ENVIRONMENT_MACHINE_NAME', array(
        'absolute' => TRUE, 'query' => array('key' => variable_get('cron_key', 'drupal')))) . '.',
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Switch'),
    );
  }
  else {
    $form['#prefix'] = t('No environments has been added. Go to !page to add an environment.', array('!page' => l(t('this page'), 'admin/config/development/tadaa/configure/environments/add')));
  }

  return $form;
}

/**
 * Form submission callback for the form that switches environments.
 *
 * This function will gather the modules that should be disabled and enabled,
 * and the variables that should be set. This will be executed through a batch
 * process.
 *
 * @see tadaa_batch_disable_modules()
 * @see tadaa_batch_enable_modules()
 * @see tadaa_batch_set_variables()
 * @see tadaa_batch_finish()
 */
function tadaa_environment_status_form_submit($form, &$form_state) {
  // Get the enivorment that the user has switched from and what enivorment
  // that is to be set.
  $old = variable_get('tadaa_environment', '');
  $new = $form_state['values']['environment'];

  // Get the configured module and variables.
  $modules = tadaa_get_modules($form_state['values']['environment']);
  $variables = tadaa_get_variables($form_state['values']['environment']);

  // Build an array of modules to disable.
  $disable = array();
  foreach ($modules as $module => $status) {
    if (!$status) {
      $disable[] = $module;
    }
  }

  // Build an array of modules to enable.
  $enable = array();
  foreach ($modules as $module => $status) {
    if ($status) {
      $enable[] = $module;
    }
  }

  // Set up the operations.
  $operations = array(
    array('tadaa_batch_init', array('old' => $old, 'new' => $new)),
  );

  if (!empty($disable)) {
    $operations[] = array('tadaa_batch_disable_modules', array($disable));
  }
  if (!empty($enable)) {
    $operations[] = array('tadaa_batch_enable_modules', array($enable));
  }
  if (!empty($variables)) {
    $operations[] = array('tadaa_batch_set_variables', array($variables));
  }
  $operations[] = array('tadaa_batch_finish', array($form_state['values']['environment']));

  // Initiate the batch process.
  $environments = variable_get('tadaa_environments', array());
  $batch = array(
    'title' => t('Switching to %environment', array('%environment' => $environments[$form_state['values']['environment']])),
    'operations' => $operations,
    'finished' => 'tadaa_batch_finished',
  );

  // Make it possible for hooks to act on swtich between environments before
  // the alterations by tadaa is made.
  module_invoke_all('tadaa_pre_switch', $old, $new);

  batch_set($batch);
}
