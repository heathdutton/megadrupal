<?php

/**
 * @file
 */

/**
 * Establishes the transition criterion and the callback performing the access
 * check.
 */
function hook_list_transitions_criterion() {
  return array(
    'criteria_name' => array(
      'label' => t('My criteria'),
      'group' => t('My Group'),
      'criteria callback' => 'mymodule_list_transitions_criteria',
      'criteria arguments' => array(),
      'settings callback' => 'mymodule_list_transitions_settings',
      'settings arguments' => array(),
    ),
  );
}
