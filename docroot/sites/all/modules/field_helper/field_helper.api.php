<?php
/**
 * @file
 * Document field_helper hooks.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Define field to be installed.
 *
 * @return
 *   Associative array of fields keyed by field name.
 * @see field_create_field()
 */
function hook_install_fields() {
  return array(
    'some_field' => array(
      'field_name' => 'some_field',
      'type' => 'text_long',
    ),
  );
}

/**
 * Define field instances to be installed.
 *
 * @return
 *   Associative array of field instances with unique keys.
 * @see field_create_instance()
 */
function hook_install_instances() {
  $t = get_t();
  return array(
    'some_field' => array(
      'entity_type' => 'node',
      'bundle' => 'some_node_type',
      'field_name' => 'some_field',
      'label' => $t('Some field'),
      'description' => $t('Some field description.'),
      'widget' => array(
        'type' => 'text_textarea',
      ),
    ),
  );
}
