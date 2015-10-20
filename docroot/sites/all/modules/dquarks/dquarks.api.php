<?php

/**
 * @file
 * Sample hooks demonstrating usage in DQuarks.
 */

/**
 * @defgroup dquarks_hooks DQuarks Module Hooks
 * @{
 * DQuarks's hooks enable other modules to intercept events within DQuarks, such
 * as the completion of a submission or adding validation. DQuarks's hooks also
 * allow other modules to provide additional components for use within forms.
 */


/**
 * Define components to DQuarks.
 *
 * @return array
 *   An array of components, keyed by machine name. Required properties are
 *   "label" and "description". The "features" array defines which capabilities
 *   the component has, such as being displayed in e-mails.
 *   The possible features of a component include:
 *
 *     - email
 *     - email_address
 *     - email_name
 *     - required
 *     - conditional
 *     - spam_analysis
 *     - group
 *
 *   Note that most of these features do not indicate the default state, but
 *   determine if the component can have this property at all. Setting
 *   "required" to TRUE does not mean that a component's fields will always be
 *   required, but instead give the option to the administrator to choose the
 *   requiredness. See the example implementation for details on how these
 *   features may be set.
 *
 *   _dquarks_[callback]_[component]
 *
 *   Where [component] is the name of the key of the component and [callback] is
 *   any of the following:
 *
 *     - defaults
 *     - edit
 *     - render
 *     - display
 *     - submit
 *     - delete
 *     - help
 *     - theme
 *     - analysis
 *     - table
 *
 * See the sample component implementation for details on each one of these
 * callbacks.
 *
 * @see dquarks_components()
 */
function hook_dquarks_component_info() {
  $components = array();

  $components['textfield'] = array(
    'label' => t('Textfield'),
    'description' => t('Basic textfield type.'),
    'features' => array(

      // This component supports default values. Defaults to TRUE.
      'default_value' => FALSE,

      // This component supports a description field. Defaults to TRUE.
      'description' => FALSE,

      // Show this component in e-mailed submissions. Defaults to TRUE.
      'email' => TRUE,

      // Allow this component to be used as an e-mail FROM or TO address.
      // Defaults to FALSE.
      'email_address' => FALSE,

      // Allow this component to be used as an e-mail SUBJECT or FROM name.
      // Defaults to FALSE.
      'email_name' => TRUE,

      // This component may be toggled as required or not. Defaults to TRUE.
      'required' => TRUE,

      // This component supports a title attribute. Defaults to TRUE.
      'title' => FALSE,

      // This component has a title that can be toggled as displayed or not.
      'title_display' => TRUE,

      // This component has a title that can be displayed inline.
      'title_inline' => TRUE,

      // If this component can be used as a conditional SOURCE. All components
      // may always be displayed conditionally, regardless of this setting.
      // Defaults to TRUE.
      'conditional' => TRUE,
    ),
    'file' => 'components/textfield.inc',
  );

  return $components;
}

/**
 * Implements hook_DquarksExporters().
 *
 * Defines the exporters this module implements.
 */
function hook_dquarks_DquarksExporters() {
  return array(
    'excel' => array(
      'title' => t('Microsoft Excel'),
      'description' => t('A file readable by Microsoft Excel.'),
      'handler' => 'DquarksExporterExcel',
    ),
  );
}
/**
 * @}
 */
