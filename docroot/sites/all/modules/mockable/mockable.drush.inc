<?php

/**
 * @file
 * Drush integration for mockable
 */

/**
 * Implements hook_drush_command().
 *
 * These are the mockable commands available when you type "drush" on the
 * terminal.
 */
function mockable_drush_command() {
  $items['mockable-set'] = array(
    'description' => dt('Mockable set.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Drupal module.',
    ),
  );
  $items['mockable-set-all'] = array(
    'description' => dt('Mockable set all.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['mockable-unset'] = array(
    'description' => dt('Mockable unset.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Drupal module.',
    ),
  );
  $items['mockable-unset-all'] = array(
    'description' => dt('Mockable unset all.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function mockable_drush_help($section) {
  switch ($section) {
    case 'drush:mockable-set':
      $help = dt('Mockable set.');
      break;

    case 'drush:mockable-unset':
      $help = dt('Mockable unset.');
      break;

    default:
      $help = NULL;
  }
  return $help;
}

/**
 * Run 'mockable-set'.
 */
function drush_mockable_set() {
  $args = func_get_args();
  if (isset($args[0])) {
    mockable_set($args[0]);
  }
  else {
    mockable_set();
  }
}

/**
 * Run 'mockable-unset'.
 */
function drush_mockable_unset() {
  mockable_unset();
}
