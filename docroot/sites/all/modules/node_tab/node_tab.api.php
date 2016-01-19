<?php

/**
 * @file
 * Hooks provided by the Node Tab module.
 */

/**
 * Control access to a tab.
 *
 * Modules implementing this hook can return FALSE to provide a blanket
 * prevention for the user to perform the requested operation on the specified
 * entity. If no modules implementing this hook return FALSE then the operation
 * will be allowed.
 *
 * Named hook_node_tab_tab_access() instead of hook_node_tab_access() to
 * prevent conflicts with entity_translation and nodequeue which would
 * accidentally implement the hook in that case.
 *
 * When a tab is being shown on a node page, the node is passed as an argument
 * allowing for per-node tab access.
 *
 * @param $tab
 *   A tab entity.
 * @param $op
 *   The operation to be performed. Possible values:
 *   - "create"
 *   - "delete"
 *   - "update"
 *   - "view"
 * @param $account
 *   The user object to perform the access check operation on.
 * @param $node
 *   An optional node entity, if known.
 *
 * @return
 *   FALSE indicating an explicit denial of permission; NULL or TRUE to not
 *   affect the access check at all.
 */
function hook_node_tab_tab_access($tab, $op, $account, $node = NULL) {
  // If there's a node, use the field_tab_displayed entityreference field
  // to determine access.
  if ($node) {
    if (!empty($node->field_tab_displayed)) {
      foreach ($node->field_tab_displayed[LANGUAGE_NONE] as $item) {
        if ($item['target_id'] == $tab->tab_id) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }
}
