<?php
/**
 * @file
 * Contact notes admin view message include file.
 */

function contact_centre_view_message($mid = NULL) {
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

  return $build;
}
