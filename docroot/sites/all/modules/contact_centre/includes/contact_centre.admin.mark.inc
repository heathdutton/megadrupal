<?php
/**
 * @file
 * Contact notes admin reply include file.
 */

function contact_centre_mark($form, &$form_state, $mid, $op, $current_status) {
  if (is_null($mid)) {
    drupal_set_message(t('No message was selected'), 'warning');
    drupal_goto('admin/contact');
  }

  $new_status = $current_status == 0 ? REQUEST_TIME : 0;

  // set the new value of the field in the form as a hidden value
  $form['field_value'] = array(
    '#type' => 'hidden',
    '#value' => $new_status,
  );

  switch ($op) {
    case 'open-status':
      // tell the form which field we will be operating on
      $form['field'] = array(
        '#type' => 'hidden',
        '#value' => 'opened',
      );
      if ($current_status == 0) {
        $new_status_text = t('opened');
      }
      else {
        $new_status_text = t('unopened');
      }
      break;
    case 'resolve-status':
      // tell the form which field we will be operating on
      $form['field'] = array(
        '#type' => 'hidden',
        '#value' => 'resolved',
      );
      if ($current_status == 0) {
        $new_status_text = t('resolved');
      }
      else {
        $new_status_text = t('unresolved');
      }
      break;
  }

  $form['mid'] = array(
    '#type' => 'hidden',
    '#value' => $mid,
  );

  $question     = t('Do you really want to mark message !mid as !status', array('!mid' => $mid, '!status' => strToUpper($new_status_text)));
  $path         = CONTACT_CENTRE_PATH;
  $description  = t('You can revert this action if necessary.');
  $yes          = t('Mark !status', array('!status' => $new_status_text));

  return confirm_form(
    $form,
    $question,
    $path,
    $description,
    $yes
  );
}

function contact_centre_mark_submit($form, &$form_state) {
  global $user;

  $result = db_update('contact_centre_messages')
    ->fields(array(
      $form_state['values']['field'] => $form_state['values']['field_value'],
    ))
    ->condition('id', $form_state['values']['mid'], '=')
    ->execute();

  if ($result != 0) {
    drupal_set_message(t('Message !id updated successfully.', array('!id' => $form_state['values']['mid'])), 'status');
    // insert a record into the notes for the message recording the action taken
    $action_text = $form_state['values']['field_value'] == 0 ? 'un' . $form_state['values']['field'] : $form_state['values']['field'];
    $record = array(
      'cid'         => $form_state['values']['mid'],
      'uid'         => $user->uid,
      'action'      => 'Marked as ' . $action_text,
      'action_type' => 'note',
      'created'     => REQUEST_TIME,
    );
    drupal_write_record('contact_centre_actions', $record);
  }
  else {
    drupal_set_message(t('An error occurred setting the status of message !id.', array('!id' => $form_state['values']['mid'])), 'error');
  }

  drupal_goto('admin/contact');
}
