<?php
/**
 * @file
 * comstack_friends.features.user_relationship.inc
 */

/**
 * Implements hook_user_relationships_default_relationships().
 */
function comstack_friends_user_relationships_default_relationships() {
  $relationships = array();

  // Exported user_relationship style: comstack_blocked.
  $relationships['comstack_blocked'] = array(
    'name' => 'Blocked',
    'plural_name' => 'blocked',
    'is_oneway' => 1,
    'is_reciprocal' => 0,
    'requires_approval' => 0,
    'expires_val' => 0,
    'reverse_name' => 'blocked',
    'name_capitalized' => 'Blocked',
    'plural_name_capitalized' => 'Blocked',
    'reverse_name_capitalized' => 'Blocked',
    'reverse_plural_name' => 'blocked',
    'reverse_plural_name_capitalized' => 'Blocked',
    'machine_name' => 'comstack_blocked',
  );

  // Exported user_relationship style: comstack_friends.
  $relationships['comstack_friends'] = array(
    'name' => 'Friend',
    'plural_name' => 'friends',
    'is_oneway' => 0,
    'is_reciprocal' => 0,
    'requires_approval' => 1,
    'expires_val' => 0,
    'reverse_name' => '',
    'name_capitalized' => 'Friends',
    'plural_name_capitalized' => 'Friends',
    'reverse_name_capitalized' => '',
    'reverse_plural_name' => '',
    'reverse_plural_name_capitalized' => '',
    'machine_name' => 'comstack_friends',
  );

  return $relationships;
}
