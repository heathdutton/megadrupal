<?php

/**
 * @file
 * Hooks provided by the PMPAPI pull module.
 */

/**
 * Act on test to determine if file-based profile should be copied locally.
 *
 * @param string $profile
 *   The name of a PMP profile
 */
function hook_pmpapi_pull_make_local_files($profile) {
  $remote_profiles = array('audio', 'video');
  if (in_array($profile, $remote_profiles)) {
    // NB: If you want to make files remote, MAKE SURE that you return an
    // explicit FALSE, (rather than just falling out of the function)
    return FALSE;
  }
  // The default return will likely be TRUE
  return TRUE;
}

/**
 * Alter pulled docs before they are saved as entities.
 *
 * @param array $docs
 *   A list of docs that have been pulled, but not saved as entities.
 */
function hook_pmpapi_pull_docs_prepare_alter(&$docs) {
  foreach($docs as $i => $doc) {
    if (!empty($doc->pmpapi_do_not_pull)) {
      unset($docs[$i]);
    }
  }
}

/**
 * Alter default field values before they are attached to an entity and saved.
 *
 * @param array $values
 *   A list of value(s) for a given field.
 *
 * @param string $field_name
 *   The name of the field.
 */
function hook_pmpapi_pull_default_values_alter($values, $field_name) {
  if ($field_name == 'slug') {
    $values = array(array('value' => t('Urgent')));
  }
}