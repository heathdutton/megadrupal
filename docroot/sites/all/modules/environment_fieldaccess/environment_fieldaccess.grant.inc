<?php
/**
 * @file
 * Check an access.
 */

/**
 * Check an access.
 *
 * @see callback_fieldaccess_grant()
 */
function environment_fieldaccess_grant(array $settings, \stdClass $object, \stdClass $account, array $context) {
  $settings = _environment_fieldaccess_settings($settings, TRUE);
  $state = array_key_exists(environment_current(), $settings['environment']);

  return $settings['not'] ? !$state : $state;
}
