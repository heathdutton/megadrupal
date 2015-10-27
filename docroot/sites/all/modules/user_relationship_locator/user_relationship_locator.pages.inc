<?php
/**
 * @file
 * Pages callbacks.
 */

/**
 * Ignore suggestion callback.
 *
 * @param int $suggestion
 *   User object.
 */
function user_relationship_locator_ignore_page($suggestion) {
  user_relationship_locator_ignore($GLOBALS['user'], $suggestion);

  drupal_set_message(t('User !user has been ignored!', array(
    '!user' => theme('username', array('account' => $suggestion)),
  )));

  drupal_goto(drupal_get_destination());
}
