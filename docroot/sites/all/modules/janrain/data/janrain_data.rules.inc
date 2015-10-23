<?php
/**
 * @file
 * Janrain Data submodule rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function janrain_data_rules_event_info() {
  return array(
    'janrain_data_profile_updated' => array(
      'label' => t('Janrain customer information updated'),
      'group' => t('Janrain'),
      'variables' => array(
        'profile' => array(
          'label' => 'Janrain profile',
          'type' => 'value',
          'skip save' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function janrain_data_rules_action_info() {
  return array(
    'janrain_data_map' => array(
      'label' => t('Store Janrain customer information'),
      'group' => t('Janrain'),
      'parameter' => array(
        'jsonpath' => array(
          'type' => 'text',
          'label' => t('JsonPath to Profile'),
          'restriction' => 'input',
        ),
        'data' => array(
          'type' => '*',
          'label' => t('Destination of Data'),
          'restriction' => 'selector',
          'wrapped' => TRUE,
          'allow null' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Performs the data mapping from Profile to Drupal.
 *
 * Heavily copied from rules_action_data_set() but extracts data values from the
 * Janrain profile event.
 */
function janrain_data_map($jsonpath, $wrapper, $settings, $state, $element, $execute) {
  // Extract the profile object from the rules event.
  $profile = $state->variables['profile']->raw();

  // Execute the jsonpath.
  $value = $profile->getFirst($jsonpath);

  // Fix dates.
  if ($value && is_string($value)) {
    $timestamp = strtotime($value);
    if ($timestamp !== FALSE) {
      $value = $timestamp;
    }
  }

  // Start copy-paste from rules_action_data_set()
  if ($wrapper instanceof EntityMetadataWrapper) {
    try {
      $wrapper->set($value);
    }
    catch (EntityMetadataWrapperException $e) {
      throw new RulesEvaluationException('Unable to modify data "@selector": ' . $e->getMessage(), array('@selector' => $settings['data:select']));
    }
    if (strpos($element->settings['data:select'], ':') !== FALSE) {
      $info = $wrapper->info();
      $state->saveChanges(implode(':', explode(':', $settings['data:select'], -1)), $info['parent']);
    }
  }
  else {
    return array('data' => $value);
  }
  // End copy-paste from rules_action_data_set()
}
