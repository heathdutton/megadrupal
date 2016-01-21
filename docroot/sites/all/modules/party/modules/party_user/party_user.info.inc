<?php
/**
 * @file
 *   Update party property info.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function party_user_entity_property_info_alter(&$info) {
  $props = &$info['user']['properties'];

  $props['party']['getter callback'] = 'party_user_party_property_get';
}
