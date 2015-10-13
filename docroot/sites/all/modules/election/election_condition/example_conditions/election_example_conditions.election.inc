<?php
/**
 * @file
 * Election hooks for the Election Example Conditions module.
 */

/**
 * Define the example conditions. Implements hook_election_condition_info().
 */
function election_example_conditions_election_condition_info() {
  $conditions = array();
  $conditions['gmail'] = array(
    'name' => t('Have a Gmail account'),
    'callbacks' => array('election_example_conditions_access_gmail'),
    'description' => t('The user\'s registered email address must end with "gmail.com" or "googlemail.com".'),
    'user explanation' => t('Your registered email address must be a GMail account.'),
  );
  $conditions['hotmail'] = array(
    'name' => t('Have a Hotmail account'),
    'callbacks' => array('election_example_conditions_access_hotmail'),
    'description' => t('The user\'s registered email address must end with "hotmail.com" or "hotmail.co.uk".'),
    'user explanation' => t('Your registered email address must be a Hotmail account.'),
  );
  return $conditions;
}
