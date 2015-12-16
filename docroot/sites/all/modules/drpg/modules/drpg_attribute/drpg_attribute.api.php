<?php

/**
 * @file
 * Hooks provided by the DRPG Attribute module.
 */

/**
 * Provides an array of names that can be assigned to attribute entities.
 *
 * @return array
 *   Array of attribute names indexed by attribute ID.
 */
function hook_drpg_attribute_names() {
  $attribute_names = array(
    'hp' => 'HP',
    'xp' => 'XP',
  );

  return $attribute_names;
}
