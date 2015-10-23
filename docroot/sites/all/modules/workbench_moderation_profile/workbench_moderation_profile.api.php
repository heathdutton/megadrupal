<?php

/**
 * @file
 * Contains documentation about the Workbench Moderation Profile module's hooks.
 */

/**
 * @defgroup workbench_moderation_profile Workbench Moderation Profile
 * @{
 * Hooks from the Workbench Moderation Profile module.
 */

/**
 * Get the appropriate profile for the given node.
 *
 * This hook should be declared by a module which provides a menchanism for
 * deciding which profile is appropriate for a given node. There are two such
 * modules included: workbench_moderation_profile_node and
 * workbench_moderation_profile_og.
 *
 * @param object $node
 *   A Drupal node object.
 *
 * @return integer|NULL
 *   Returns the wmpid for the profile to be used; or NULL if this module
 *   doesn't know which profile to use.
 *
 * @see workbench_moderation_profile_workbench_moderation_states_next_alter()
 */
function hook_workbench_moderation_profile_get_profile($node) {
  // Store which profile is connected with each node type in a variable,
  // returning NULL if none is configured.
  $profile = variable_get('mymodule_node_' . $node->type, NULL);
  return $profile;
}

/**
 * Get the appropriate profile for the given node.
 *
 * This hook should be declared by a module which provides a menchanism for
 * deciding which profile is appropriate for a given node. There are two such
 * modules included: workbench_moderation_profile_node and
 * workbench_moderation_profile_og.
 *
 * @param string $op
 *   The operation being performed. One of 'view', 'update', 'create', 'delete'
 *   or just 'edit' (being the same as 'create' or 'update').
 * @param object|NULL $profile
 *   A profile object to check access for. If NULL, you should check access for
 *   all profiles.
 * @param object|NULL $account
 *   The user to check for. If NULL if you should check for the global user.
 *
 * @return boolean|NULL
 *   Returns TRUE to allow access, FALSE to deny or NULL if this module doesn't
 *   want to affect access for this profile.
 *
 * @see workbench_moderation_profile_access()
 */
function hook_workbench_moderation_profile_access($op, $profile, $account) {
  // This example puts a new boolean field on profiles and uses it to determine
  // if a profile is private. It will check if the user has a special
  // permission to access private profiles.
  if ($profile) {
    return !$profile->field_wmp_private[LANGUAGE_NONE]['0']['value'] ||
      user_access('view private workbench_moderation_profile entities', $account);
  }
}

/**
 * @}
 */
