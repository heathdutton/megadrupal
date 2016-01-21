<?php

/**
 * @file
 * Hooks implemented on behalf of the Workbench Moderation module to integrate
 * with Entity Xliff.
 */

if (!function_exists('workbench_moderation_entity_xliff_translatable_source_alter')) {

  /**
   * Implements hook_entity_xliff_translatable_source_alter().
   */
  function workbench_moderation_entity_xliff_translatable_source_alter(&$wrapper, $type) {
    if ($type === 'node') {
      // If workbench moderation is enabled for this content type...
      if (workbench_moderation_node_type_moderated($wrapper->getBundle())) {
        // Then wrap the current revision, not the live version.
        $node = workbench_moderation_node_current_load($wrapper->value());
        $wrapper->set($node);
      }
    }
  }

}
