<?php
/**
 * @file
 * commerce_credits_transaction.api.php
 */


/**
 * hook_commerce_credits_transaction_transfer_access()
 *
 * @param $entity
 *   The entity to tranfer to.
 * @param $entity_type
 *   The entity type of the entity.
 * @param $entity_id
 *   The entity id of the entity.
 * @return string
 *   Return one of the following NODE_ACCESS_ALLOW, NODE_ACCESS_DENY, or
 *   NODE_ACCESS_IGNORE. This follows the same strict interpretation as
 *   hook_node_access(), but is not restricted to nodes.
 *
 * @see hook_node_access().
 */
function hook_commerce_credits_transaction_transfer_access($entity, $entity_type, $entity_id) {
  if (isset($entity->field_blah['und']) && $entity->field_blah['und'][0]['value'] == 'blah') {
    // Always deny access when field_blah is equal to the string "blah".
    return NODE_ACCESS_DENY;
  }

  return NODE_ACCESS_IGNORE;
}
