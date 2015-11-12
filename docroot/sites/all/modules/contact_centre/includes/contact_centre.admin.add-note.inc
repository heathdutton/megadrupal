<?php
/**
 * @file
 * Contact notes admin add note include file.
 */

function contact_centre_add_note($form, &$form_state, $mid) {
  if (is_null($mid)) {
    drupal_set_message(t('No message was selected for viewing'), 'warning');
    drupal_goto(CONTACT_CENTRE_PATH);
  }

  $date_type    = variable_get('contact_centre_date_format', 'medium');
  $date_format  = variable_get('date_format_' . $date_type, $date_type);

  // we can now mark this message as opened
  $result = db_update('contact_centre_messages')
    ->fields(array(
      'opened' => REQUEST_TIME,
    ))
    ->condition('id', $mid, '=')
    ->execute();

  drupal_add_css(drupal_get_path('module', 'contact_centre') . '/css/contact_centre.css');

  // get the message details
  $query = db_select('contact_centre_messages', 'm');

  // join the contact table so we can extract the contact categories
  $query->join('contact', 'c', 'c.cid = m.cid');
  $query->fields('m', array('id', 'uid', 'name', 'mail', 'subject', 'message', 'created', 'opened', 'resolved'))
    ->fields('c', array('category'))
    ->condition('m.id', $mid, '=');

  // execute the query and fetch all the results back into an associative array
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  // get any notes/replies recorded against the selected message
  $notes_query = db_select('contact_centre_actions', 'c');
  $notes_query->join('users', 'u', 'u.uid = c.uid');
  $notes_query->fields('c', array('action', 'action_type', 'created'))
    ->fields('u', array('name'))
    ->condition('c.cid', $mid, '=')
    ->orderBy('c.created', 'desc');

  $notes_results = $notes_query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $notes = array();
  foreach ($notes_results as $key => $notes_result) {
    $notes[] = array(
      'data' => array(
        array('data' => format_date($notes_result['created'], $date_type, $date_format)),
        array('data' => check_plain($notes_result['name'])),
        array('data' => check_plain($notes_result['action'])),
        array('data' => $notes_result['action_type'] == 'note' ? t('No') : t('Yes')),
      ),
    );
  }

  $build = theme(
    'contact_centre_message_view',
    array(
      'message_id'        => $mid,
      'contact_category'  => $result[0]['category'],
      'uid'               => $result[0]['uid'],
      'user_name'         => $result[0]['name'],
      'user_email'        => $result[0]['mail'],
      'subject'           => $result[0]['subject'],
      'message'           => $result[0]['message'],
      'created'           => format_date($result[0]['created'], $date_type, $date_format),
      'opened'            => $result[0]['opened'] == 0 ? t('No') : t('Yes'),
      'resolved'          => $result[0]['resolved'] == 0 ? t('No') : t('Yes'),
      'notes'             => $notes,
    )
  );

  $form['new_note'] = array(
    '#type' => 'fieldset',
    '#title' => t('New Note'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['new_note']['note'] = array(
    '#type' => 'textarea',
    '#title' => t('Note'),
    '#required' => TRUE,
  );

  $form['new_note']['send_as_reply'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send as Reply'),
    '#description' => t('Check this box if you want to send the note above to the original sender.'),
  );

  $form['new_note']['mark_as_resolved'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark as Resolved'),
    '#description' => t('Check this box if you want to mark this contact as resolved and requiring no further action.'),
  );

  $form['new_note']['actions'] = array('#type' => 'actions');
  $form['new_note']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['message_details'] = array(
    '#markup' => $build,
  );

  $form['mid'] = array(
    '#type' => 'hidden',
    '#value' => $mid,
  );

  return $form;
}

/**
 * Submit handler for contact_centre_add_note form.
 */
function contact_centre_add_note_submit($form, &$form_state) {
  global $user;

  // pick up the submitted values
  $uid = $user->uid;
  $mid = $form_state['values']['mid'];
  $created = REQUEST_TIME;
  $action = $form_state['values']['note'];
  $action_type = $form_state['values']['send_as_reply'] == 0 ? 'note' : 'reply';

  // build the record array for storing in the database
  $record = array(
    'cid'         => $mid,
    'uid'         => $uid,
    'action'      => $action,
    'action_type' => $action_type,
    'created'     => $created,
  );

  // execute the insert
  $result = drupal_write_record('contact_centre_actions', $record);

  // check the status returned from the insert to make sure it was ok
  if ($result == SAVED_NEW) {
    drupal_set_message(t('Note saved successfully.'), 'status');
  }
  else {
    drupal_set_message(t('The note could not be saved.'), 'error');
  }

  // check to see if we need to send this note to the sender of the message
  if ($action_type == 'reply') {
    // get the email address we are sending to
    $email_address = db_query('SELECT `mail` FROM {contact_centre_messages} WHERE id = :mid', array(':mid' => $mid))->fetchField();
    $sent = drupal_mail(
      'contact_centre',
      'action_reply',
      $email_address,
      language_default(),
      array(
        'mid' => $mid,
        'note' => check_plain($action),
        'subject' => t('In response to your website contact submission...'),
      ),
      NULL,
      TRUE
    );

    // check mail was sent and set an appropriate message
    if ($sent['result']) {
      drupal_set_message(t('A copy of this note has been sent to the original sender.'), 'status');
    }
    else {
      drupal_set_message(t('It was not possible to send a copy of this note to the original sender.'), 'warning');
    }
  }

  // check to see if the message should be marked as resolved
  if ($form_state['values']['mark_as_resolved'] == '1') {
    $result = db_update('contact_centre_messages')
      ->fields(array(
        'resolved' => REQUEST_TIME,
      ))
      ->condition('id', $mid, '=')
      ->execute();

    if ($result != 0) {
      drupal_set_message(t('Message !id has been marked as resolved.', array('!id' => $mid)), 'status');
    }
    else {
      drupal_set_message(t('An error occurred while attempting to mark message !id as resolved.', array('!id' => $mid)), 'error');
    }
  }

  drupal_goto(CONTACT_CENTRE_PATH . '/' . $mid . '/view');
}
