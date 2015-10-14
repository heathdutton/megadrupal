<?php

/**
 * Implements hook_permission().
 *
 * Purpose: Permission to access default entitypage '/es'. Used in hook_menu.
 */
function entityspice_permission() {
  $p['administer entityspice'] = array(
    'title' => 'Administer EntitySpice',
    'description' => 'Do administrative tasks on entities defined by ES',
  );

  return $p;
}

