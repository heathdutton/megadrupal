<?php
/**
 * @file
 * Default package implementations.
 */

/**
 * Implements hook_ph_package_stamp_info().
 */
function ph_package_ph_package_stamp_info() {
  return array(
    'default' => array(
      'label' => t('Default'),
      'factory class' => 'PHPackageDefaultStampFactory',
    ),
  );
}
