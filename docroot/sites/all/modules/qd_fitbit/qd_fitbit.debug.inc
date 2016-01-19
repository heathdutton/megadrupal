<?php
/**
 * @file
 * Debugging functions.
 */


/**
 * Implements hook_qd_debug_form().
 */
function qd_fitbit_qd_debug_form() {
  $form = array();

  $form['qd_fitbit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fitbit'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['qd_fitbit']['load_user_profile'] = array(
    '#type' => 'submit',
    '#value' => 'Load user profile',
    '#submit' => array('qd_fitbit_debug_load_user_profile'),
  );

  $form['qd_fitbit']['run_data_load'] = array(
    '#type' => 'submit',
    '#value' => 'Run data load',
    '#submit' => array('qd_fitbit_debug_run_data_load'),
  );

  return $form;
}


/**
 * Debug call to force load the user profile from Fitbit.
 */
function qd_fitbit_debug_load_user_profile() {
  // Get the profile from Fitbit.
  global $user;
  $client = qd_fitbit_get_access_client($user->uid);
  $fitbit_profile = qd_fitbit_get_user_info($client);

  // Save the profile to the Drupal user data.
  $user->data['qd_fitbit_profile'] = $fitbit_profile;
  $edit = array(
    'data' => array(
      'qd_fitbit_profile' => $fitbit_profile,
    ),
  );
  user_save($user, $edit);

  // Display what was retrieved.
}


/**
 * Debug call to force load all the data calls.
 */
function qd_fitbit_debug_run_data_load() {
  global $user;

  $activity_levels = qd_fitbit_retrieve_data($user, QD_FITBIT_MINSEDENTARY_ENDPOINT, 'activities-minutesSedentary', 'qd_core_activitylevel');
  qd_fitbit_save_entities($activity_levels, 'qd_core_activitylevel');
  dpm($activity_levels);

  $fats = qd_fitbit_retrieve_data($user, QD_FITBIT_BODYFAT_ENDPOINT, 'fat', 'qd_core_bodyfat');
  qd_fitbit_save_entities($fats, 'qd_core_bodyfat');
  dpm($fats);

  $distances = qd_fitbit_retrieve_data($user, QD_FITBIT_DISTANCE_ENDPOINT, 'activities-distance', 'qd_core_distance');
  qd_fitbit_save_entities($distances, 'qd_core_distance');
  dpm($distances);

  $elevations = qd_fitbit_retrieve_data($user, QD_FITBIT_ELEVATION_ENDPOINT, 'activities-elevation', 'qd_core_elevation');
  qd_fitbit_save_entities($elevations, 'qd_core_elevation');
  dpm($elevations);

  $weights = qd_fitbit_retrieve_data($user, QD_FITBIT_WEIGHT_ENDPOINT, 'weight', 'qd_core_weight');
  qd_fitbit_save_entities($weights, 'qd_core_weight');
  dpm($weights);

  $steps = qd_fitbit_retrieve_data($user, QD_FITBIT_STEP_ENDPOINT, 'activities-steps', 'qd_core_steps');
  qd_fitbit_save_entities($steps, 'qd_core_step');
  dpm($steps);
}


/**
 * Load the data from a local file instead of live call.
 *
 * @param string $url
 *   The complete URL being bypassed.
 * @param string $endpoint
 *   The endpoint portion of the URL.
 *
 * @return object
 *   The JSON object representing the response.
 */
function qd_fitbit_debug_cachedcall($url, $endpoint) {
  $response = '';
  $filename = '';

  switch ($endpoint) {
    case QD_FITBIT_USERPROFILE:
      $filename = 'profile.json';
      break;

    case QD_FITBIT_BODYFAT_ENDPOINT:
      $filename = 'bodyfat.json';
      break;

    case QD_FITBIT_DISTANCE_ENDPOINT:
      $filename = 'distance.json';
      break;

    case QD_FITBIT_ELEVATION_ENDPOINT:
      $filename = 'elevation.json';
      break;

    case QD_FITBIT_MINSEDENTARY_ENDPOINT:
      $filename = 'minutesSedentary.json';
      break;

    case QD_FITBIT_MINLIGHTLYACTIVE_ENDPOINT:
      $filename = 'minutesLightlyActive.json';
      break;

    case QD_FITBIT_MINFAIRLYACTIVE_ENDPOINT:
      $filename = 'minutesFairlyActive.json';
      break;

    case QD_FITBIT_MINVERYACTIVE_ENDPOINT:
      $filename = 'minutesVeryActive.json';
      break;

    case QD_FITBIT_STEP_ENDPOINT:
      $filename = 'steps.json';
      break;

    case QD_FITBIT_WEIGHT_ENDPOINT:
      $filename = 'weight.json';
      break;
  }

  $file = drupal_get_path('module', 'qd_fitbit') . '/sample_data//' . $filename;

  $response = file_get_contents($file);
  $response = json_decode(($response));

  return $response;
}
