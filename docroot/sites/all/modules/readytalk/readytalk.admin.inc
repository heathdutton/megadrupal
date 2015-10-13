<?php
/**
 * @file
 * Admin forms.
 */

/**
 * Form constructor for the Readytalk webform management form.
 *
 * @param array $form
 *   Contains form array.
 * @param array $form_state
 *   An array that stores information about the form.
 * @param object $webform_node
 *   The webform node that the readytalk information is attached to.
 *
 * @see readytalk_components_form_submit()
 * @see readytalk_components_form_delete_submit()
 *
 * @ingroup readytalk_webform_forms
 */
function readytalk_components_form($form = array(), $form_state = array(), $webform_node) {
  $form = array();
  $readytalk_user_options = readytalk_get_all_users_options();

  $form['#tree'] = TRUE;
  $form['#node'] = $webform_node;
  $nid = $webform_node->nid;
  $record = readytalk_load($nid);

  $form['#record'] = $record;

  // Set Defaults.
  if (!isset($record->is_active)) {
    $record = new stdClass();
    $record->is_active = TRUE;
    $record->meeting_id = "";
    $record->username = "";
    $record->password = "";
    $record->send_confirmation_email = TRUE;
    $record->uid = NULL;
  }

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );
  $form['details']['is_active'] = array(
    '#title' => t('Is active'),
    '#type' => 'checkbox',
    '#default_value' => $record->is_active,
  );
  $form['details']['meeting_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Readytalk Meeting ID'),
    '#default_value' => $record->meeting_id,
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('Click here to view list of available meeting ids.'),
    '#required' => TRUE,
  );
  $form['details']['uid'] = array(
    '#type' => 'select',
    '#title' => t('Readytalk Username'),
    '#default_value' => $record->uid,
    '#options' => $readytalk_user_options,
    '#description' => t('The Readytalk username associated to the meeting.'),
    '#required' => TRUE,
  );
  // Only show the start date field once the meeting id is entered and the start
  // date is returned from ReadyTalk.
  if (isset($record->start_date)) {
    $form['details']['start_date'] = array(
      '#type' => 'textfield',
      '#title' => t('Start Date'),
      '#default_value' => date('l jS \of F Y h:i:s A', $record->start_date),
      '#size' => 60,
      '#maxlength' => 255,
      '#disabled' => TRUE,
    );
  }
  $form['details']['send_confirmation_email'] = array(
    '#title' => t('Send Confirmation Email'),
    '#type' => 'checkbox',
    '#default_value' => $record->send_confirmation_email,
  );

  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Info'),
  );
  $form['instructions']['markup'] = array(
    '#markup' => t('Map the fields to the Ready Talk fields. You can find all the available field names <a href="https://cc.readytalk.com/api/1.3/rest#RegistrationService-createRegistration">here</a>.'),
  );

  $form['components'] = array(
    '#tree' => TRUE,
  );
  foreach ($webform_node->webform['components'] as $key => $component) {
    $form['components'][$key] = array(
      '#component' => $component,
      'key' => array(
        '#title' => 'Field name',
        '#type' => 'textfield',
        '#size' => 25,
        '#default_value' => isset($record->data[$component['form_key']]['key']) ? $record->data[$component['form_key']]['key'] : '',
      ),
    );
  }
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('readytalk_components_form_delete_submit'),
  );

  return $form;
}

/**
 * Form submission handler for readytalk_components_form().
 *
 * Saves the readytalk webform information to the database.
 *
 * @param array $form
 *   The form element.
 * @param array $form_state
 *   An array that stores information about the form.
 */
function readytalk_components_form_submit($form = array(), $form_state = array()) {
  $node = $form['#node'];

  $record = $form['#record'];
  if ($record) {
    $update = array('nid');
  }
  else {
    $record = new stdClass();
    $update = array();
  }

  $data = array();
  foreach (element_children($form['components']) as $k) {
    $component = $form['components'][$k]['#component'];
    $data[$component['form_key']]['key'] = $form_state['values']['components'][$k]['key'];
  }

  // Get start date from meeting ID.
  $readytalk_user = readytalk_userload($form_state['values']['details']['uid']);
  $readytalk = new Readytalk($readytalk_user->telephone_number, $readytalk_user->access_code, $readytalk_user->pass_code);
  $meeting_info = $readytalk->get_meeting_info($form_state['values']['details']['meeting_id']);
  if (isset($meeting_info->meeting->startDate)) {
    $start_date = (string) $meeting_info->meeting->startDateIso8601;
    $timestamp = strtotime($start_date);
  }
  else {
    form_set_error('meeting_id', t('Could not get Meeting start date. Please verify that the meeting id is correct.'));
  }

  $record->nid = $node->nid;
  $record->is_active = (bool) $form_state['values']['details']['is_active'];
  $record->meeting_id = $form_state['values']['details']['meeting_id'];
  $record->uid = $form_state['values']['details']['uid'];
  $record->send_confirmation_email = (bool) $form_state['values']['details']['send_confirmation_email'];
  if (isset($timestamp)) {
    $record->start_date = $timestamp;
  }
  $record->data = $data;
  drupal_write_record('readytalk_webform', $record, $update);

  drupal_set_message(t('Readytalk webform configuration has been saved.'));
}

