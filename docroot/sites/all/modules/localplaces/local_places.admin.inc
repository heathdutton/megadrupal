<?php
/**
 * @file
 * Administrative page callbacks for the local_places module.
 */

/**
 * The general settings form code stored in the $form variable.
 */
function local_places_settings_form($form, &$form_state) {
  $form['local_places_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for your Local Places Home Page'),
    '#default_value' => variable_get('local_places_title', 'Local Places'),
    '#size' => 50,
    '#maxlength' => 150,
    '#required' => TRUE,
  );
  $form['local_places_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Description for your Local Places Home/Results Page'),
    '#default_value' => variable_get('local_places_desc', ''),
    '#description' => t('Description that appears above your results page listings'),
    '#format' => NULL,
    '#required' => TRUE,
  );
  $form['local_places_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Places Base URL'),
    '#default_value' => variable_get('local_places_url', 'https://maps.googleapis.com/maps/api/place/nearbysearch/xml?sensor=false'),
    '#size' => 150,
    '#maxlength' => 255,
    '#description' => t('The base URL for Google Places API searches. At time of writing, it is https://maps.googleapis.com/maps/api/place/nearbysearch/xml?sensor=false. You should only change this if the base URL has changed since this module was written (June 2013).'),
    '#required' => TRUE,
  );
  $form['local_places_mapurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps Base URL'),
    '#default_value' => variable_get('local_places_mapurl', 'http://maps.googleapis.com/maps/api/staticmap?type=roadmap&sensor=false'),
    '#size' => 150,
    '#maxlength' => 255,
    '#description' => t('The base URL for Google Maps API calls. At time of writing, it is http://maps.googleapis.com/maps/api/staticmap?type=roadmap&sensor=false. You should only change this if the base URL has changed since this module was written (June 2013).'),
    '#required' => TRUE,
  );
  $form['local_places_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Places API Key'),
    '#default_value' => variable_get('local_places_key', 'YOU MUST ENTER AN API KEY.'),
    '#size' => 40,
    '#maxlength' => 40,
    '#description' => t('Your <a href="https://developers.google.com/places/documentation/#Authentication" target="_blank">Google Places API Key</a>. This module will not work without it.'),
    '#required' => TRUE,
  );
  $form['local_places_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Location'),
    '#default_value' => variable_get('local_places_location', '51.5073,0.1276'),
    '#size' => 22,
    '#maxlength' => 24,
    '#description' => t('The location you wish to search from, in the format [latitude],[longitude].'),
    '#required' => TRUE,
  );
  $form['local_places_radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius'),
    '#default_value' => variable_get('local_places_radius', 50000),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The radius in meters, from your search start point. Maximum is 50000.'),
    '#required' => TRUE,
  );
  $form['local_places_types'] = array(
    '#type' => 'textfield',
    '#title' => t('Place Types'),
    '#default_value' => variable_get('local_places_types', 'cafe|restaurant'),
    '#size' => 30,
    '#maxlength' => 100,
    '#description' => t('The types of place you wish to search for, separated by a pipe (|). See <a href="https://developers.google.com/places/documentation/supported_types" target="_blank">https://developers.google.com/places/documentation/supported_types</a> for supported types.'),
    '#required' => TRUE,
  );
  $form['local_places_lmapsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Large Map Size'),
    '#default_value' => variable_get('local_places_lmapsize', '600x600'),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('The size of the large map that apears on the results pages. In the format [width]x[height].'),
    '#required' => TRUE,
  );
  $form['local_places_smapsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Small Map Size'),
    '#default_value' => variable_get('local_places_smapsize', '300x200'),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('The size of the small map that apears on the details pages. In the format [width]x[height].'),
    '#required' => TRUE,
  );
  $form['local_places_lmapzoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Large Map Zoom'),
    '#default_value' => variable_get('local_places_lmapzoom', 15),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('The zoom level of the large map that apears on the results pages. 1 is furthest zoomed out and 22 closest possible zoom in.'),
    '#required' => TRUE,
  );
  $form['local_places_smapzoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Small Map Zoom'),
    '#default_value' => variable_get('local_places_smapzoom', 16),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('The zoom level of the small map that apears on the details pages. 1 is furthest zoomed out and 22 closest possible zoom in.'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
