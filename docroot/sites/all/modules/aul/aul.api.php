<?php

/**
 * @file
 * Hooks provided by the AUL module.
 */

/**
 * Respond before AUL creation for some operations. Allows to cancel access 
 * granting to node.
 * 
 * @param array $context
 *   Context for grants assignment with next values:
 *   - aul_id: AUL id.
 *   - uid: User id.
 *   - nid: Node id.
 *   - grants: array of permissions to be assigned. Array of grants. can contain
 *     next keys: 
 *     - "view"
 *     - "update"
 *     - "delete"
 *   - module: Module name string. Used for separating grants realms into 
 *     different groups and namespaces.
 *   - op: Operation. Grants will be added for this operation.
 *   - prefix: Optional custom realm prefix.
 * 
 * @return
 *  - AUL_CANCEL_ACTION: Grants assignment will be canceled.
 *  - In any other case grants assignment will be allowed. Even if you return
 *    nothing.
 */
function hook_aul_add($context) {
  $node = node_load($context['nid']);
  if($node->type == 'article') {
    // Cancel grants assignment.
    return AUL_CANCEL_ACTION;
  }
  // In any other case grants assignment will be allowed.
}

/**
 * Respond before AUL deletion for some operations. Allows to cancel access 
 * granting to node.
 * 
 * @param array $context
 *   Context for grants assignment with next values:
 *   - aul_id: AUL id.
 *   - uid: User id.
 *   - nid: Node id.
 *   - grants: array of permissions to be assigned. Array of grants. can contain
 *     next keys: 
 *     - "view"
 *     - "update"
 *     - "delete"
 *   - module: Module name string. Used for separating grants realms into 
 *     different groups and namespaces.
 *   - op: Operation. Grants will be removed for this operation.
 *   - prefix: Optional custom realm prefix.
 * 
 * @return
 *  - AUL_CANCEL_ACTION: Grants assignment action will be canceled.
 *  - In any other case grants assignment will be allowed. Even if you return
 *    nothing.
 */
function hook_aul_remove($context) {
  $node = node_load($context['nid']);
  if($node->type == 'article') {
    // Cancel grants assignment.
    return AUL_CANCEL_ACTION;
  }
  // In any other case grants assignment action will be allowed.
}

/**
 * React after adding grants for a user to a node.
 * 
 * @param int $nid
 *   Node id.
 * @param int $aul_id
 *   AUL id.
 */
function hook_aul_add_node($nid, $aul_id) {
  $node = node_load($nid);
  if($node->type == 'article') {
    // Some logic.
  }
}

/**
 * React before node AUL deletion.
 *
 * @param int $nid
 *   Node id.
 */
function hook_aul_node_pre_delete($nid) {
  $node = node_load($nid);
  if($node->type == 'article') {
    // Some logic.
  }
}

/**
 * React before user AUL deletion.
 *
 * @param int $uid
 *   User id.
 */
function hook_aul_user_pre_delete($uid) {
  if($user = user_load($uid)) {
    // Some logic.
  }
}
