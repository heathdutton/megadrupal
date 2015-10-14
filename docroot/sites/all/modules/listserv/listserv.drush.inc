<?php

/**
 * @file
 * Custom drush commands for the Listserv module.
 */

/**
 * Implements hook_drush_command().
 */
function listserv_drush_command() {
  $items = array();
  $items['subscription'] = array(
    'description' => 'Add or change a user from a Liserv list.',
    'drupal dependencies' => array('listserv'),
    'aliases' => array('lsvs'),
    'arguments' => array(
      'email' => 'Email of the user.',
    ),
    'options' => array(
      'operation' => 'The action you would like to use on the users subscription. example("subscribe")',
      'username' => 'The name of the user subscribing to the list. This is only for subscribing.',
    ),
    'examples' => array(
      'drush lsvs name@test.com --operation=subscribe --username="Michael Potter"' => 'Subscribe a user to a listserv.',
    ),
  );

  return $items;
}

/**
 * Listserv Subscription command.
 */
function drush_listserv_subscription($email) {
  $operation = (drush_get_option('operation') ? drush_get_option('operation') : LISTSERV_DEFAULT_OPERATION);
  $name = (drush_get_option('username') ? drush_get_option('username') : LISTSERV_DEFAULT_USERNAME);

  listserv_listserv_subscription($email, $operation, $name);
}
