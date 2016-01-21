<?php
/**
 * @file
 * Hook documentation for the Package module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide package stamp handlers.
 *
 * This hook should be implemented in MODULENAME.ph.inc.
 *
 * @return array
 *   Stamp handler info. Each stamp is keyed by its name and contains:
 *   - label: Translated user-friendly label.
 *   - factory class: Stamp factory class. This class must implement
 *       PHPackageStampFactoryInterface and construct without arguments.
 */
function hook_ph_package_stamp_info() {
  return array(
    'custom_stamp' => array(
      'label' => t('Custom stamp'),
      'factory class' => 'CustomPackageStampFactory',
    ),
  );
}

/**
 * @}
 */
