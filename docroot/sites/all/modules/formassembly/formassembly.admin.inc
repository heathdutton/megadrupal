<?php

/**
 * @file
 * Contains FormAssembly admin settings.
 */

/**
 * Page callback for the FormAssembly settings form.
 */
function formassembly_settings_form() {
  $form = array();
  $key = 'formassembly_oauth_cid';
  $form[$key] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('FormAssembly OAuth Client ID'),
    '#description' => t('The client ID.'),
    '#default_value' => variable_get($key, ''),
  );

  $key = 'formassembly_oauth_secret';
  $form[$key] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('FormAssembly OAuth Client Secret'),
    '#description' => t('The client secret.'),
    '#default_value' => variable_get($key, ''),
  );

  $key = 'formassembly_endpoint';
  $form[$key] = array(
    '#type' => 'select',
    '#title' => t('API Endpoint'),
    '#required' => TRUE,
    '#description' => t('Select the endpoint for API requests.'),
    '#options' => array(
      'dev' => t('Developer Sandbox'),
      'pro' => t('FormAssembly.com'),
      'ent' => t('FormAssembly Enterprise Cloud'),
    ),
    '#default_value' => variable_get($key, 'pro'),
  );

  $key = 'formassembly_instance';
  $form[$key] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Enterprise Cloud Subdoman'),
    '#description' => t('<em>subdomain</em>.tfaforms.net'),
    '#default_value' => variable_get($key, ''),
    '#states' => array(
      'visible' => array(
        ':input[name="formassembly_endpoint"]' => array('value' => 'ent'),
      ),
    ),
  );

  $key = 'formassembly_admin_index';
  $form[$key] = array(
    '#type' => 'checkbox',
    '#required' => FALSE,
    '#title' => t('Admin Index'),
    '#description' => t('Select to return a list of all forms in the FormAssembly instance. The authentication tokens entered above must be from an admin-level user'),
    '#default_value' => variable_get($key, FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="formassembly_endpoint"]' => array('value' => 'ent'),
      ),
    ),
  );

  $form['batch_sync_formassembly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync now'),
    '#description' => t('Sync forms after submitting this form.'),
    '#default_value' => FALSE,
    '#suffix' => '<div>' . t('To sync on cron, place the drush command %command into your crontab.  See !url for more information.', array(
        '%command' => 'drush fa-sync',
        '!url' => l('https://www.drupal.org/node/1294438', 'https://www.drupal.org/node/1294438pwd')
      )) . '</div>'
  );

  $form['formassembly_reauthorize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reauthorize'),
    '#description' => t('Reauthorize this application and get a new access token.'),
    '#default_value' => FALSE,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Validation callback for the FormAssembly settings form.
 */
function formassembly_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $client_id = !empty($values['formassembly_oauth_cid']);
  $client_secret = !empty($values['formassembly_oauth_secret']);

  // Check that the user has provided oauth credentials.
  if (!$client_id) {
    $error_message = t('Client ID is required to sync.');
    form_set_error('formassembly_oauth_cid', $error_message);
    watchdog('formassembly', $error_message);
  }
  if (!$client_secret) {
    $error_message = t('Client secret is required to sync.');
    form_set_error('formassembly_oauth_secret', $error_message);
    watchdog('formassembly', $error_message);
  }
}

/**
 * Submit handler for the FormAssembly settings form.
 */
function formassembly_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  _formassembly_save_vars($values);

  $client_id = $values['formassembly_oauth_cid'];

  // Deleting this variable is enough to generate a new access_token.
  if ($values['formassembly_reauthorize']) {
    variable_del('formassembly_oauth_access_token');
  }

  $request = new FormAssemblyRequest($client_id);
  $token = $request->authorize();
  $request->setToken($token);

  if ($values['batch_sync_formassembly']) {
    //Setup a batch
    $batch = array(
      'operations' => array(
        array(
          'formassembly_batch_get_forms',
          array($values['formassembly_admin_index'])
        ),
        array('formassembly_batch_extract_forms', array()),
      ),
      'finished' => 'formassembly_batch_get_finished',
      'title' => 'Request Forms Data from FormAssembly',
      'init_message' => 'Contacting FormAssembly',
    );
    batch_set($batch);
  }
  drupal_set_message('FormAssembly configuration saved.');
}

/**
 * Save values needed later - those that begin with 'formassembly'.
 *
 * @param array $vars
 *   The contents of $form_state['values'].
 */
function _formassembly_save_vars($vars) {
  foreach ($vars as $key => $value) {
    // Only set variables for keys that start with 'formassembly'.
    if (strpos($key, 'formassembly') === 0) {
      variable_set($key, $value);
    }
  }
}

/**
 * Callback for the formassembly overview page.
 */
function formassembly_build_list() {
  $rows = array();

  // Build the sortable table header.
  $header = array(
    'title' => array(
      'data' => 'Title',
      'type' => 'property',
      'specifier' => 'name',
      'sort' => 'asc'
    ),
    'operations' => array('data' => t('Operations')),
  );


  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'fa_form')
    ->tableSort($header)
    ->pager(50);
  $result = $query->execute();
  if (!empty($result)) {
    $forms = formassembly_load_multiple(array_keys($result['fa_form']));
  }
  else {
    $forms = array();
  }

  foreach ($forms as $form) {
    $rows[$form->eid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $form->name,
          '#href' => 'formassembly/' . $form->eid,
        ),
      ),
    );

    $destination = drupal_get_destination();

    // Build a list of all the accessible operations for the current form.
    $operations = array();
    if (entity_access('edit', 'fa_form', $form)) {
      $operations['edit'] = array(
        'title' => t('Edit Parameters'),
        'href' => 'formassembly/' . $form->eid . '/edit',
        'query' => $destination,
      );
    }
    // Render an unordered list of operations links.
    $rows[$form->eid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__fa_form_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  $output = array(
    'fa_form_content' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('There are no FormAssembly forms available.'),
    ),
    'pager' => array(
      '#theme' => 'pager',
    ),
  );

  return $output;
}