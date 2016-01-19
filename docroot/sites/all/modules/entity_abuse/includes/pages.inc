<?php

/**
 * @file
 * Contains page callbacks for the Entity Abuse module.
 */

/**
 * Ajax page callback for adding new abuse.
 */
function entity_abuse_add_abuse_ajax_callback($entity_type, $entity_id, $token) {

  $commands[] = array();

  // Validate the received token.
  if (!drupal_valid_token($token, $entity_type . $entity_id)) {
    return MENU_ACCESS_DENIED;
  }

  // Ensure that such entity exists.
  $entity = entity_load($entity_type, array($entity_id));
  if (empty($entity)) {
    return MENU_NOT_FOUND;
  }

  // Get entity from the array.
  $entity = reset($entity);

  // Get name of entity bundle.
  $info = entity_get_info($entity_type);
  $entity_bundle = empty($info['entity keys']['bundle']) ? $entity_type : $entity->{$info['entity keys']['bundle']};

  // Ensure that user allowed to add complain for this entity bundle.
  if (!entity_abuse_entity_bundle_enabled($entity_type, $entity_bundle)) {
    return MENU_ACCESS_DENIED;
  }

  // Include file with API functions.
  entity_abuse_include('api');
  entity_abuse_include('ajax');

  // Replace all abuse links with links to cancel abuse.
  $abuse_link = entity_abuse_get_abuse_link($entity_id, $entity_type, TRUE);
  $abuse_link_selector = '.entity-abuse-' . $entity_type . '-' . $entity_id;
  $commands[] = ajax_command_invoke($abuse_link_selector, 'replaceWith', array(render($abuse_link)));

  // Check if current user already complained for this entity.
  $user_has_complain = entity_abuse_user_has_complain($entity_id, $entity_type);

  if ($user_has_complain) {

    // Show in popup message that user already complained.
    $commands[] = entity_abuse_ajax_command_open_dialog(t('You already complained on this content.'));
  }
  else {

    // Check whether we should show the form with reason input or not.
    $reason_field_status = variable_get('entity_abuse_reason_field', ENTITY_ABUSE_REASON_FIELD_OPTIONAL);
    if ($reason_field_status == ENTITY_ABUSE_REASON_FIELD_DISABLED) {

      // Add a complain directly (without popup).
      $result = entity_abuse_add($entity_id, $entity_type);

      // Set message about results of adding abuse.
      $message = $result ? t('Your complain recieved. Thank you!') :
        t('You complain was not added. Probably, you already complained for this content.');

      $commands[] = entity_abuse_ajax_command_open_dialog($message);
    }
    else {

      // Load form where user allowed to input a complain reason.
      entity_abuse_include('forms');
      $form = drupal_get_form('entity_abuse_submit_form', $entity_type, $entity_id);

      // Show this form in a modal dialog.
      $commands[] = entity_abuse_ajax_command_open_dialog(render($form));
    }
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Ajax page callback for canceling existing abuse.
 */
function entity_abuse_cancel_abuse_ajax_callback($entity_type, $entity_id, $token) {

  $commands[] = array();

  // Validate the received token.
  if (!drupal_valid_token($token, $entity_type . $entity_id)) {
    return MENU_ACCESS_DENIED;
  }

  // Ensure that such entity exists.
  $entity = entity_load($entity_type, array($entity_id));
  if (empty($entity)) {
    return MENU_NOT_FOUND;
  }

  // Get entity from the array.
  $entity = reset($entity);

  // Get name of entity bundle.
  $info = entity_get_info($entity_type);
  $entity_bundle = empty($info['entity keys']['bundle']) ? $entity_type : $entity->{$info['entity keys']['bundle']};

  // Ensure that user allowed to add complain for this entity bundle.
  if (!entity_abuse_entity_bundle_enabled($entity_type, $entity_bundle)) {
    return MENU_ACCESS_DENIED;
  }

  // Include file with API functions.
  entity_abuse_include('api');
  entity_abuse_include('ajax');

  // Check if current user already complained for this entity.
  $user_has_complain = entity_abuse_user_has_complain($entity_id, $entity_type);

  if ($user_has_complain) {

    // Cancel one abuse for the content.
    entity_abuse_cancel($entity_id, $entity_type);
    $message = t('Your complain was canceled.');
  }
  else {
    $message = t('You did not complain for this content.');
  }

  // Replace all abuse links with links to cancel abuse.
  $abuse_link = entity_abuse_get_abuse_link($entity_id, $entity_type, TRUE);
  $abuse_link_selector = '.entity-abuse-' . $entity_type . '-' . $entity_id;
  $commands[] = ajax_command_invoke($abuse_link_selector, 'replaceWith', array(render($abuse_link)));

  // Open dialog with callback result message.
  $commands[] = entity_abuse_ajax_command_open_dialog($message, array('title' => t('Cancel complain')));

  return array('#type' => 'ajax', '#commands' => $commands);
}
