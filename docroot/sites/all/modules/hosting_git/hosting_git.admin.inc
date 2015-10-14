<?php

/**
 * @file
 * Admin UI code for Git support
 */

/**
 * General settings form.
 */
function hosting_git_settings_form() {
  $form = array();

  return system_settings_form($form);
}
