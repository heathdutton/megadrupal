<?php
/**
 * @file
 * Hooks provided by the Packing API of Ubercart Aus Post PAC Shipping module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide a Packing Algorithm via the Packing API.
 *
 * Module wishing to implement their own packing algorithm must implement this
 * hook. It defines the user visible name, callback that does the work, and an
 * optional form if it requries additional configuration.
 *
 * @return
 *   An array whoes keys are algorithm machine names, and values are strings
 *   or arrays containing the following keys.
 *   - title: The translated human readable name of the packing algorithm.
 *   - description: (optional) A human readable, translated description of the
 *     algorithm that will be shown in the packing method choice admin form.
 *   - callback: Function to call to pack the contents of the cart.
 *   - file: (optional) If the callback function is not in the mail .module
 *     file, the file must be specified here. It may include path elements and
 *     will be relative to the file path key.
 *   - file path: (optional) Defaults to the module directory if not given.
 *   - config: (optional) A form id, passed to drupal_get_form along with the
 *     quote method and packing method, used to configure this module's
 *     settings.
 *   - config file: (optional) If the callback function of the configuration
 *     settings form is not in the main .module file, the file must be set
 *     here. It is relative to the file path key.
 */
function hook_packing_info() {
  $info = array();

  $info['volume_weight'] = array(
    'title' => t('Volume and Weight'),
    'description' => t('Pack optimal boxes up to volume and weight limits.'),

    'callback' => 'pack_volume_weight_pack',
    'file' => 'pack_volume_weight.packing.inc',

    'config' => 'pack_volume_weight_config',
    'config file' => 'pack_volume_weight.admin.inc',
  );

  return $info;
}

/**
 * Alter the result returned by all modules implementing hook_packing_info().
 *
 * @see hook_packing_info().
 */
function hook_packing_info_alter(&$info) {

  // Alter the title of the Volume and Weight packing algorithm.
  $info['volume_weight']['title'] = t('Weight');
}

/**
 * @} End of "addtogroup hooks".
 */
