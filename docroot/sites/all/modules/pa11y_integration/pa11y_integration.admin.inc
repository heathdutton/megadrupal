<?php
/**
 * @file
 * Administrative functions.
 *
 * Contains administrative functions.
 */

/**
 * Form constructor for the admin settings form.
 *
 * @see pa11y_integration_menu()
 */
function pa11y_integration_admin_settings_form(array $form, array $form_state) {

  $form['pa11y_integration_settings']['#tree'] = TRUE;

  $form['pa11y_integration_url'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('PA11Y webservices url'),
    '#field_prefix' => variable_get('pa11y_integration_url', 'http://localhost:3000'),
    '#description' => t('PA11Y webservices url'),
    '#default_value' => variable_get('pa11y_integration_url', 'http://localhost:3000'),
  );

  $form['pa11y_integration_test_standard'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('PA11Y test standard'),
    '#description' => t('PA11Y test standard to use for testing'),
    '#options' => array(
      'Section508' => t('Section508'), 'WCAG2A' => t('WCAG2A'), 'WCAG2AA' => t('WCAG2AA'), 'WCAG2AAA' => t('WCAG2AAA')),
    '#default_value' => variable_get('pa11y_integration_test_standard', 'WCAG2AA'),
  );

  $form['pa11y_integration_run_cron_limit'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('PA11Y run cron limit'),
    '#description' => t('Limit on how many tasks to run for each cron run.'),
    '#options' => range(0, 25, PA11Y_INTEGRATION_LIMIT_INCREMENT),
    '#default_value' => variable_get('pa11y_integration_run_cron_limit', 10),
  );

  $form['pa11y_integration_test_run'] = array(
    '#type' => 'checkbox',
    '#required' => FALSE,
    '#title' => t('PA11Y test run'),
    '#description' => t('Experimental feature to run the tasks at the same time as submitting the tasks'),
    '#default_value' => 0,
  );

  $form['pa11y_integration_test'] = array(
    '#title' => t('Start AX testing'),
    '#type' => 'submit',
    '#description' => t('<p>To test every page on this site for accessibility, please press the Start button below</p>'),
    '#prefix' => '<div>',
    '#value' => t('Start PA11Y test'),
    '#submit' => array('pa11y_integration_admin_settings_form_test_submit'),

  );

  $form['pa11y_integration_delete'] = array(
    '#title' => t('Delete all tests'),
    '#type' => 'submit',
    '#value' => t('Delete all PA11Y tests'),
    '#description' => t('<p>To test every page on this site for accessibility, please press the Start button below</p>'),
    '#submit' => array('pa11y_integration_admin_settings_form_delete_submit'),
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Form validation handler for pa11y_integration_admin_settings_form().
 *
 * @see pa11y_integration_admin_settings_form()
 */
function pa11y_integration_admin_settings_form_validate(array $form, array $form_state) {

  $parsed_url = parse_url($form_state['values']['pa11y_integration_url']);
  if (!($parsed_url['scheme'] == 'http' || $parsed_url['scheme'] == 'https' && isset($parsed_url['host']))) {
    form_set_error('pa11y_integration_url', t('PA11Y Integration url set is not a valid url'));
  }
}
/**
 * Form submit function for test form.
 *
 * @see pa11y_integration_menu()
 */
function pa11y_integration_admin_settings_form_test_submit(array $form, array &$form_state) {
  $destination = array();
  // Check url is valid.
  if ($test_request = drupal_http_request(variable_get('pa11y_integration_url', '') . '/tasks', array('method' => 'GET'))) {

    if ($test_request->code != 200) {
      form_set_error('pa11y_integration_url', t('PA11Y Webservice is not responding, perhaps url is not set or invalid?'));
      drupal_goto('admin/config/content/pa11y_integration');
    }
  }
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('admin/config/content/pa11y_integration/test', array('query' => $destination));
}
/**
 * Form submit function for delete form..
 *
 * @see pa11y_integration_menu()
 */
function pa11y_integration_admin_settings_form_delete_submit(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('admin/config/content/pa11y_integration/delete', array('query' => $destination));

}

/**
 * Form builder for the start test form.
 */
function pa11y_integration_admin_settings_form_test_confirm(array $form, array &$form_state) {
  return confirm_form($form,
    t('Are you sure you want to start testing your site with PA11Y?'),
    'admin/config/content/pa11y_integration/test/start',
    t('All publicly visible content will be validated using PA11Y.'),
    t('Test all content')
  );
}

/**
 * Form builder for the delete test form.
 */
function pa11y_integration_admin_settings_form_delete_confirm(array $form, array &$form_state) {
  return confirm_form($form,
    t('Are you sure you want delete to all tasks registered with PA11Y?'),
    'admin/config/content/pa11y_integration/delete/start',
    t('All publicly visible content tasks will be deleted from PA11Y.'),
    t('Delete all content tasks from PA11Y')
  );
}
/**
 * Form submit function for test confirm form..
 *
 * @see pa11y_integration_menu()
 */
function pa11y_integration_admin_settings_form_test_confirm_submit(array $form, array &$form_state) {
  pa11y_integration_batch_test();
}
/**
 * Form submit function for delete confirm form..
 *
 * @see pa11y_integration_menu()
 */
function pa11y_integration_admin_settings_form_delete_confirm_submit(array $form, array &$form_state) {
  pa11y_integration_batch_delete();
}

/**
 * Submit a batch job to test content.
 */
function pa11y_integration_batch_test() {

  $result = db_select('node', 'n')->fields('n', array('nid', 'title'))->condition('status', 1)->countQuery()->execute();
  $total_count = $result->fetchAssoc();

  $batch = array(
    'operations' => array(
      array(
        'pa11y_integration_batch_test_tasks',
        array($total_count['expression']),
      ),
    ),
    'finished' => 'pa11y_integration_batch_finished',
    'title' => t('Testing'),
    'init_message' => t('Preparing to submit content to PA11Y for testing...'),
    'progress_message' => t('Submitting content to PA11Y...'),
    'error_message' => t('PA11Y test submission has encountered an error.'),
    'file' => drupal_get_path('module', 'pa11y_integration') . '/pa11y_integration.admin.inc',
  );
  batch_set($batch);
}


/**
 * Submit a batch job to delete content.
 */
function pa11y_integration_batch_delete() {
  $batch = array(
    'operations' => array(
      array(
        'pa11y_integration_batch_delete_tasks',
        array(),
      ),
    ),
    'finished' => 'pa11y_integration_batch_finished',
    'title' => t('Deleting'),
    'init_message' => t('Preparing to submit content to PA11Y for removal...'),
    'progress_message' => t('Submitting content to PA11Y...'),
    'error_message' => t('PA11Y delete submission has encountered an error.'),
    'file' => drupal_get_path('module', 'pa11y_integration') . '/pa11y_integration.admin.inc',
  );
  batch_set($batch);
}

/**
 * Batch Operation Callback.
 *
 * @return false
 *   return false when an exception was caught
 *
 * @throws Exception
 *   When PA11Y gives an error, throw an exception that PA11Y is not available
 */
function pa11y_integration_batch_test_tasks($total_limit, &$context) {

  if (empty($context['sandbox'])) {

    try {
      // Get content.
      $context['sandbox']['content'] = pa11y_integration_get_content();

    }
    catch (Exception $e) {
      watchdog('PA11Y', $e->getMessage(), NULL, WATCHDOG_ERROR);

    }

    $context['sandbox']['submitted'] = 0;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['batch_progress'] = 0;
    $context['sandbox']['total'] = $total_limit;
    $context['sandbox']['batch_total'] = count($context['sandbox']['content']);
  }

  try{
    if (pa11y_integration_submit_task($context['sandbox']['content'][$context['sandbox']['batch_progress']])) {
      $context['sandbox']['submitted']++;
    }
  }
  catch (\Exception $e) {
    watchdog('PA11Y', $e->getMessage(), NULL, WATCHDOG_ERROR);
  }
  $context['sandbox']['progress']++;
  $context['sandbox']['batch_progress']++;

  $arguments = array(
    '@current' => $context['sandbox']['progress'],
    '@total' => $total_limit,
    '@submitted' => $context['sandbox']['submitted'],
  );

  $context['message'] = t('Inspected @current of @total entities. Submitted @submitted documents to PA11Y', $arguments);

  // Inform the batch engine that we are not finished, and provide an
  // estimation of the completion level we reached.
  $context['finished'] = empty($context['sandbox']['batch_total']) ? 1 : $context['sandbox']['progress'] / $context['sandbox']['total'];

  // Reset content if we've completed a batch.
  if ($context['sandbox']['batch_total'] == $context['sandbox']['batch_progress'] && $context['sandbox']['batch_total'] < $context['sandbox']['total']) {
    $context['sandbox']['content'] = pa11y_integration_get_content($context['sandbox']['progress']);
    $context['sandbox']['batch_progress'] = 0;
  }
  // Put the total into the results section when we're finished so we can
  // show it to the admin.
  if ($context['finished']) {
    $context['results']['count'] = $context['sandbox']['progress'];
    $context['results']['submitted'] = $context['sandbox']['submitted'];
    $context['results']['op'] = 'testing';
  }

}

/**
 * Batch Operation Callback.
 *
 * @return false
 *   return false when an exception was caught
 *
 * @throws Exception
 *   When PA11Y gives an error, throw an exception that PA11Y is not available
 */
function pa11y_integration_batch_delete_tasks(&$context) {

  if (empty($context['sandbox'])) {

    try {
      // Get content,
      require_once drupal_get_path('module', 'pa11y_integration') . '/lib/PA11Y/Client/PA11YDrupalClient.class.inc';
      $pa11y = new lib\PA11Y\Client\PA11YDrupalClient(variable_get('pa11y_integration_url', 'http://localhost:3000'));
      $context['sandbox']['content'] = array_values($pa11y->getTasks());
    }
    catch (Exception $e) {
      watchdog('PA11Y', $e->getMessage(), NULL, WATCHDOG_ERROR);
      return FALSE;
    }

    $context['sandbox']['submitted'] = 0;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($context['sandbox']['content']);
  }

  try{
    pa11y_integration_delete_task($context['sandbox']['content'][$context['sandbox']['progress']]);
  }
  catch (\Exception $e) {
    watchdog('PA11Y', $e->getMessage(), NULL, WATCHDOG_ERROR);
    return FALSE;
  }

  $context['sandbox']['progress']++;
  $context['sandbox']['submitted']++;

  $arguments = array(
    '@current' => $context['sandbox']['progress'],
    '@total' => $context['sandbox']['total'],
    '@submitted' => $context['sandbox']['submitted'],
  );

  $context['message'] = t('Removed @current of @total entities. Submitted @submitted documents to PA11Y for deletion', $arguments);

  // Inform the batch engine that we are not finished, and provide an
  // estimation of the completion level we reached.
  $context['finished'] = empty($context['sandbox']['total']) ? 1 : $context['sandbox']['progress'] / $context['sandbox']['total'];

  // Put the total into the results section when we're finished so we can
  // show it to the admin.
  if ($context['finished']) {
    $context['results']['count'] = $context['sandbox']['progress'];
    $context['results']['submitted'] = $context['sandbox']['submitted'];
    $context['results']['op'] = 'deleting';
  }

}

/**
 * Batch 'finished' callback.
 */
function pa11y_integration_batch_finished($success, array $results, array $operations) {

  $message = '';

  if (isset($results['count'])) {
    $message .= format_plural($results['count'], '1 item processed successfully. ', '@count items successfully processed. ');
  }
  if (isset($results['submitted'])) {
    $message .= format_plural($results['submitted'], '1 document successfully sent to PA11Y for ' . $results['op'] . '.', '@count documents successfully sent to PA11Y for ' . $results['op'] . '. For the results, see <a href="!url">the status page</a>', array('!url' => '/admin/reports/pa11y_integration'));
  }
  if ($success) {
    $type = 'status';
  }
  else {
    // An error occurred. $operations contains the unprocessed operations.
    $error_operation = reset($operations);
    $message .= ' ' . t('An error occurred while processing @num with arguments: @args',
            array(
              '@num' => $error_operation[0],
              '@args' => print_r($error_operation[0], TRUE)));
    $type = 'error';
  }
  drupal_set_message(($message), $type);
  drupal_goto(variable_get('pa11y_integration_admin_path', 'admin/config/content/pa11y_integration'));
}
