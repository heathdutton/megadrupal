<?php
/**
 * @file
 * Hook documentation for the Project Repository module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide repository backends for interacting with specialized repositories.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 *
 * @return array
 *   An array of repository types, keyed by machine name (suitable as entity
 *   bundle name). Information for each type is an array containing:
 *   - label: Translated user-friendly label.
 *   - backend class: Class name for the backend handler of the repository type.
 *     The class must extend PHRepositoryBackend.
 *   - field_config: (optional) Field configurations for field_create_field().
 *   - field_instance: (optional) Field instances for field_create_instance().
 *     Each instance array must contain 'field_name'. Values for 'entity_type'
 *     and 'bundle' are filled in using 'ph_repository' and the backend machine
 *     name, respectively.
 *
 * @see ph_repository_git_ph_repository_info()
 */
function hook_ph_repository_info() {
  return array(
    'custom' => array(
      'label' => t('Custom'),
      'backend class' => 'CustomBackend',
      'field_config' => array(
        'field_name' => 'custom_attr_1',
        'type' => 'text',
      ),
      'field_instance' => array(
        'field_name' => 'custom_attr_1',
      ),
    ),
  );
}

/**
 * @}
 */
