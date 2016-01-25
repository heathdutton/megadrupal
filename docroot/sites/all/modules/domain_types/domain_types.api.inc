<?php

/**
 * @file
 * Hooks created by Domain Types.
 */

/**
 * Implements hook_domain_types().
 */
function hook_domain_types() {
  return array(
    // Override domain type "default".
    'default' => array('name' => t('None')),

    // Add new domain types.
    'foo' => array('name' => t('Foo')),
    'bar' => array('name' => t('Bar')),
  );
}

/**
 * Implements hook_domain_types_alter().
 */
function hook_domain_types_alter(&$types) {
  $types['default']['name'] = t('This domain type name is mine!');
}
