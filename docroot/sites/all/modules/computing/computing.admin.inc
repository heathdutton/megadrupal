<?php


/**
 * Generate the overview page.
 */
function computing_overview_form($form, &$form_state) {
  $apps = computing_get_applications();

  $app_stats = array();
  $records_stats = db_query("SELECT application, status, COUNT(*) AS num FROM {computing_record} GROUP BY application, status")->fetchAll();
  foreach ($records_stats as $rs) {
    $app_stats[$rs->application][$rs->status] = $rs->num;
  }

  $table_rows = array();
  foreach ($apps as $app) {
    $table_rows[] = array(
      "{$app->label} ({$app->application})",
      $app->description,
      @$app_stats[$app->application]['RDY'] ?: 0,
      @$app_stats[$app->application]['SCF'] ?: 0,
      @$app_stats[$app->application]['FLD'] ?: 0,
      @array_sum(@$app_stats[$app->application]) ?: 0
    );
  }

  $form['table'] = array(
    '#markup' => theme('table', array(
      'header' => array(t('Application (machine name)'), t('Description'), t('Unhandled'), t('Accomplished'), t('Failed'), t('Total')),
      'rows' => $table_rows,
      'caption' => t('Application Summary')
    )),
  );

//  $form['actions'] = array(
//    'purge' => array(
//      '#type' => 'submit',
//      '#value' => t('Purge old records'),
//    ),
//    'abort' => array(
//      '#type' => 'submit',
//      '#value' => t('Abort long running records'),
//    )
//  );

  return $form;
}


function computing_overview_form_submit($form, &$form_state) {
  // do nothing for now.
}


/**
 * Generate the "Command" page. Code following the logic of system_admin_config_page().
 */
function computing_list_command_page() {
  $apps = computing_get_applications();
  $computing_data = computing_fetch_data();
  $blocks = array();

  foreach ($computing_data as $app_name => $app_command_data) {

    // make sure hook_computing_data returns valid computing applications, which are stored as "computing application' entities
    if (!isset($apps[$app_name])) {
      continue;
    }

    // block data: title, content, description, position, show (see theme_admin_page(). theme_admin_block().
    $app_entity = $apps[$app_name];
    $block = array(
      'title' => $app_entity->label,
      // description is shown only when block content is empty.
      'description' => $app_entity->description,
      'show' => TRUE,
      // "position" specifies whether to show to left or right.
    );

    // build application command data.
    // block content data: title, href, localized_options, description.
    $commands = array();
    foreach ($app_command_data as $command_name => $command_data) {
      $commands[] = array(
        'title' => empty($command_data['title']) ? strtoupper($command_name) : $command_data['title'],
        'description' => empty($command_data['description'])? t('Add a command: %command', array('%command' => $command_name)) : $command_data['description'],
        'href' => COMPUTING_MODULE_ADMIN_PATH ."/add/{$app_name}/{$command_name}",
        'localized_options' => array(),
      );
    }

    // if $commands is empty, it will show $block['description'] according to theme_admin_block().
    if ($commands) {
      $block['content'] = theme('admin_block_content', array('content' => $commands));
    }

    $blocks[$app_entity->weight .' '. $app_entity->application] = $block;

  } // end of iterating $computing_data.


  if ($blocks) {
    ksort($blocks);
    return theme('admin_page', array('blocks' => $blocks));
  }
  else {
    return t('You do not have any computing applications');
  }
}

/**
 * Generate a form for a command. We expect this function will create a computing record.
 *
 * @param $app_name
 * @param $command_name
 * @return array|mixed|void
 */
function computing_add_command($app_name, $command_name) {
  $command_data = computing_fetch_data($app_name, $command_name);
  if (!$command_data) {
    return drupal_not_found();
  }

  if (!isset($command_data['application'])) {
    $command_data['application'] = $app_name;
  }
  if (!isset($command_data['command'])) {
    $command_data['command'] = $command_name;
  }

  // load file if necessary.
  if (isset($command_data['file']) && is_array($command_data['file'])) {
    call_user_func_array('module_load_include', $command_data['file']);
  }

  $form_callback = isset($command_data['form callback']) && function_exists($command_data['form callback']) ? $command_data['form callback'] : 'computing_command_form';
  return drupal_get_form($form_callback, $command_data);
}


