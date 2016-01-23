<?php

/**
 * @file
 * Hooks provided by the List predefined options module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide information about available predefined list options.
 *
 * @return
 *   An associative array whose keys define the machine name for each
 *   predefined list options and whose values contain the list options
 *   descriptions. Each list options description is itself an associative
 *   array, with the following key-value pairs:
 *   - 'label': (required) The human-readable name of the list options.
 *   - 'callback': (required) The name of the function that will return the
 *     list options.
 *
 * @see hook_list_option_info_alter()
 */
function hook_list_option_info() {
  $info['cats'] = array(
    'label' => t('Types of cats'),
    'callback' => 'mymodule_list_option_cats',
  );

  $info['dogs'] = array(
    'label' => t('Types of dogs'),
    'callback' => 'mymodule_list_option_dogs',
  );

  return $info;
}

/**
 * Alter the metadata about available predefined list options.
 *
 * @param array $info
 *   The associative array of predefined list option definitions from
 *   hook_list_option_info().
 *
 * @see hook_list_option_info()
 */
function hook_list_option_info_alter(array &$info) {
  // Change the callback for cat types to kittens.
  $info['cats']['label'] = t('Types of kittens');
  $info['cats']['callback'] = 'mymodule_list_option_kittens';

  // Remove the dog option list because dogs are dumb.
  unset($info['dogs']);
}

/**
 * @} End of "addtogroup hooks".
 */
