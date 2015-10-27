<?php

/**
 * @file
 * Contains handler classes for entity operations.
 *
 */

/**
 * Remove a comment from the queue, if it's already published.
 *
 * This is for the case where a comment is queued, but then is published via
 * another UI.
 */
class HumanQueueCommentWorkerOperationUnQueue extends HumanQueueWorkerOperationUnQueue {

  /**
   * Determine whether the action is valid for the given entity.
   */
  function operationAccess($entity_type, $entity, $params = array()) {
    // A comment can only be removed from the queue if it's already published.
    return $entity->status == TRUE;
  }

  /**
   * Determine access to the operation for the user.
   */
  function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    // Don't use the access verb, as that isn't being used to generated
    // permissions for comments.
    return user_access('access comments');
  }

}
