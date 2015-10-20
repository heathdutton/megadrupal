<?php

/**
 * Alter the list of entity properties that are available for merging.
 *
 * @param array $list
 *   A list of allowable entity properties to be merged.
 *   @see _simpleentitymerge_allowed_entity_properties().
 */
function hook_simpleentitymerge_entity_properties_alter(&$list) {
  unset($list['node']['title']);
  $list['video'] = array(
    'title',
    'created',
  );
}
