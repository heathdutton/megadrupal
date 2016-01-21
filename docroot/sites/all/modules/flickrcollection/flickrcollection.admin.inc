<?php

/**
 * @file
 * Administration page callbacks.
 */


/**
 * Administration settings form.
 */
function flickrcollection_collection_form() {
  $form = array();

  drupal_set_title(t('FlickrCollection Settings'));

  $form['flickrcollection_collection_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr Collection ID'),
    '#description' => t('Fill in the Flickr Collection ID.'),
    '#default_value' => variable_get('flickrcollection_collection_id'),
  );

  $form['#submit'][] = 'flickrcollection_collection_form_submit';

  return system_settings_form($form);
}


/**
 * Custom submit function for the settings form.
 *
 * When the settings form is submitted update the list of sets in the
 * collection.
 *
 * @see flickrcollection_get_sets_from_collection()
 */
function flickrcollection_collection_form_submit($form, &$form_state) {
  if ($form_state['values']['flickrcollection_collection_id'] != $form['flickrcollection_collection_id']['#default_value']) {
    variable_set('flickrcollection_collection_id', $form_state['values']['flickrcollection_collection_id']);
    flickrcollection_get_sets_from_collection();
  }
}
