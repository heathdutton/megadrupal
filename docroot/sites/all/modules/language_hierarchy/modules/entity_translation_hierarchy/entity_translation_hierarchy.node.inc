<?php

/**
 * @file
 * Functionality related to nodes.
 */

/**
 * Implements hook_init().
 */
function entity_translation_hierarchy_init() {
  // When adding translation make sure the parent language is the source of target translation.
  if (fnmatch('node/*/edit/add/*/*', current_path()) && $node = menu_get_object()) {
    $target_langcode = arg(5);
    $fallback_candidate = entity_translation_hierarchy_get_candidate($node, 'node', arg(5));

    // Referrer is checked to prevent redirect when user is changing translation source at node form.
    $changed_source_manually = !empty($_SERVER['HTTP_REFERER']) && fnmatch('*node/*/edit/add/*/*', $_SERVER['HTTP_REFERER']);
    if (!empty($fallback_candidate) && $fallback_candidate != arg(4) && !$changed_source_manually) {
      $path = "node/$node->nid/edit/add/$fallback_candidate/$target_langcode";
      drupal_goto($path);
    }
  }
}

/**
 * Implements hook_node_access().
 */
function entity_translation_hierarchy_node_access($node, $op, $account) {
  if ($op == 'view') {
    if (entity_translation_hierarchy_is_blocked($node, 'node', $GLOBALS['language_content']->language)) {
      return NODE_ACCESS_DENY;
    }
  }

  return NODE_ACCESS_IGNORE;
}
