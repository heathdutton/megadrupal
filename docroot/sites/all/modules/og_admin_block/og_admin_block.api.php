<?php

/**
 * @file
 * Hooks provided by the OG admin block module.
 */

/**
 * Alter organic groups admin block.
 *
 * @param array $vars
 *   The og admin block render array.
 */
function hook_og_admin_block_alter(array &$vars) {
  // Reorder links.
  $vars['content']['invite_new_member']['#weight'] = 1;
  $vars['content']['manage_members']['#weight'] = 2;
  $vars['content']['edit_group']['#weight'] = 3;
}
