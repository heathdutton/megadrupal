<?php

/**
 * @file
 * Hooks provided by the Panopoly Config module.
 */

/**
 * @defgroup panopoly_config_hooks Panopoly Config Hooks
 * @{
 * Functions to define and modify config for panopoly config.
 * @}
 */

/**
 * Define config in a module.
 *
 * Provides meta-information for each config group and config variables.
 *
 * Though not required we can also provide some more information to be able to handle the variable in an effective
 * way, like which type of data and form element it uses, default value, etc.. There are multiple predefined
 * variable types ('type' attribute) that will add a predefined set of properties. Some of them are:
 *
 * - "string": Simple plain string variable. The form element will be a text field and it will be localizable.
 * - "number": Simple numeric value. The form element will be a text field.
 * - "boolean": Simple TRUE/FALSE value. It will be a checkbox.
 * - "enable": Enabled / Disabled selector. It will display as two radio buttons.
 * - "select": Selectable list of options. Depending on the number of options, the element will be a list of
 *   radios or a drop down.
 * - "options": List of options with multiple choices. It will be a list of checkboxes.
 * ...
 *
 * More variable types can be defined by modules using hook_variable_type_info().
 *
 * Notes:
 * - Naming convention for config group: PROFILE_NAME_config_group_GROUP_NAME.
 *   A config group with name site_information will create a config group with the name PROFILE_NAME_config_group_site_information.
 *
 * - Naming convention for config varaible: PROFILE_NAME_config_CONFIG_NAME.
 *   A config with name site_name will create a config variable with the name PROFILE_NAME_config_group_site_name.
 */
function hook_panopoly_config_info() {
  $info = array();

  $info['site_information'] = array(
    'title' => t('Site information'),
    'description' => t('General site configuration.'),
    'weight' => 10,
    'config' => array(
      'site_name' => array(
        'type' => 'string',
        'title' => t('Site name'),
        'default' => 'Drupal',
        'description' => t('The name of this website.'),
        'required' => TRUE,
        'show_on_install' => TRUE,
      ),
      'site_slogan' => array(
        'type' => 'string',
        'title' => t('Site slogan'),
        'default' => 'Lipsum dolor sit amet',
        'description' => t('The slogan of this website.'),
        'required' => TRUE,
      ),
    ),
  );

  $info['contact'] = array(
    'title' => t('Contact'),
    'description' => t('Contact information.'),
    'weight' => 20,
    'config' => array(
      'phone' => array(
        'type' => 'string',
        'title' => t('Phone'),
        'required' => TRUE,
        'show_on_install' => TRUE,
      ),
      'address' => array(
        'type' => 'text',
        'title' => t('Address'),
        'required' => TRUE,
        'show_on_install' => TRUE,
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_panopoly_config_info_alter().
 */
function hook_panopoly_config_info_alter(&$info) {
  // Make changes to $info here.
}
