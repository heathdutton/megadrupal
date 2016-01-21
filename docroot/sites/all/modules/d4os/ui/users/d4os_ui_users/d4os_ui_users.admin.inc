<?php

/**
 * @package   d4os_ui_users
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_ui_users_admin() {
  global $base_url;

  // get roles
  $roles = array(
    '0' => t('None'),
  );
  $roles += user_roles(TRUE);

  $form = array();

  $form['d4os_ui_users_autocreate_grid_account'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autocreate grid account'),
    '#default_value' => variable_get('d4os_ui_users_autocreate_grid_account', 0),
    '#description' => t('Auto create a grid account if the user is not registered on the grid and when updating his profile.'),
  );
  /*
   * User level management
   */
  $form['user_level'] = array(
    '#type' => 'fieldset',
    '#title' => t('User level'),
    '#description' => t('That usage is NOT supported by OpenSim devs. Use at your own risk. Side effects unknown. More infos can be found here !link', array('!link' => l('http://opensimulator.org/wiki/Userlevel', 'http://opensimulator.org/wiki/Userlevel'))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['user_level']['d4os_ui_users_default_user_level'] = array(
    '#type' => 'textfield',
    '#title' => t('Default user level'),
    '#default_value' => variable_get('d4os_ui_users_default_user_level', 0),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("The default user level for new users."),
    '#required' => TRUE,
  );

  $form['user_level']['d4os_ui_users_user_level_roles_blocked'] = array(
    '#type' => 'select',
    '#title' => t('Blocked'),
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_blocked', 0),
    '#options' => $roles,
    '#description' => t('-1 : Resident:Login blocked'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_0'] = array(
    '#type' => 'select',
    '#title' => 'GOD_NOT',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_0', 0),
    '#options' => $roles,
    '#description' => t('0 : <strong>Resident:GOD_NOT</strong>'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_1'] = array(
    '#type' => 'select',
    '#title' => 'GOD_LIKE',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_1', 0),
    '#options' => $roles,
    '#description' => t('1 : Resident:GOD_LIKE'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_20'] = array(
    '#type' => 'select',
    '#title' => '20',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_20', 0),
    '#options' => $roles,
    '#description' => t('20 : Resident:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_30'] = array(
    '#type' => 'select',
    '#title' => '30',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_30', 0),
    '#options' => $roles,
    '#description' => t('30 : Testing:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_40'] = array(
    '#type' => 'select',
    '#title' => '40',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_40', 0),
    '#options' => $roles,
    '#description' => t('40 : Testing:No payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_50'] = array(
    '#type' => 'select',
    '#title' => '50',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_50', 0),
    '#options' => $roles,
    '#description' => t('50 : Testing:No payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_60'] = array(
    '#type' => 'select',
    '#title' => '60',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_60', 0),
    '#options' => $roles,
    '#description' => t('60 : Member Estatute:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_70'] = array(
    '#type' => 'select',
    '#title' => '70',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_70', 0),
    '#options' => $roles,
    '#description' => t('70 : Member Estatute:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_80'] = array(
    '#type' => 'select',
    '#title' => '80',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_80', 0),
    '#options' => $roles,
    '#description' => t('80 : Linden Contracted'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_90'] = array(
    '#type' => 'select',
    '#title' => '90',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_90', 0),
    '#options' => $roles,
    '#description' => t('90 : Linden Contracted'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_100'] = array(
    '#type' => 'select',
    '#title' => '100',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_100', 0),
    '#options' => $roles,
    '#description' => t('100 : GOD_CUSTOMER_SERVICE'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_150'] = array(
    '#type' => 'select',
    '#title' => '150',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_150', 0),
    '#options' => $roles,
    '#description' => t('150 : GOD_LIAISON'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_200'] = array(
    '#type' => 'select',
    '#title' => '200',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_200', 0),
    '#options' => $roles,
    '#description' => t('200 : <strong>GOD_FULL</strong>'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_250'] = array(
    '#type' => 'select',
    '#title' => '250',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_250', 0),
    '#options' => $roles,
    '#description' => t('250 : GOD_MAINTENANCE'),
  );

  /*
   * Models
   */
  $form['models'] = array(
    '#type' => 'fieldset',
    '#title' => t('Models'),
    '#description' => t('UUID of the users to use as models for new registrations.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['models']['d4os_ui_users_models_image_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Images width'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('d4os_ui_users_models_image_width', '150px'),
    '#description' => t('Define the width of the model images.'),
  );
  $form['models']['d4os_ui_users_models_image_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Images height'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('d4os_ui_users_models_image_height', '150px'),
    '#description' => t('Define the height of the model images.'),
  );
	$form['models']['d4os_ui_users_models_output_type'] = array(
    '#type' => 'radios',
    '#title' => t('Output type'),
    '#options' => array('tableselect'=>'tableselect', 'radios'=>'radios'),
    '#default_value' => variable_get('d4os_ui_users_models_output_type', 'tableselect'),
    '#description' => t('Select the output type of the models form.'),
  );
  if (module_exists('lightbox2')) {
    $form['models']['d4os_ui_users_models_image_use_lightbox2'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use lightbox2'),
      '#default_value' => variable_get('d4os_ui_users_models_image_use_lightbox2', 0),
      '#description' => t('Select if you want to use the lightbox 2 effect to display images for models.'),
    );
  }

  /*
   * Grid region owners role
   */
  $form['default_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default roles'),
    '#description' => t('Assign website roles depending on special actions.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['default_roles']['d4os_ui_users_default_grid_user_role'] = array(
    '#type' => 'select',
    '#title' => t('Grid user role'),
    '#default_value' => variable_get('d4os_ui_users_default_grid_user_role', 0),
    '#options' => $roles,
    '#description' => t('This role is assigned to user when they register on the grid or when a new website account is created from the grid auth.'),
  );
	$form['default_roles']['d4os_ui_users_default_region_owner_role'] = array(
    '#type' => 'select',
    '#title' => t('Region owner role'),
    '#default_value' => variable_get('d4os_ui_users_default_region_owner_role', 0),
    '#options' => $roles,
    '#description' => t('This role is assigned to region owners when running cron.'),
  );
  $form['default_roles']['d4os_ui_users_register_role'] = array(
    '#type' => 'select',
    '#title' => t('Grid register role'),
    '#default_value' => variable_get('d4os_ui_users_register_role', 0),
    '#options' => $roles,
    '#description' => t('This role is allowed to register a grid account for themselves.  This is useful for private grids.  Leave it at "None" to disable this feature.'),
  );

  /*
   * Default home
   */
  $form['home'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Home'),
    '#description' => t('Define a home place for new users.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['home']['d4os_ui_users_default_home_region'] = array(
    '#type' => 'textfield',
    '#title' => t('Home region uuid'),
    '#size' => 36,
    '#maxlength' => 36,
    '#default_value' => variable_get('d4os_ui_users_default_home_region', UUID_ZERO),
  );
  $form['home']['d4os_ui_users_default_home_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Home location'),
    '#size' => 13,
    '#maxlength' => 13,
    '#description' => t('Default value') . ' <128,128,128>',
    '#default_value' => variable_get('d4os_ui_users_default_home_location', '<128,128,128>'),
  );
  $form['home']['d4os_ui_users_default_home_look_at'] = array(
    '#type' => 'textfield',
    '#title' => t('Home look at'),
    '#size' => 13,
    '#maxlength' => 13,
    '#description' => t('Default value') . ' <100,100,100>',
    '#default_value' => variable_get('d4os_ui_users_default_home_look_at', '<100,100,100>'),
  );

  return system_settings_form($form);
}

function d4os_ui_users_admin_validate($form, &$form_state) {
  // check the default region
  $default_region_uuid = $form_state['values']['d4os_ui_users_default_home_region'];
  if ($default_region_uuid != UUID_ZERO && $default_region_uuid != '') {
    $d4os_region = D4OS_IO::create('Regions');
    $region_exists = $d4os_region->region_exists($default_region_uuid);
    if (!$region_exists) {
      form_set_error('d4os_ui_users_default_home_region', t('This uuid for region !uuid is not registered on the grid.', array('!uuid' => $default_region_uuid)));
      return;
    }
  }
}