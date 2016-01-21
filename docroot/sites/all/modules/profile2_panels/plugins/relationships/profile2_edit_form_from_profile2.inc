<?php

/**
 * @file
 * Plugin to provide an relationship handler 'edit form' for Profile2.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Profile2 edit form'),
  'keyword' => 'profile2_edit_form',
  'description' => t('Adds profile2 edit form from a profile2 context.'),
  'required context' => new ctools_context_required(t('Profile2'), 'entity:profile2'),
  'context' => 'profile2_panels_edit_form_from_profile2_context',
);

/**
 * Return a new context based on an existing context.
 */
function profile2_panels_edit_form_from_profile2_context($context, $conf) {

  // If the profile is new, we load one. Creation and saving of the
  // new profile2 is handled in the context.
  // If data is empty, it means the profile is loaded by a relation.
  if (empty($context->data)) {
    $uid = $context->placeholder['conf']['id'];
    $profile_type = $context->placeholder['conf']['type'];
    return ctools_context_create('profile2_edit_form', profile2_by_uid_load($uid, $profile_type));
  }

  // If data is not empty, it can either be a fully qualified (saved earlier)
  // profile2 or it's a new one and the form is loaded by argument with
  // profile2_by_uid_load.
  if (isset($context->data->pid)) {
    return ctools_context_create('profile2_edit_form', $context->data);
  }
}
