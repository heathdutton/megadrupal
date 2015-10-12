<?php
/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Implements hook_feeds_node_processor_targets_alter().
 */
function termstatus_feeds_processor_targets_alter(&$targets, $entity_type, $bundle) {
  if ($entity_type === 'taxonomy_term') {
    $targets['status'] = array(
      'name' => t('Published status'),
      'description' => t('Whether a term is published or not. 1 stands for published, 0 for not published.'),
    );
  }
}