/**
 * Form submission handler for readytalk_components_form().
 *
 * Deletes the readytalk webform settings from the database for the node.
 *
 * @param array $form
 *   The form element.
 * @param array $form_state
 *   An array that stores information about the form.
 */
function readytalk_components_form_delete_submit($form = array(), &$form_state = array()) {
  $node = $form['#node'];
  db_delete('readytalk_webform')
    ->condition('nid', $node->nid)
    ->execute();

  drupal_set_message(t('Readytalk settings for this webform have been deleted.'), 'status');
}

/**
 * Theme callback for readytalk_webform_components_form().
 *
 * @param array $form
 *   The form element.
 *
 * @return string $output
 *   The themed string.
 */
function theme_readytalk_components_form($form = array()) {
  $form = $form['form'];
  $rows = array();
  $output = '';

  $header = array(t('Name'), t('Type'), t('Readytalk Field Name'));

  foreach (element_children($form['components']) as $componenent) {
    $row = array();
    $row[] = $form['#node']->webform['components'][$componenent]['name'];
    $row[] = $form['#node']->webform['components'][$componenent]['type'];
    // Remove field label.
    unset($form['components'][$componenent]['key']['#title']);
    $row[] = drupal_render($form['components'][$componenent]['key']);
    $rows[] = $row;
  }

  $output .= drupal_render($form['details']);
  $output .= drupal_render($form['instructions']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form constructor for the Readytalk credentials for a user.
 *
 * @param array $form
 *   Contains form array.
 * @param array $form_state
 *   Array containing form state values.
 * @param object $user
 *   The user object.
 *
 * @see readytalk_user_form_submit()
 *
 * @ingroup readytalk_user_forms
 */
function readytalk_user_form($form = array(), $form_state = array(), $user) {
  // Check if there is a record for this user already.
  $readytalk_user = readytalk_userload($user->uid);

  // Set defaults if no records.
  if (!$readytalk_user) {
    $readytalk_user = new stdClass();
    $readytalk_user->uid = $user->uid;
    $readytalk_user->telephone_number = NULL;
    $readytalk_user->access_code = NULL;
    $readytalk_user->pass_code = NULL;
  }

  $form['#user'] = $readytalk_user;
  $form['telephone_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Conferencing Telephone Number'),
    '#default_value' => $readytalk_user->telephone_number,
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['access_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Code'),
    '#default_value' => $readytalk_user->access_code,
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['pass_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Pass Code'),
    '#default_value' => $readytalk_user->pass_code,
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for readytalk_user_form().
 *
 * Saves the readytalk user to the database.
 *
 * @param array $form
 *   The form element.
 * @param array $form_state
 *   An array that stores information about the form.
 */
function readytalk_user_form_submit($form = array(), $form_state = array()) {
  $readytalk_user = $form['#user'];

  if ($readytalk_user->telephone_number) {
    $update = array('uid');
  }
  else {
    $update = array();
  }

  $readytalk_user->uid = $readytalk_user->uid;
  $readytalk_user->telephone_number = $form_state['values']['telephone_number'];
  $readytalk_user->access_code = $form_state['values']['access_code'];
  $readytalk_user->pass_code = $form_state['values']['pass_code'];

  drupal_write_record('readytalk_user', $readytalk_user, $update);
}

/**
 * Theme callback for readytalk_user_form().
 *
 * @param array $form
 *   The form element.
 *
 * @return string $output
 *    The themed string.
 */
function theme_readytalk_user_form($form = array()) {
  $form = $form['form'];
  $rows = array();
  $output = '';

  $output .= drupal_render($form['details']);
  $output .= drupal_render_children($form);

  // Get Meeting info from Readytalk if the user has their info already.
  if ($form['telephone_number']['#value'] && $form['telephone_number']['#value'] && $form['pass_code']['#value']) {
    $readytalk = new Readytalk($form['telephone_number']['#value'], $form['access_code']['#value'], $form['pass_code']['#value']);
    $header = array(t('Meeting ID'), t('Title'), t('Date'));
    $rows = $readytalk->get_meetings_info();
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }

  return $output;
}

/**
 * Load all webform enabled nodes with readytalk_webform post url.
 *
 * @return string
 *   Settings object or false on failure.
 */
function readytalk_enabled_readytalk_webform_load() {
  $results = db_query("SELECT * FROM {readytalk_webform}");
  $header = array(t('Node'), t('Edit Settings'), t('Meeting ID'), t('Active'));
  $rows = array();

  if ($results->rowCount() > 0) {
    foreach ($results as $result) {
      $node = node_load($result->nid);
      $rows[] = array(
        l($node->title, "node/{$result->nid}"),
        l(t('Edit'), "node/{$result->nid}/webform/readytalk"),
        $result->meeting_id,
        $result->is_active ? "Yes" : "No",
      );
    }

    $output = theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output = '<p>No Readytalk webforms have been enabled.</p>';
  }

  return $output;
}
