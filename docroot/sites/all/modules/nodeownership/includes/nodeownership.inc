<?php

/**
 * @file
 * Helper file, contains reusable code.
 */

/**
 * Approves a claim.
 */
function nodeownership_claim_approve($claim) {
  if ($claim->uid == 0) {
    drupal_set_message(t('Error: you cannot approve claim #@claim_id because it was made by an anonymous user. You will have to manually create a new user account and set ownership for this request. If you want to remove it from the queue, delete it using the provided Views Bulk Operatons actions.', array('@claim_id' => $claim->id)), 'error');
    return FALSE;
  }
  elseif ($claim->status != 0) {
    drupal_set_message(t('Error: you cannot approve claim #@claim_id because it is not a pending claim.', array('@claim_id' => $claim->id)), 'error');
    return FALSE;
  }

  $node = node_load($claim->nid);
  $node->uid = $claim->uid;
  node_save($node);

  // Update nodeownership table for node owner status.
  $num_updated = db_update('nodeownership')
    ->fields(array('status' => 1))
    ->condition('id', $claim->id, '=')
    ->execute();

  nodeownership_send_mail('approve_accept', $claim->uid);
  watchdog('nodeownership', 'Approved claim @claim_id by user @uid for node @nid', array('@claim_id' => $claim->id, '@uid' => $claim->uid, '@nid' => $claim->nid));

  return $num_updated ? TRUE : FALSE;
}

/**
 * Declines a claim.
 */
function nodeownership_claim_decline($claim) {
  if ($claim->uid == 0) {
    drupal_set_message(t('Error: you cannot decline claim #@claim_id because it was made by an anonymyous user. If you want to remove it from the queue, delete it using the provided Views Bulk Operatons actions.', array('@claim_id' => $claim->id)), 'error');
    return FALSE;
  }
  if ($claim->status != 0) {
    drupal_set_message(t('Error: you cannot decline claim #@claim_id for <em>@title</e> because it is not a pending claim.', array('@claim_id' => $claim->id)), 'error');
    return FALSE;
  }

  $num_updated = db_update('nodeownership')
    ->fields(array('status' => 2))
    ->condition('id', $claim->id, '=')
    ->execute();

  nodeownership_send_mail('approve_decline', $claim->uid);
  watchdog('nodeownership', 'Declined claim @claim_id by user @uid for node @nid', array('@claim_id' => $claim->id, '@uid' => $claim->uid, '@nid' => $claim->nid));

  return $num_updated ? TRUE : FALSE;
}

/**
 * Sets parameters for outgoing mail and execute mail delivery
 * for admin
 */
function nodeownership_send_mail($type, $uid) {
  global $language;

  $to = db_select('users', 'u')
    ->fields('u', array('mail', 'name'))
    ->condition('uid', $uid)
    ->execute()
    ->fetchAssoc();
  $mail = drupal_mail('nodeownership', 'admin_' . $type, $to['mail'], $language);

  $message = array(
    'approve_accept' => 'Request approved. A confirmation message has been sent to user.',
    'approve_decline' => 'Request declined. A confirmation message has been sent to user.',
  );
  drupal_set_message(t($message[$type]));
  drupal_goto('admin/content/claims');
}

/**
 * Function for setting default mail content and saving custom mail content by admin.
 *
 * @param $key
 *   parameter for different mail content.
 * @param $language
 *   It is optional.
 * @param $variables
 *   a set of variables to be used inside mail content.
 *
 * @return string
 *   it returns the subject and body of different mail.
 */
function _nodeownership_mail_text($key, $language = NULL, $variables = array()) {
  $langcode = isset($language) ? $language->language : NULL;
  if ($admin_setting = variable_get($key, FALSE)) {
    // An admin setting overrides the default string.
    return strtr($admin_setting, $variables);
  }
  else {
    // No override, return default string.
    switch ($key) {
      case 'user_claim_subject':
        return t('Node Claim By !username at !site', $variables, array('langcode' => $langcode));

      case 'user_claim_body':
        return t("Hello!!,\n\n User !username has claim a node. So Please review it and confirm as soon as possible.", $variables, array('langcode' => $langcode));

      case 'admin_approve_subject':
        return t('Approval for claimed node', $variables, array('langcode' => $langcode));

      case 'admin_approve_body':
        return t("Hello!!,\n\n Admin has approved your claimed for the node. From now onward you are the owner of the claimed node.", $variables, array('langcode' => $langcode));

      case 'admin_decline_subject':
        return t('Approval for claimed node', $variables, array('langcode' => $langcode));

      case 'admin_decline_body':
        return t("Sorry!!,\n\n Admin has rejected your claimed for the node. For further details contact the administrator.", $variables, array('langcode' => $langcode));
    }
  }
}

/**
 * Implementation for the dynamic variable used in mail.
 */
function nodeownership_mail_tokens($language) {
  global $base_url;
  global $user;

  $tokens = array(
    '!username' => $user->name,
    '!site' => variable_get('site_name', 'Drupal'),
    '!uri' => $base_url,
    '!uri_brief' => preg_replace('!^https?://!', '', $base_url),
    '!mailto' => $user->mail,
    '!date' => format_date(REQUEST_TIME, 'medium', '', NULL, $language->language),
  );
  return $tokens;
}
