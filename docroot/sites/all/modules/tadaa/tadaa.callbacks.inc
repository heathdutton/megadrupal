<?php
/**
 * @file
 * This file contains the different callbacks used by Tadaa!.
 */

/**
 * Callback that checks if the specified machinreadable name already exists.
 *
 * Used by the environment form.
 *
 * @return boolean
 *   TRUE or FALSE depending on whether the name already exists or not.
 */
function tadaa_environment_name_exists($environment_name) {
  $existing = variable_get('tadaa_environments', array());

  return isset($existing[$environment_name]);
}

/**
 * Callback that loads the available variables from the database.
 *
 * This is used for the autocomplete textfield when configuring a new variable.
 *
 * @param string $search_string
 *   The search string for the variable name.
 */
function tadaa_variable_autocomplete($search_string = '') {
  $matches = array();
  if ($search_string) {
    $result = db_select('variable')->fields('variable', array('name'))->condition('name', '%' . db_like($search_string) . '%', 'LIKE')->range(0, 10)->execute();
    foreach ($result as $variable) {
      $matches[$variable->name] = check_plain($variable->name);
    }
  }

  drupal_json_output($matches);
}

/**
 * Callback that loads the current value for a variable.
 *
 * Used by the variable form.
 */
function tadaa_variable_load_value($form, &$form_state) {
  $value = variable_get($form_state['values']['variable_name'], NULL);
  if (!isset($value)) {
    // No value saved, bail out early.
    return;
  }

  // Load the triggering element, and the name of the environment.
  $button = $form_state['triggering_element'];
  $environment = $button['#name'];

  // Set the value of the textarea, and wrap it.
  $form['environment']['variable_value']['#value'] = serialize($value);
  $form['environment']['variable_value']['#prefix'] = '<div id="' . $environment . '-value-wrapper">';
  $form['environment']['variable_value']['#suffix'] = '</div>';

  // Return the new fieldset.
  return $form['environment']['variable_value'];
}

/**
 * Callback that switches environment.
 *
 * @param string $environment
 *   The environment that we should switch to.
 */
function tadaa_environment_switch($environment) {
  // Get the enivorment that the user has switched from. The enivorment that is
  // to be swtiched to is the first argument in this function.
  $old = variable_get('tadaa_environment', '');
  $new = $environment;

  // Make it possible for hooks to act on swtich between environments before
  // the alterations by tadaa is made.
  module_invoke_all('tadaa_pre_switch', $old, $environment);

  // Get the configured module and variables.
  $modules = tadaa_get_modules($environment);
  $variables = tadaa_get_variables($environment);

  // Build an array of the modules that we should enable and one for those that
  // should be disabled.
  $enable = array();
  $disable = array();
  foreach ($modules as $module => $status) {
    if ($status) {
      $enable[] = $module;
    }
    else {
      $disable[] = $module;
    }
  }

  // Enable and disable the appropriate modules.
  module_enable($enable);
  module_disable($disable);

  // Change the values of the variables.
  foreach ($variables as $name => $value) {
    variable_set($name, $value);
  }

  // Finally, set the new environment, flush the cache, and exit.
  variable_set('tadaa_environment', $environment);
  drupal_flush_all_caches();

  // Make it possible for hooks to act on swtich between environments after
  // the alterations by tadaa is made.
  module_invoke_all('tadaa_post_switch', $old, $new);

  exit();
}
