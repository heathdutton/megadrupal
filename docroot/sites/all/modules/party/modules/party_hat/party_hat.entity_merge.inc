<?php
/**
 * @file
 * Integration with the merge module.
 */

/**
 * Implements hook_entity_merge_field_items_alter().
 */
function party_hat_entity_merge_field_items_alter(&$items, $target_entity, $context) {
  if ($context['field']['type'] != 'hat_reference') {
    return;
  }

  $has_hats = array();
  foreach ($items as $delta => $item) {
    if (in_array($item['hat_name'], $has_hats) || empty($item['hat_name'])) {
      unset($items[$delta]);
    }
    else {
      $has_hats[] = $item['hat_name'];
    }
  }
}
