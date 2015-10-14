<?php
/**
 * @file
 * Email Field Confirm module hook implementations
 */


/**
 * Implements hook_email_field_confirm_emails_confirmed().
 *
 * An email field may be configured to retain the original value until the new
 * email has been confirmed. This hook handles updating the entity field with
 * the new value.
 *
 * An email/uid combination may also exist on multiple entity/fields.  A user
 * only needs to confirm an email address once. When an email address is
 * confirmed, all instances of that email/uid will be handled.
 */
function email_field_confirm_email_field_confirm_emails_confirmed($emails, $uid, $confirmed_by_uid, $status) {
  $params = array(
    'email' => $emails,
    'uid' => $uid,
  );
  // We get ALL records for this email/user to take action on. Assumption is
  // when this hook is called at least one of the records is confirmed so we can
  // take action on all records and then remove the pending records.
  $efc_data_results = email_field_confirm_read($params);
  $efc_data_array = $efc_data_results->fetchAllAssoc('email');

  // If we end up with no records then exit.
  if (empty($efc_data_array)) {
    return;
  }

  // Loop through each records and grab the instance settings. If the instance
  // settings dictate retaining the original email address then load the entity
  // and update that email field value with the newly confirmed email address.
  foreach ($efc_data_array as $email => $efc_data) {
    $instance = field_info_instance($efc_data->entity_type, $efc_data->field_name, $efc_data->bundle);

    $efc_settings = email_field_confirm_efc_settings($instance);
    if (empty($efc_settings)) {
      continue;
    }

    // The new email was not saved with the entity so we need to update the
    // entity field with this email value.
    if (!$efc_settings['save_value']) {
      $entities = entity_load($efc_data->entity_type, array($efc_data->entity_id), array(), TRUE);
      $entity = $entities[$efc_data->entity_id];
      $entity->{$efc_data->field_name}[LANGUAGE_NONE][0]['email'] = $efc_data->email;
      field_attach_update($efc_data->entity_type, $entity);
      entity_get_controller($efc_data->entity_type)->resetCache(array($efc_data->entity_id));
      watchdog('email_field_confirm', 'Email Confirmed: Updated field !field with %email.', array('!field' => $efc_data->field_name, '%email' => $efc_data->email));
    }
  }

  // Remove the remaining pending emails now that we have at least one confirmed
  // record.
  email_field_confirm_delete_pending_confirmed($params);
}


/**
 * Implements hook_email_field_confirm_delete_expired().
 *
 * A new email address may be saved on an entity field while in the pending
 * state. If the email address expires we potentially have the option to revert
 * the value back to the original value. This is true only with single-value
 * fields.
 *
 * We can also attempt to revert the field back to an empty field if it is not a
 * required field.
 */
function email_field_confirm_email_field_confirm_delete_expired($efc_data_array) {
  // If we end up with no records then exit.
  if (empty($efc_data_array)) {
    return;
  }

  // Loop through each records and grab the instance settings. If the instance
  // settings allow the new email addresses to be saved on the entity and we
  // have the original email value, then revert back to the original value. If
  // we don't have the original value but the field is not required then we can
  // set the field to blank.
  foreach ($efc_data_array as $email => $efc_data) {
    $instance = field_info_instance($efc_data->entity_type, $efc_data->field_name, $efc_data->bundle);

    $efc_settings = email_field_confirm_efc_settings($instance);
    if (empty($efc_settings)) {
      continue;
    }

    // We only need to alter the field value if we saved the new email address.
    if ($efc_settings['save_value']) {
      $email_original = !empty($efc_data->email_original) ? $efc_data->email_original : '';

      // If the original email value exists or if the field is not required then
      // reset the original value.
      if (!empty($email_original) || !$instance['required']) {
        $entities = entity_load($efc_data->entity_type, array($efc_data->entity_id), array(), TRUE);
        $entity = $entities[$efc_data->entity_id];
        $entity->{$efc_data->field_name}[LANGUAGE_NONE][0]['email'] = $efc_data->email_original;
        field_attach_update($efc_data->entity_type, $entity);
        entity_get_controller($efc_data->entity_type)->resetCache(array($efc_data->entity_id));
        watchdog('email_field_confirm', 'Email Reverted: Reset field !field with %email.', array('!field' => $efc_data->field_name, '%email' => $efc_data->email_original));
      }
    }
  }
}

/**
 * Delete any pending email records that have already been confirmed elsewhere
 * on the site.
 *
 * @param  array  $params
 *   (optional) An array of conditions to filter on.
 */
function email_field_confirm_delete_pending_confirmed($params = array()) {
  // Get a list of all confirmed emails so we can search for them in a pending
  // status elsewhere.
  $emails_confirmed = email_field_confirm_read_confirmed_emails($params);

  // Remove records that are pending using the confirmed emails as a filter
  $deleted = db_delete('email_field_confirm')
    ->condition('status', EMAIL_FIELD_CONFIRM_AWAITING_CONFIRMATION)
    ->condition('email', array_keys($emails_confirmed));
  foreach ($params as $key => $value) {
    // We already added the email condition so we cannot reuse that here.
    if ($key != 'email') {
      $deleted->condition($key, $value);
    }
  }
  $deleted_count = $deleted->execute();

  // Perform any messaging / logging.
  if ($deleted_count) {
    $message = t('!count pending emails removed.', array('!count' => $deleted_count));
    if (user_access('view any pending email field addresses')) {
      drupal_set_message($message);
    }
    watchdog('email_field_confirm', $message);
  }
}
