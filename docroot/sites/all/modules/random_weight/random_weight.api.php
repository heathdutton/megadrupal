<?php
/**
 * @file
 * Defines all available hooks for random_weight.
 */

/**
 * Add your own frequencies.
 *
 * This hook allows you to provide your own frequency. A frequency
 * is simply a key and a call back function that is called to see
 * if the nodes weight should be updated. The callback function
 * should be in the format _module_name_key but this is not enforced.
 *
 * @return array
 *   Array must be in format shown below.
 */
function hook_random_weight_frequency_info() {

  return array(
    'daily' => array(
      'label' => t('Run once daily'),
      'callback' => '_module_name_daily',
    ),
  );
}

/**
 * Alter frequencies added by this or other modules.
 *
 * @param array $frequencies
 *   An array of frequencies which can be manipulated.
 */
function hook_random_weight_frequency_info_alter(&$frequencies) {

}
