<?php
/**
 * @file
 * Implement the callback methods for the content_lock resource.
 */

/**
 * Callback for retrieving a lock from a node.
 *
 * @param int $nid
 *   The nid to check for a lock.
 *
 * @return object
 *   Lock object if one exists, FALSE if no lock.
 */
function _services_content_lock_retrieve($nid) {
  // Make sure this node exists.
  if (!_services_content_lock_node_exists($nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $nid)), 404);
  }

  return content_lock_fetch_lock($nid);
}

/**
 * Callback for creating a lock on a node.
 *
 * @param object $data
 *   POST data that represents a new lock.
 *
 * @return string
 *   Success or fail message.
 */
function _services_content_lock_create($data) {
  global $user;

  // Default lock owner to currently authenticated user.
  $uid = $user->uid;

  // Validate the input.
  if (!isset($data['nid'])) {
    // Return 406 Unacceptable.
    return services_error(t('nid is required'), 406);
  }

  $nid = $data['nid'];

  // Make sure this node exists.
  if (!_services_content_lock_node_exists($nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $nid)), 404);
  }

  // Attempt to lock the node.
  $result = content_lock_node($nid, $uid, TRUE);

  if (!$result) {
    // Node is already locked.
    $content_lock = content_lock_fetch_lock($nid);
    $message_args = array(
      '@nid' => $nid,
      '@name' => $content_lock->name,
      '@date' => format_date($content_lock->timestamp, 'short'),
    );

    // Return 409 Conflict.
    return services_error(t('Node @nid is locked by @name since @date', $message_args), 409);
  }

  return t('Lock successfully created on node') . ': ' . $nid;
}

/**
 * Callback for deleting a lock on a node.
 *
 * @param int $nid
 *   The target nid.
 *
 * @return string
 *   Success or fail message.
 */
function _services_content_lock_delete($nid) {
  global $user;

  // Make sure this node exists.
  if (!_services_content_lock_node_exists($nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $nid)), 404);
  }

  // Make sure there is a lock to delete.
  $lock = content_lock_fetch_lock($nid);
  if (!isset($lock) || !isset($lock->nid)) {
    return services_error(t('No lock to delete on node @nid', array('@nid' => $nid)), 406);
  }

  // Make sure the owner of this lock is the same as the user deleting it.
  if ($user->uid != $lock->uid) {
    return services_error(t('Cannot delete the lock on node @nid owned by another user @name', array('@nid' => $nid, '@name' => $lock->name)), 409);
  }

  // Release the lock, only the owner of the lock is allowed to delete it.
  content_lock_release($nid, $user->uid);

  // Verify that the lock was deleted.
  $lock = content_lock_fetch_lock($nid);
  if (isset($lock) && isset($lock->nid)) {
    // Lock still exists on this node so it wasn't deleted.
    return services_error(t('Lock was not be deleted. Reason unknown. Contact an administrator.'), 406);
  }
  return t('Lock successfully deleted on node') . ': ' . $nid;
}

/**
 * Fast function to see if a node exists without doing a full node_load().
 */
function _services_content_lock_node_exists($nid) {
  return (bool) db_select('node', 'n')->condition('n.nid', $nid, '=')->fields('n', array('nid'))->execute()->fetchField();
}
