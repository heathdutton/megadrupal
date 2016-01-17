<?php

/**
 * @file
 * Hooks provided by Virtual Field.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Use the hook_field_info() hook to declare a field virtual.
 *
 * The virtual_field key of the field definition specifies that this field is
 * virtual. Further options may be specified by making
 *
 * A field is declared virtual by adding an array of options in the
 * 'virtual_field' key of the field definition. Currently supported options:
 * - entity_types: The type of entities supported. If not specified, the field
 *   is allowed on all types of entities.
 *
 * @see hook_field_info()
 */
function virtual_field_hook_field_info() {
  return array(
    'myvirtual' => array(
      'label' => t('Virtual thing'),
      'description' => t('Something not quite here.'),
      'default_widget' => 'hidden',
      'default_formatter' => 'myvirtual_default',
      'virtual_field' => array(
        'entity_types' => array('node'),
      ),
    ),
  );
}

/**
 * @} End of "addtogroup hooks".
 */
