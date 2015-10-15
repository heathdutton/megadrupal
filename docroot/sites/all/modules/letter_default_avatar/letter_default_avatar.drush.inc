<?php
/**
 * @file
 * Drush commands for the letter_default_avatar module!
 */

/**
 * Implements hook_drush_command().
 */
function letter_default_avatar_drush_command() {
  $items['letter-default-avatar-generate'] = array(
    'description' => dt("Generate default avatars for all users who don't have one."),
    'aliases' => array('lda-generate'),
  );

  return $items;
}

/**
 * Drush command callback.
 */
function drush_letter_default_avatar_generate() {

}
