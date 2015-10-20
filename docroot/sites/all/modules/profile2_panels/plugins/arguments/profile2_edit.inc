<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for a Profile2 edit form
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Profile2 edit form"),
  // keyword to use for %substitution
  'keyword' => 'profile2',
  'description' => t('Creates a profile2 edit form context from a profile2 PID argument.'),
  'context' => 'profile2_panels_profile2_edit_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the profile ID of a profile for this argument'),
  ),
);

/**
 * Discover if this argument gives us the profile we crave.
 */
function profile2_panels_profile2_edit_context($arg = NULL, $conf = NULL, $empty = FALSE) {

  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('profile2_edit_form');
  }

  // We can accept either a profile object or a pure pid.
  if (is_object($arg)) {
    return ctools_context_create('profile2_edit_form', $arg);
  }

  if (!is_numeric($arg)) {
    return FALSE;
  }

  $profile = profile2_load($arg);
  if (!$profile) {
    return NULL;
  }

  // This will perform a access check, so we don't have to.
  return ctools_context_create('profile2_edit_form', $profile);
}

