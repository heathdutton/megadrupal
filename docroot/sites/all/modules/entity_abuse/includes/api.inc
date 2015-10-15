<?php

/**
 * @file
 * Contains API functions for the module.
 */

/**
 * Adds new abuse for an entity.
 */
function entity_abuse_add($entity_id, $entity_type, $reason = '', $uid = NULL) {

  if ($uid === NULL) {
    $uid = $GLOBALS['user']->uid;
  }

  // Create new entity abuse object.
  $entity_abuse = new stdClass();
  $entity_abuse->entity_id = $entity_id;
  $entity_abuse->entity_type = $entity_type;
  $entity_abuse->reason = $reason;
  $entity_abuse->uid = $uid;
  $entity_abuse->timestamp = REQUEST_TIME;
  $entity_abuse->source = ip_address();

  // Get database transaction.
  $transaction = db_transaction();
  try {

    // Write abuse to the table.
    drupal_write_record('entity_abuse', $entity_abuse);

    // Calculate new amount of abuses for the current entity.
    entity_abuse_recalculate($entity_abuse->entity_id, $entity_abuse->entity_type);

  } catch(PDOException $e) {

    // Remove changes that could be done during previous actions.
    $transaction->rollback();

    // Set message if something went wrong.
    watchdog('entity_abuse', $e->getMessage(), array(), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Cancel user's abuse.
 */
function entity_abuse_cancel($entity_id, $entity_type, $all = FALSE, $uid = NULL) {

  $conditions = array(
    'entity_id' => $entity_id,
    'entity_type' => $entity_type,
  );

  if (!$all) {
    $conditions['uid'] = $uid === NULL ? $GLOBALS['user']->uid : $uid;

    if (empty( $conditions['uid'])) {
      $conditions['source'] = ip_address();
    }
  }

  $transaction = db_transaction();
  try {

    // Remove abuse from the database.
    $query = db_delete('entity_abuse');
    foreach($conditions as $key => $value) {
      $query->condition($key, $value);
    }
    $query->execute();

    // Recalculate current amount of abuses.
    entity_abuse_recalculate($entity_id, $entity_type);

  } catch (PDOException $e) {

    // Remove changes that could be done during previous actions.
    $transaction->rollback();

    // Set message if something went wrong.
    watchdog('entity_abuse', $e->getMessage(), array(), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Calculates current amount of abuses for the entity.
 */
function entity_abuse_recalculate($entity_id, $entity_type) {

  // Get total amount of abuses for the entity.
  $abuses = db_select('entity_abuse', 'e')
    ->condition('e.entity_type', $entity_type)
    ->condition('e.entity_id', $entity_id)
    ->countQuery()
    ->execute()
    ->fetchField();

  // If no abuses left - we need to remove entry from the database.
  if (empty($abuses)) {

    db_delete('entity_abuse_cache')
      ->condition('entity_id', $entity_id)
      ->condition('entity_type', $entity_type)
      ->execute();
  }
  // Update current amount of abuses in the cache table.
  else {

    db_merge('entity_abuse_cache')
      ->key(array(
        'entity_type' => $entity_type,
        'entity_id' => $entity_id
      ))
      ->fields(array(
        'abuses' => $abuses,
        'timestamp' => REQUEST_TIME
      ))
      ->execute();
  }
}

/**
 * Delete all abuses by user.
 */
function entity_abuse_cancel_user_abuses($uid) {

  // Get all user abuses.
  $abuses = db_select('entity_abuse', 'e')
    ->fields('e', array('entity_id', 'entity_type'))
    ->condition('e.uid', $uid)
    ->execute()
    ->fetchAllAssoc('entity_id');

  if (!empty($abuses)) {

    // Delete user abuses.
    db_delete('entity_abuse')
      ->condition('uid', $uid)
      ->execute();

    // Recalculate total amount of abuses.
    foreach ($abuses as $abuse) {
      entity_abuse_recalculate($abuse->entity_id, $abuse->entity_type);
    }
  }
}

/**
 * Build "add abuse" or "cancel abuse" link depends on current state.
 */
function entity_abuse_get_abuse_link($entity_id, $entity_type, $renderable = FALSE) {

  drupal_add_library('system', 'drupal.ajax');

  $link = array(
    'attributes' => array('class' => array('use-ajax', 'entity-abuse-' . $entity_type . '-' . $entity_id)),
  );

  // Add token to avoid CSRF.
  $token = drupal_get_token($entity_type . $entity_id);

  $user_has_complain = entity_abuse_user_has_complain($entity_id, $entity_type);
  if ($user_has_complain) {
    $link['title'] = variable_get('entity_abuse_link_cancel_title', ENTITY_ABUSE_LINK_CANCEL_ABUSE_TITLE);
    $link['href'] = 'entity_abuse/ajax/cancel/' . $entity_type . '/' . $entity_id . '/' . $token;
    $link['attributes']['class'][] = 'cancel-abuse';
  }
  else {
    $link['title'] = variable_get('entity_abuse_link_submit_title', ENTITY_ABUSE_LINK_SUBMIT_ABUSE_TITLE);
    $link['href'] = 'entity_abuse/ajax/add/' . $entity_type . '/' . $entity_id . '/' . $token;
    $link['attributes']['class'][] = 'add-abuse';
  }

  // Add nofollow relation to the ajax link.
  $link['attributes']['rel'] = 'nofollow';

  if ($renderable) {
    $link = array(
      '#theme' => 'link',
      '#text' => $link['title'],
      '#path' => $link['href'],
      '#options' => array(
        'attributes' => $link['attributes'],
        'html' => FALSE,
      )
    );
  }

  return $link;
}

/**
 * Checks if current user already complained on the entity.
 */
function entity_abuse_user_has_complain($entity_id, $entity_type, $uid = NULL) {

  if ($uid === NULL) {
    $uid = $GLOBALS['user']->uid;
  }

  $conditions = array(
    'entity_id' => $entity_id,
    'entity_type' => $entity_type,
    'uid' => $uid,
  );

  if (empty($uid)) {
    $conditions['source'] = ip_address();
  }

  $query = db_select('entity_abuse', 'e');

  foreach ($conditions as $key => $value) {
    $query->condition("e.$key", $value);
  }

  $user_has_complain = (int) $query->countQuery()
    ->execute()
    ->fetchField();

  return $user_has_complain ? TRUE : FALSE;
}

/**
 * Returns amount of abuses for an entity.
 */
function entity_abuse_get_abuse_amount($entity_type, $entity_id) {
  return (int) db_select('entity_abuse_cache', 'c')
    ->fields('c', array('abuses'))
    ->condition('c.entity_type', $entity_type)
    ->condition('c.entity_id', $entity_id)
    ->execute()
    ->fetchField();
}
