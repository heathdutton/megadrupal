<?php

/**
 * @file
 *  Forms and functionality for the admin side of Drupal. The entity 
 *  add/edit forms and module config form are defined here.
 */

/**
 * Implements hook_form().
 * 
 * Provide a form to allow the user to setup the module.
 */
function update_trigger_build_config_form($form, &$form_state) {

  // Retrieve all CI Server Types and display them in a list.
  $entity_helper = new CIEntityHelper();
  $ci_server_links = $entity_helper->getCIServerAdminLinks();

  $form['ci_server_types'] = array(
    '#type' => 'item',
    '#title' => t('1) Create a CI Server'),
    '#description' => t('This is where you specify the connection details for your Continuous Integration Server.'),
    '#theme' => 'links',
    '#links' => $ci_server_links,
  );

  $form['build_schedule'] = array(
    '#type' => 'item',
    '#title' => t('2) Create a <a href="@url">Build Schedule</a>', 
                   array('@url' => $GLOBALS['base_url'] . '/admin/config/development/update_trigger_build/ci_build_schedule')),
    '#description' => t('A <a href="@url">Build Schedule</a> specifies how often a build should be run, which job should be run and the build triggers.', 
                        array('@url' => $GLOBALS['base_url'] . '/admin/config/development/update_trigger_build/ci_build_schedule')),
  );
  return $form;
}

/**
 * Display a single CI Build Schedule entity.
 *
 * @param $id
 *  The Id of the CI Build Schedule.
 *  
 * @param string $output
 *  The markup for display. 
 */
function update_trigger_build_view_ci_build_schedule($id) {

  // Load the CI Build Schedule entity.
  $build_schedules = entity_load('ci_build_schedule', array($id));
  $build_schedule = $build_schedules[$id];

  // Set the page title.
  drupal_set_title($build_schedule->name);

  // Build and return the markup for display.
  $output = entity_view('ci_build_schedule', array($build_schedule));
  return $output;
}

/**
 * Display a single CI Server entity.
 *
 * @param $id
 *  The Id of the CI Server.
 *
 * @param string $output
 *  The markup for display. 
 */
function update_trigger_build_view_ci_server($id) {

  // Load the CI Server entity.
  $servers = entity_load('ci_server', array($id));
  $server = $servers[$id];

  // Set the page title.
  drupal_set_title($server->name);

  // Build and return the markup for display.
  $output = entity_view('ci_server', array($server));
  return $output;
}

/**
 * Implements hook_form().
 * 
 * Provide a form to allow the user to add / edit a CI Build Schedule entity.
 */
