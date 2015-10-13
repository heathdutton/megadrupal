<?php
/**
 * @file
 * Contains all implementations of Flag module hooks.
 */

/**
 * Implements hook_flag_definitions_alter().
 */
function og_flag_flag_definitions_alter(&$definitions) {
  foreach ($definitions as $key => $flag) {
    if ($flag['handler'] == 'flag_node') {
      $definitions[$key]['handler'] = OG_FLAG_FLAG_NODE_TYPE;
      $definitions[$key]['module'] = 'og_flag';
    }
    if ($flag['handler'] == 'flag_user') {
      $definitions[$key]['handler'] = OG_FLAG_FLAG_USER_TYPE;
      $definitions[$key]['module'] = 'og_flag';
    }
    if ($flag['handler'] == 'flag_comment') {
      $definitions[$key]['handler'] = OG_FLAG_FLAG_COMMENT_TYPE;
      $definitions[$key]['module'] = 'og_flag';
    }
  }
}

/**
 * Implements hook_flag_alter().
 *
 * Modifies the settings of the flag prior to use if they have been
 * overridden in the group.
 */
function og_flag_flag_alter($flag) {
  if (in_array('OgFlagEntityInterface', class_implements($flag))) {
    $flag->OgFlagInit();
  }
}


