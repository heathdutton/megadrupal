<?php
/**
 * @file
 * Administration forms and pages.
 */

/**
 * Custom function for register pre-approved settings form.
 */
function register_preapproved_settings_form($form, &$form_id) {
  $form['register_preapproved_count'] = array(
    '#type' => 'radios',
    '#title' => t('Include pre-existing account email addresses in list page counts'),
    '#default_value' => variable_get('register_preapproved_count', 0),
    '#options' => array(t('Exclude'), t('Include')),
    '#description' => t('Choose to exclude/include pre-existing account email addresses created prior to the corresponding pattern creation timestamp.'),
  );
  $form['register_preapproved_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Pre-approved message'),
    '#default_value' => variable_get('register_preapproved_message', t('You have been pre-approved and granted access to %site_name.')),
    '#size' => 100,
    '#maxlength' => 250,
    '#description' => t('This message will appear to pre-approved users after registration. Available variable: %site_name'),
  );
  $form['register_preapproved_restrict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Retrict registration to only pre-approved email addresses.'),
    '#default_value' => variable_get('register_preapproved_restrict', 0),
  );

  return system_settings_form($form);
}

/**
 * Custom function for register pre-approved settings form.
 */
function register_preapproved_roles_form($form, &$form_id) {
  $register_preapproved_roles = user_roles(TRUE);
  unset($register_preapproved_roles[DRUPAL_AUTHENTICATED_RID]);

  if (count($register_preapproved_roles)) {
    $form['register_preapproved_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Pre-approved roles'),
      '#default_value' => variable_get('register_preapproved_roles', array()),
      '#options' => $register_preapproved_roles,
      '#description' => t('Select the custom roles available for assignment when adding email addresses and domain patterns.'),
    );
  }

  return system_settings_form($form);
}

/**
 * List Form
 * FORM
 *
 * Custom callback function to list pre-approved email addresses or domains.
 */
function register_preapproved_list_form($form, &$form_state) {
  $headers = array(
    'email' => array('data' => t('E-mail/Domain'), 'field' => 'rp.mail', 'sort' => 'desc'),
    'timestamp' => array('data' => t('Added'), 'field' => 'rp.timestamp'),
    'count' => array('data' => t('Count')),
    'roles' => array('data' => t('Roles')),
    'operations' => array('data' => t('Operations')),
  );

  // Get all prereg records and set the pager.
  $select = db_select('register_preapproved', 'rp')
    ->fields('rp')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->limit(50)
    ->orderByHeader($headers);
  $results = $select->execute();

  $options = array();
  while ($result = $results->fetchAssoc()) {
    // Create the subquery to count the appropriate matching users.
    $sub_select1 = db_select('register_preapproved', 'rp');
    $sub_select1->join('users', 'u', 'u.mail = rp.mail AND rp.mail <> :mail', array(':mail' => $result['mail']));
    $sub_select1->fields('rp', array('mail'));

    // Start the primary query to count appropriate matching users.
    $select = db_select('users', 'u')
      ->fields('u')
      ->condition('u.mail', '%' . db_like($result['mail']), 'LIKE')
      ->condition('u.mail', $sub_select1, 'NOT IN');

    // If we are only counting users who registered AFTER the preapproval was
    // created, add another filter.
    if (!variable_get('register_preapproved_count')) {
      $select->condition('u.created', $result['timestamp'], '>');
    }

    // Run the count.
    $count = $select->execute()->rowCount();

    // Compile date for the table
    $options[$result['rpid']] = array(
      'email' => $result['mail'],
      'timestamp' => t('!date (!ago ago)', array('!date' => date("M j, Y", $result['timestamp']), '!ago' => format_interval(time() - $result['timestamp']))),
      'count' => $count,
      'roles' => implode(', ', unserialize($result['roles'])),
      'operations' => l(t('edit roles'), 'admin/people/register_preapproved/' . $result['rpid'] . '/edit'),
    );
  }

  //Build the tableselect.
  $form['delete'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $options,
    '#empty' => t('No e-mail addresses or domain patterns found.'),
  );

  $form['pager'] = array('#theme' => 'pager');
  $form['submit'] = array('#type' => 'submit', '#value' => t('Delete'));

  return $form;
}