function ci_build_schedule_form($form, &$form_state, $build_schedule = NULL) {

  $ci_entity_helper = new CIEntityHelper();

  // Get a list of CI Server to display in a select.
  $ci_server_options = $ci_entity_helper->getCIServers(TRUE);

  // If there are no CI Servers then the user must create one.
  if(empty($ci_server_options)) {
    return array(
      'error' => array(
        '#markup' => t('You must create a CI Server before you can create a build schedule.'),
      ),
    );
  }

  $form['label'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($build_schedule->label) ? $build_schedule->label : '',
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($build_schedule->description) ? $build_schedule->description : '',
  );
  $form['ci_server'] = array(
    '#title' => t('CI Server'),
    '#type' => 'select',
    '#options' => $ci_server_options,
    '#default_value' => isset($build_schedule->ci_server) ? $build_schedule->ci_server : '',
    '#required' => TRUE,
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'ci_job',
      'callback' => 'update_trigger_build_ci_build_schedule_ajax',
      'method' => 'replace',
    ),
    '#submit' => array('update_trigger_build_get_trigger_options'),
  );

  // Collect the list of jobs dependent on the value in the ci_server select element.
  if(isset($form_state['values']['ci_server'])) {

    // $form_state will be set after AJAX request or form submission.
    // Select option value is stored in format ci_Server_type:entity_id, so we split it.
    $selected_value = explode(':', $form_state['values']['ci_server']);
    $job_options = update_trigger_build_get_ci_job_options($selected_value[1], $selected_value[0]);
  }
  else {
    // If first load then use the first CI server option value to populate the list.
    $job_options = array();

    if(!empty($form['ci_server']['#options'])) {
      $first_option = key($form['ci_server']['#options']);

      // Select option value is stored in format ci_Server_type:entity_id, so we split it.
      $first_option = explode(':', $first_option);
      $job_options = update_trigger_build_get_ci_job_options($first_option[1], $first_option[0]);
    }
  }

  $form['ci_job'] = array(
    '#title' => t('CI Job'),
    '#type' => 'select',
    '#options' => $job_options,
    '#default_value' => isset($build_schedule->ci_job) ? $build_schedule->ci_job : '',
    '#prefix' => "<div id='ci_job'>",
    '#suffix' => "</div>",
    '#required' => TRUE,
  );
  $form['frequency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#description' => t('How often the module should check for updates.  ' .
      'N.b. frequency is limited by <A href="/admin/config/system/cron" target="_blank">the cron frequency</a>.  ' .
      'If cron is set to run every 3 hours then updates will only be checked every 3 hours.  ' .
      'Install a module, such as Ultimate Cron or Elysia Cron if you need more granularity over cron run frequency.'),
    '#options' => array(
      '0' => t('As often as possible'),
      '60' => t('Every 1 minute'),
      '600' => t('Every 10 minutes'),
      '1800' => t('Every 30 minutes'),
      '3600' => t('Every 1 hour'),
      '10800' => t('Every 3 hours'),
      '21600' => t('Every 6 hours'),
      '43200' => t('Every 12 hours'),
      '86400' => t('Every 24 hours'),
      '259200' => t('Every 3 days'),
      '604800' => t('Every 7 days'),
    ),
    '#default_value' => isset($build_schedule->frequency) ? $build_schedule->frequency : '3600',
  );

  // Build triggers.
  $form['build_triggers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Build Triggers'),
    '#description' => t('Choose which type of updates will trigger a build. E.g. "security" will only trigger a build when security updates are available'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['build_triggers']['trigger_core'] = array(
    '#title' => t('Core'),
    '#type' => 'select',
    '#options' => update_trigger_build_get_trigger_options(),
    '#default_value' => isset($build_schedule->trigger_core) ? $build_schedule->trigger_core : '',
  );
  $form['build_triggers']['trigger_module'] = array(
    '#title' => t('Module'),
    '#type' => 'select',
    '#options' => update_trigger_build_get_trigger_options(),
    '#default_value' => isset($build_schedule->trigger_module) ? $build_schedule->trigger_module : '',
  );
  $form['build_triggers']['trigger_theme'] = array(
    '#title' => t('Theme'),
    '#type' => 'select',
    '#options' => update_trigger_build_get_trigger_options(),
    '#default_value' => isset($build_schedule->trigger_theme) ? $build_schedule->trigger_theme : '',
  );

  // Parameters.
  $parameters_default = isset($build_schedule->parameters) ? unserialize($build_schedule->parameters) : array();
  $form['parameters'] = array(
    '#title' => t('Build Parameters'),
    '#type' => 'checkboxes',
    '#description' => t('Choose which elements should be included as build parameters.'),
    '#options' => array(
      'drupal_root' => t('Include path to <strong>Drupal root</strong> directory (parameter name is "drupal_root")'),
      'site_root' => t('Include path to <strong>site root</strong> (parameter name is "site_root")'),
      'projects' => t('Include <strong>list of projects</strong> that require an upgrade (parameter name is "projects".  Format of parameter - media:1.1,views:3.8,panels:2.6)'),
    ),
    '#default_value' => $parameters_default,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($build_schedule->id) ? t('Update CI Build Schedule') : t('Save CI Build Schedule'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Submit handler for the CI Build Schedule add/edit form.
 */
function ci_build_schedule_form_submit($form, &$form_state) {

  // This function is provided by the entity API to prepare form values
  // for saving in the database.
  $build_schedule = entity_ui_form_submit_build_entity($form, $form_state);

  // Convert the parameter checkbox values into a string for storing in the 
  // database.
  $build_schedule->parameters = serialize(array_filter($build_schedule->parameters));

  // There is no machine name on the form, so generate it from the label.
  // Only do this on create, not update.
  if(!isset($build_schedule->id)) {
    $machine_readable = strtolower($build_schedule->label);
    $build_schedule->name = preg_replace('@[^a-z0-9_]+@','_', $machine_readable);
  }

  $build_schedule->save();

  drupal_set_message(t('The build schedule: @name has been saved.', array('@name' => $build_schedule->label)));
  $form_state['redirect'] = 'admin/config/development/update_trigger_build/ci_build_schedule';
}

/**
 * Ajax callback.  When creating a CI Build Schedule, we dynamically 
 * populate the list of CI Jobs based on what was selected for CI Server.
 */
function update_trigger_build_ci_build_schedule_ajax($form, &$form_state) {
  return $form['ci_job'];
}

/**
 * Dynamically retrieve the jobs belonging to a specific CI Server in a 
 * format that's suitable for a 'select' form element.
 *
 * @param string $ci_server_id
 *  The Id of the CI Server.
 *
 * @return array $jobs
 *  The list of jobs in associative array format.
 */
function update_trigger_build_get_ci_job_options($ci_server_id, $entity_type) {

  $jobs = array();

  if(isset($ci_server_id) && !empty($ci_server_id)) {

    // Load the CI Server using its Id.
    $ci_server = entity_load($entity_type, array($ci_server_id));
    $ci_server = array_shift($ci_server);

    // Invoke the getJobs method on the CI Server class. The jobs are retrieved
    // directly from the CI Server, so this list will always be current.
    $jobs = $ci_server->getJobs(TRUE);
  }
  return $jobs;  
}

/**
 * Get a list of the possible triggers for a build in a format that's 
 * suitable for a select form item.
 *
 * Possible values are:
 *  none: do not trigger a build.
 *  security: only trigger a build when security updates are available.
 *  all: trigger a build when any kind of update is available.
 *
 * @return array $options
 *  A list of the possible values for the build triggers.
 */
function update_trigger_build_get_trigger_options() {
  return array(
  	'none' => t('None'),
    'security' => t('Security'),
    'all' => t('All'),
  );
}
