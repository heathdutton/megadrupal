<?php
/**
 * @file
 * Election hooks for the Election Example Conditions module.
 */

/**
 * Implements hook_election_condition_info().
 */
function election_example_conditions_election_condition_info() {
  $conditions = array();
  $conditions['election_example_conditions_gmail'] = array(
    'name' => t('Have a Gmail account'),
    'callback' => 'election_example_conditions_check_mail_domain',
    'data' => array('gmail.com', 'googlemail.com'),
    'description' => t('The user\'s registered email address must end with "gmail.com" or "googlemail.com".'),
    'user explanation' => t('Your registered email address must be a GMail account.'),
  );
  $conditions['election_example_conditions_hotmail'] = array(
    'name' => t('Have a Hotmail account'),
    'callback' => 'election_example_conditions_check_mail_domain',
    'data' => array('hotmail.com', 'hotmail.co.uk'),
    'description' => t('The user\'s registered email address must end with "hotmail.com" or "hotmail.co.uk".'),
    'user explanation' => t('Your registered email address must be a Hotmail account.'),
  );
  return $conditions;
}
