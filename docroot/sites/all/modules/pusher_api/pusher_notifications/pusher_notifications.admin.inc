<?php

/**
 * @file
 * Admin page callback file for the pusher notifications module.
 */

/**
 * Form builder; Configure pusher notification settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function pusher_notifications_admin_settings() {
  $form = array();

  return system_settings_form($form);
}