/**
 * This is similar to system_settings_form().
 * Make all data in 'input', and add "Add" button and other generic fields.
 */
function computing_command_form($form, &$form_state, $command_data) {
  // 1. process $form data. make them into $form['input']
  if (isset($command_data['form elements callback']) && function_exists($command_data['form elements callback'])) {
    $form['input'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure input parameters for %command', array('%command' => $command_data['title'])),
      //'#weight' => 5,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      //'#description' => $command_data['description'],
    );
    $form['input'] = $form['input'] + call_user_func($command_data['form elements callback']);
  }
  else {
    $form['no_input'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure parameters for %command', array('%command' => $command_data['title'])),
      //'#weight' => 5,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('This command has no input parameter.'),
    );
  }

  // 2. add other command data.
  $form['common'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure generic parameters'),
    //'#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['common']['application'] = array(
    '#type' => 'textfield',
    '#title' => 'Application',
    '#title_display' => 'before',
    '#default_value' => $command_data['application'],
    '#disabled' => TRUE,
  );

  $form['common']['command'] = array(
    '#type' => 'textfield',
    '#title' => 'Command',
    '#title_display' => 'before',
    '#default_value' => $command_data['command'],
    '#disabled' => TRUE,
  );

  $form['common']['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Label',
    '#description' => 'The human-readable name of this command.',
    '#default_value' => $command_data['title'],
    '#required' => TRUE,
  );

  $form['common']['weight'] = array(
    '#type' => 'textfield',
    '#title' => 'Weight',
    '#description' => 'Command execution weight. Low number has higher priority.',
    '#default_value' => 0,
    '#element_validation' => 'element_validate_integer',
    '#required' => TRUE,
  );

  $form['common']['status'] = array(
    '#type' => 'select',
    '#title' => 'Status',
    '#options' => computing_status_readable(),
    //'#description' => 'Command execution weight. Low number has higher priority.',
    '#default_value' => 'RDY',
    '#disabled' => TRUE,
  );

  // 3. add code from system_settings_form().
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add Command'));
  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The command cannot be added because of the errors.'), 'error');
  }
  $form['#submit'][] = 'computing_command_form_submit';

  return $form;
}


function computing_command_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  //form_state_values_clean($form_state);

  $input_data = NULL;
  if (isset($form_state['values']['input'])) {
    $input_data = $form_state['values']['input'];
  }

  $common_data = $form_state['values']['common'];
  $result = computing_create($common_data['application'], $common_data['command'], $common_data['label'], $input_data, $common_data);

  if ($result) {
    drupal_set_message(t('Adding new command successfully.'));
  }
  else {
    drupal_set_message(t('Cannot add new command. Please inform site administrators.'), 'error');
  }

  $form_state['redirect'] = COMPUTING_MODULE_ADMIN_PATH;
}


/**
 * 'form elements callback' for the Echo command.
 */
function computing_echo_optional_form_elements() {
  $elements = array();
  $elements['ping'] = array(
    '#type' => 'textfield',
    '#title' => 'Ping Message',
    '#description' => 'Type in the message to ping remote agent.',
    '#default_value' => 'Hello, world!',
    '#required' => TRUE,
  );
  return $elements;
}


/**
 * 'result callback' for the Echo command.
 */
function computing_echo_handle_result($output, $record) {
  $input = $record->input;
  if (isset($output['pong']) && isset($input['ping']) && $output['pong'] == $input['ping']) {
    watchdog('computing', 'Echo command executed correctly from agent.');
  }
  else {
    watchdog('computing', 'Echo command not executed correctly. See computing record: !id', array('!id' => $record->id), WATCHDOG_WARNING);
  }
}