/**
 * List Form
 * VALIDATE
 *
 * Implements hook_validate().
 */
function register_preapproved_list_form_validate($form, &$form_state) {
  $form_state['values']['delete'] = array_filter($form_state['values']['delete']);
  if (count($form_state['values']['delete']) == 0) {
    form_set_error('', t('No email addresses or domain patterns selected.'));
  }
}

/**
 * List Form
 * SUBMIT
 *
 * Implements hook_submit().
 */
function register_preapproved_list_form_submit($form, &$form_state) {
  $deleted = 0;
  $deletes = array_filter($form_state['values']['delete']);
  foreach ($deletes as $rpid) {
    if (db_delete('register_preapproved')->condition('rpid', $rpid)->execute()) {
      $deleted++;
    }
  }
  drupal_set_message(t('%deleted email addresses or domain patterns successfully deleted.', array('%deleted' => $deleted)));
}

/**
 * Add Form
 * FORM
 *
 * Register pre-approved add form.
 */
function register_preapproved_add_form($form, &$form_state) {
  $form['register_preapproved']['emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Pre-approved email addresses and domains'),
    '#required' => TRUE,
    '#description' => t('Enter a list of email addresses and domain patterns, one entry per line. Valid patterns are full email addresses or domains beginning with the @ symbol. Ex. @domain.com'),
  );

  $register_preapproved_roles = array_filter(variable_get('register_preapproved_roles', array()));

  if (count($register_preapproved_roles)) {
    // retrieve user roles excluding anonymous
    $user_roles = user_roles(TRUE);
    // create options from default role selections
    foreach ($register_preapproved_roles as $rid) {
      // make sure pre-approved role exists
      if (isset($user_roles[$rid])) {
        $options[$rid] = $user_roles[$rid];
      }
    }
    $form['register_preapproved']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Pre-approved roles'),
      '#options' => $options,
      '#description' => t('Select the custom roles automatically assigned to these pre-approved users during registration.'),
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Add'));

  return $form;
}

/**
 * Add Form
 * VALIDATE
 *
 * Implements hook_validate().
 */
function register_preapproved_add_form_validate($form, &$form_state) {
  $errors = array();

  // preg_split more reliable than split to eliminate empty array elements
  $emails = preg_split('/[\n\r]+/', $form_state['values']['emails']);

  foreach ($emails as $email) {
    if ($email = trim($email)) {
      $args = array('%email' => $email);

      // portion of validation logic copied from _user_edit_validate()
      if (!valid_email_address($email) && !valid_domain($email)) {
        $errors[] = t('The e-mail address or domain %email is not valid.', $args);
      }
      elseif (db_select('users', 'u')->condition('u.mail', $email)->fields('u')->execute()->rowCount()) {
        drupal_set_message(t('The e-mail address %email is already registered.', $args), 'warning');
        $form_state['values']['emails'] = str_replace($email, '', $form_state['values']['emails']);
      }
      elseif (db_select('register_preapproved', 'rp')->condition('rp.mail', $email)->fields('rp')->execute()->rowCount()) {
        drupal_set_message(t('The e-mail address or domain %email is already pre-approved.', $args), 'warning');
        $form_state['values']['emails'] = str_replace($email, '', $form_state['values']['emails']);
      }
      elseif (drupal_strlen($email) > EMAIL_MAX_LENGTH) {
        $errors[] = t('The e-mail address or domain %email cannot not exceed !maxlength characters.', array('%email' => $email, '!maxlength' => EMAIL_MAX_LENGTH));
      }
    }
  }

  if (count($errors)) {
    array_unshift($errors, t('The following problems occurred while preparing to add the email addresses and/or domain patterns and must be corrected before continuing:') . '<br />');
    form_set_error('emails', implode('<br />', $errors));
  }
}

/**
 * Add Form
 * SUBMIT
 *
 * Implements hook_submit().
 */
