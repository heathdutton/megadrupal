<?php

/**
 * @file
 * API documentation for Commerce ADS module.
 */

/**
 * Alter ADS product configuration admin form's frequency dropdowns' options.
 *
 * @param array $frequency_options
 *   An array of possible synchronization frequencies, where keys are
 *   minimum number of seconds between the synchronizations, and values are
 *   their textual representations displayed as the dropdown value labels.
 */
function hook_commerce_ads_frequency_options_alter(&$frequency_options) {
  $frequency_options['604800'] = t('1 week');
}
