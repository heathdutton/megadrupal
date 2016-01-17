<?php
/**
 * @file
 * Repository type implementation.
 */

/**
 * Implements hook_ph_repository_info().
 */
function ph_repository_test_ph_repository_info() {
  return array(
    'test' => array(
      'label' => 'Test',
      'backend class' => 'PHRepositoryTestBackend',
      'field_config' => array(
        array(
          'field_name' => 'repository_key',
          'type' => 'text',
        ),
      ),
      'field_instance' => array(
        array(
          'field_name' => 'repository_key',
        ),
      ),
    ),
  );
}