function register_preapproved_add_form_submit($form, &$form_state) {
  $added = 0;
  $register_preapproved_roles = array();
  $emails = preg_split('/[\n\r]+/', $form_state['values']['emails']);
  $original = count($emails);

  // remove duplicate entries, if any
  $emails = array_unique($emails);

  if (isset($form_state['values']['roles'])) {
    // retrieve user roles excluding anonymous
    $user_roles = user_roles(TRUE);
    $register_preapproved_roles = array_filter($form_state['values']['roles']);

    // create pattern default role selections
    foreach ($register_preapproved_roles as $rid) {
      $register_preapproved_roles[$rid] = $user_roles[$rid];
    }
  }

  foreach ($emails as $email) {
    if ($email = trim($email)) {
      $record = new stdClass();
      $record->mail = $email;
      $record->roles = serialize($register_preapproved_roles);
      $record->timestamp = time();

      switch (drupal_write_record('register_preapproved', $record)) {
        case SAVED_NEW:
        case SAVED_UPDATED:
          $added++;
      }
    }
  }

  drupal_set_message(t('%added pre-approved email addresses or domain patterns successfully added.', array('%added' => $added)));

  if ($original != $added) {
    // alert admin of duplicate entries
    $adjusted = $original - $added;
    drupal_set_message(t('%adjusted duplicate email addresses or domain patterns were detected and automatically excluded.', array('%adjusted' => $adjusted)), 'warning');
  }

  drupal_goto('admin/people/register_preapproved/list');
}

/**
 * Edit Form
 * CALLBACK PAGE
 *
 * Custom callback function for register pre-approved edit form.
 */
function register_preapproved_edit($rpid) {
  drupal_set_title(t('Register pre-approved'));

  if (is_numeric($rpid)) {
    $result = db_select('register_preapproved', 'rp')->condition('rp.rpid', $rpid)->fields('rp')->execute();

    while ($pattern = $result->fetchAssoc()) {
      if (count(array_filter(variable_get('register_preapproved_roles', array())))) {
        return drupal_get_form('register_preapproved_edit_form', $pattern);
      }
      else {
        drupal_set_message(t('There are no default roles defined. You can define some on the !settings page.', array('!settings' => l(t('register pre-approved settings'), 'admin/people/register_preapproved/settings'))), 'error');
      }
    }
  }
  else {
    drupal_set_message(t('The email address or domain record was not found.'), 'error');
  }

  drupal_goto('admin/people/register_preapproved/list');
}

/**
 * Edit Form
 * FORM
 *
 * Register pre-approved edit form.
 */
function register_preapproved_edit_form($form, &$form_state, $pattern) {
  $register_preapproved_roles = array_filter(variable_get('register_preapproved_roles', array()));
  if (count($register_preapproved_roles)) {
    // retrieve user roles excluding anonymous
    $user_roles = user_roles(TRUE);
    foreach ($register_preapproved_roles as $rid) {
      // make sure pre-approved role exists
      if (isset($user_roles[$rid])) {
        $options[$rid] = $user_roles[$rid];
      }
    }
    $form['register_preapproved']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Pre-approved roles for %title', array('%title' => $pattern['mail'])),
      '#default_value' => array_keys(unserialize($pattern['roles'])),
      '#options' => $options,
      '#description' => t('Select the custom roles automatically assigned for this pattern during registration.'),
    );
  }

  $form['rpid'] = array('#type' => 'value', '#value' => $pattern['rpid']);
  $form['mail'] = array('#type' => 'value', '#value' => $pattern['mail']);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));

  return $form;
}

/**
 * Edit Form
 * SUBMIT
 *
 * Implements hook_submit().
 */
function register_preapproved_edit_form_submit($form, &$form_state) {
  if (isset($form_state['values']['roles'])) {
    // retrieve user roles excluding anonymous
    $user_roles = user_roles(TRUE);
    $register_preapproved_roles = array_filter($form_state['values']['roles']);

    // create pattern default role selections
    foreach ($register_preapproved_roles as $rid) {
      $register_preapproved_roles[$rid] = $user_roles[$rid];
    }

    $record['roles'] = serialize($register_preapproved_roles);
    $record['rpid'] = $form_state['values']['rpid'];

    if (drupal_write_record('register_preapproved', $record, 'rpid')) {
      drupal_set_message(t('The custom role selections for @email were successfully updated.', array('@email' => $form_state['values']['mail'])));
    }
  }

  drupal_goto('admin/people/register_preapproved/list');
}
