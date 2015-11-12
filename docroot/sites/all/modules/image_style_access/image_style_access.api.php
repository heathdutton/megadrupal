<?php
/**
 * @file
 * Hooks related to access control for image styles.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define access control type plugins.
 *
 * Each plugin implements a behavior for controlling user access. A plugin may
 * provide a form for configuring the access parameter settings.
 *
 * @return array
 *   An array of plugin definitions keyed by plugin name, each containing:
 *   - label: User-friendly title of the access control type.
 *   - description: Optional text describing the access control behavior.
 *   - factory class: Factory for creating the access control. This
 *     class must implement ImageStyleAccessControlFactoryInterface.
 *   - form controller class: Form controller class for the access control. If
 *     not given, this class defaults to ImageStyleAccessConfigFormController.
 */
function hook_image_style_access_type_info() {
  return array(
    'my_access' => array(
      'label' => t('My access control'),
      'factory class' => 'MyAccessControlFactory',
      'form controller class' => 'MyAccessFormController',
    ),
  );
}

/**
 * Alter access control plugin definitions.
 *
 * @param array &$type_info
 *   Plugin definition to alter.
 */
function hook_image_style_access_type_info_alter(&$type_info) {
}

/**
 * @} End of "addtogroup hooks".
 */
