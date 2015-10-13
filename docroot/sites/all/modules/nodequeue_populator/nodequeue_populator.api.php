<?php
/**
 * @file
 * Hooks provided by the Nodequeue Populator.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Inform Nodequeue Populator about processors.
 *
 *
 * @return array
 *   An array of processors keyed by the processor name.
 */
function hook_nodequeue_populator_processor_info() {
  $items = array(
    'my_processor_name' => array(
      'title' => t('My title'),
      'description' => t('My description.'),
      'callback' => 'my_processor_callback',
      'settings callback' => 'my_processor_settings_form_callback',
    ),
  );
  return $items;
}
