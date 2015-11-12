<?php

/**
 * @file
 * Declare variables implemented by flickrcollection module for use by the
 * variable module (if used).
 */


/**
 * Implements hook_variable_info().
 *
 * Provide more info on variable if variable.module is used.
 */
function flickrcollection_variable_info($options) {
  $variable = array();

  $variable['flickrcollection_collection_id'] = array(
    'title' => t('Flickr Collection ID'),
    'description' => t('Fill in the Flickr Collection ID.'),
    'access' => 'administer flickr settings',
  );

  return $variable;
}


/**
 * Implements hook_variable_update().
 *
 * If variable.module is used ensure we update the sets from the collection.
 */
function flickrcollection_variable_update($name, $value, $old_value, $options) {
  if ($name == 'flickrcollection_collection_id') {
    flickrcollection_get_sets_from_collection();
  }
}
