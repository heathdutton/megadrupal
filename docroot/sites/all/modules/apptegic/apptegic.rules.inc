<?php

/**
 * @file
 * Rules integration for the Apptegic Drupal module.
 */

/**
 * Implements hook_rules_action_info().
 */
function apptegic_rules_action_info() {
  // Action to add a contact to Apptegic.
  $actions['apptegic_rules_track'] = array(
    'label' => t('Modify information being sent over Apptegic'),
    'group' => t('Apptegic'),
    'access callback' => 'apptegic_rules_access',
  );
  // Provide the parameters to Apptegic.
  foreach (apptegic_parameters() as $name => $type) {
    $actions['apptegic_rules_track']['parameter'][$name] = array(
      'type' => $type,
      'label' => $name,
      'allow null' => TRUE,
      'optional' => TRUE,
    );
  }
  return $actions;
}

/**
 * Implements hook_rules_event_info().
 */
function apptegic_rules_event_info() {
  $events['apptegic_track'] = array(
    'label' => t('An action is being tracked with Apptegic'),
    'group' => t('Apptegic'),
  );
  foreach (apptegic_parameters() as $name => $type) {
    $events['apptegic_track']['variables'][$name] = array(
      'type' => $type,
      'label' => $name,
      'description' => t('When tracking an event with Apptegic, send this information for the %name field.', array(
        '%name' => $name,
      )),
    );
  }
  return $events;
}

/**
 * Rules callback; Injects the given parameters into Apptegic.
 */
function apptegic_rules_track() {
  // Match the parameters to what was passed in through the Rules.
  $parameters = apptegic_parameters();
  $args = func_get_args();
  $i = 0;
  foreach ($parameters as $name => $type) {
    if (isset($args[$i])) {
      $parameters[$name] = trim($args[$i]);
    }
    else {
      unset($parameters[$name]);
    }
    $i++;
  }

  // Filter out any empty arguments and .
  $parameters = array_filter($parameters);
  apptegic_add_action($parameters['setAction'], $parameters);
}

/**
 * Rules callback; Determines whether or not the user has access to use the rule.
 */
function apptegic_rules_access() {
  return user_access('administer apptegic');
}
