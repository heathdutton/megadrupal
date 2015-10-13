<?php
/**
 * @file
 * Administration menu module integration.
 */

/**
 * Implements hook_setup_init() on behalf of admin_menu.module.
 */
function admin_menu_setup_init() {
  if (arg(0) == 'setup') {
    admin_menu_suppress();
  }
}
