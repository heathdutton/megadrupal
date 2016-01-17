<?php
/**
 * @file Provide config forms for views
 */

/**
 * User integration settings form.
 */
function party_user_settings_form($form, &$form_state) {
  $form['party_user_on_registration'] = array(
    '#type' => 'radios',
    '#title' => t('On user registration:'),
    '#options' => array(
      'nothing' => t('Do nothing'),
      'create' => t('Create a new party'),
      'acquire' => t('Acquire an existing party, creating one if necessary'),
    ),
    '#default_value' => variable_get('party_user_on_registration', 'nothing'),
  );

  $options = array();
  foreach (party_get_data_set_info() as $data_set_name => $def) {
    if ($data_set_name != 'user') {
      $options[$data_set_name] = $def['label'];
    }
  }

  if (module_exists('party_hat')) {
    $options = array();
    foreach (party_hat_get_tree() as $hat_name => $hat) {
      $options[$hat_name] = str_repeat('-', $hat->depth) . $hat->label;
    }

    $form['registration_hats'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#title' => t("What hat's should users' parties be given on registration?"),
      '#default_value' => variable_get('party_user_registration_hats', array()),
    );
  }

  // Retrieve all the data sets so we can look for appropriate fields
  $form['email_sync'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => party_find_fields_of_types('varchar'),
    '#title' => t('Fields to synchronise with user email'),
    '#description' => t("Any time the user is saved, these fields will be updated to match the user's email address."),
    '#default_value' => variable_get('party_user_email_sync_fields', array()),
    '#size' => 10,
  );

  $form['party_user_format_username'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Party label to format usernames'),
    '#default_value' => variable_get('party_user_format_username', TRUE),
  );

  $form['party_user_party_delete_action'] = array(
    '#type' => 'radios',
    '#title' => t('What to do with attached users when their Party is deleted'),
    '#default_value' => variable_get('party_user_party_delete_action', 'disallow'),
    '#options' => array(
      'disallow' => 'Disallow the deletion of Parties with attached users',
      'user_cancel_reassign' => 'Delete the account and make its content belong to the Anonymous user',
      'user_cancel_delete' => 'Delete the account and its content',
    ),
    '#description' => t('When a party is deleted what should be done with the attached user entities?'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save configuration',
    '#weight' => 40,
  );

  return $form;
}

/**
 * Submit handler for the user integration settings form.
 */
function party_user_settings_form_submit($form, &$form_state) {
  variable_set('party_user_on_registration', $form_state['values']['party_user_on_registration']);
  variable_set('party_user_format_username', $form_state['values']['party_user_format_username']);
  variable_set('party_user_party_delete_action', $form_state['values']['party_user_party_delete_action']);

  if (module_exists('party_hat')) {
    $registration_hats = $form_state['values']['registration_hats'];
    foreach ($registration_hats as $key => $var) {
      if ($var === 0) {
        unset($registration_hats[$key]);
      }
    }
    $registration_hats = array_keys($registration_hats);
    variable_set('party_user_registration_hats', $registration_hats);
  }

  drupal_set_message(t('The configuration options have been saved.'));

  // Email Sync Settings
  $email_sync_fields = $form_state['values']['email_sync'];
  $email_sync_fields = array_keys($email_sync_fields, TRUE);
  variable_set('party_user_email_sync_fields', $email_sync_fields);
}

/**
 * User Sync Form
 *
 * @todo: Optionally use the rule event to sync
 * @todo: Limit by role
 * @todo: Choose which Party Types to create
 */
function party_user_sync_form($form, &$form_state) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Synchronise Users',
  );

  return $form;
}

function party_user_sync_form_submit($form, &$form_state) {
  // Get all Users
  $query = db_select('users', 'u')
    ->fields('u', array('uid', 'name'))
    ->condition('uid', 0, '<>');
  $query->leftJoin('party_attached_entity', 'pae', '%alias.data_set = :data_set AND %alias.eid = u.uid', array(':data_set' => 'user'));
  $query->isNull('pae.pid');
  $users = $query->execute()
    ->fetchAllAssoc('uid', PDO::FETCH_ASSOC);

  $batch = array(
    'operations' => array(),
    'finished' => 'party_user_sync_finished',
    'title' => t("Processing User Sync"),
    'init_message' => t('User Sync is starting'),
    'progress_message' => t('Processed @current out of @total'),
    'error_message' => t('User Sync Encountered an Error'),
    'file' => drupal_get_path('module', 'party_user') . '/party_user.batch.inc',
  );

  foreach ($users as $user) {
    $batch['operations'][] = array('party_user_batch_user_sync', array($user, array()));
  }

  batch_set($batch);

  $form_state['redirect'] = 'admin/config/party/user/sync';
}
