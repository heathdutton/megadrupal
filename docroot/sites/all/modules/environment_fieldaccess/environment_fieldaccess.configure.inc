<?php
/**
 * @file
 * Configure access arguments.
 */

/**
 * Configure access arguments.
 *
 * @see callback_fieldaccess_configure()
 */
function environment_fieldaccess_configure(array $settings, array &$form, array &$form_state) {
  return _environment_fieldaccess_settings($settings);
}
