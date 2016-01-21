<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Define available voting conditions.
 *
 * @return array
 *   A structured election condition array, keyed by a unique machine name. Each
 *   election condition array can contain:
 *   - name: (Required) The human-readable, translated name of the condition.
 *   - callback: A callback for testing the condition. If not specified, the
 *     condition machine name will be treated as a callback function. The
 *     callback takes at least two arguments: the election post object and the
 *     user account object. The callback must return TRUE if the user passes the
 *     condition.
 *   - data: (Optional) An optional extra argument to pass to the callback.
 *   - file: (Optional) An optional filename that contains the condition.
 *   - description: A description of the condition for administrators.
 *   - user explanation: An explanation of the condition for users.
 */
function hook_election_condition_info() {
  return array(
    'example_gmail' => array(
      'name' => t('Have a Gmail account'),
      'callback' => 'condition_to_check_email_domain',
      'data' => array('gmail.com'),
      'description' => t('The user\'s registered email address must end with "gmail.com".'),
      'user explanation' => t('You must have a GMail account'),
    ),
  );
}

/**
 * Alter previously defined voting conditions.
 */
function hook_election_condition_info_alter(&$info) {
  // Add an extra email domain to the example_gmail condition.
  $info['example_gmail']['data'][] = 'googlemail.com';
}
