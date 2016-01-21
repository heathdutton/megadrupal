<?php

/**
 * @file
 * This handles the crc pseudo-hooks for the following modules:
 *  menu
 *
 *  It implement the following pseudo-hooks
 *
 *  hook_crc_node_form($settings, $org_form, $form_state)
 *  hook_crc_node_prepopulate(&$node, $settings, $values, $base_node)
 *  hook_crc_type_settings($type, $settings, $org_form, $form_state)
 */

define('CRC_MENU_UNRELATED', 0);
define('CRC_MENU_SIBBLING', 1);
define('CRC_MENU_CHILD', 2);
define('CRC_MENU_CUSTOM', 3);

/**
 * Implementation of hook_crc_node_form().
 *
 * This provides the additional form elements to show on the
 * create related content page.
 *
 * @param $type The content type to generate the form for.
 * @param $settings Settings for all moudles
 * @param $form_state
 * @param $base_node Parent node
 * @return $form
 */
function menu_crc_node_form($settings, $form_state, $base_node) {
  $options = array(CRC_MENU_UNRELATED => t('Unrelated'), CRC_MENU_SIBBLING => t('Sibbling'), CRC_MENU_CHILD => t('Child'), CRC_MENU_CUSTOM => t('Custom'));
  $form = array();
  if (user_access('administer menu')) {
    $form['menu_parent'] = array(
      '#type' => 'radios',
      '#title' => t('Menu relationship'),
      '#options' => $options,
      '#default_value' => key($options),
    );
  }
  return $form;
}

/**
 * Helper function
 *
 * Sets the default values to use.
 *
 * @param $settings
 * @return void
 */
function menu_crc_settings(&$settings = array()) {
  $settings += array(
    'menu_name' => 2,
    'menu_name_custom' => '',
    'menu_parent' => CRC_MENU_UNRELATED,
    'menu_parent_custom' => 'primary-links:0',
  );
}

/**
 * Implementation of hook_crc_node_prepopulate().
 *
 * This prepopulates the node object before it is passed
 * to the node_form.
 *
 * @param $node
 * @param $settings
 * @param $values
 * @param $base_node
 * @return void
 */
function menu_crc_node_prepopulate(&$node, $settings, $values, $base_node) {
  $node->menu = crc_default_menu();
  switch ($settings['menu_name']) {
    case 1:
      $node->menu['link_title'] = '';
      break;
    case 2:
      $node->menu['link_title'] = $values['name'];
      break;
    case 3:
    default:
      $node->menu['link_title'] = $settings['menu_name_custom'];
      if (module_exists('token')) {
        $node->menu['link_title'] = token_replace($node->menu['link_title'], 'crc', $base_node);
      }
      break;
  }

  // If no user access, set the defaults
  $switch_key = user_access('administer menu') ? $values['menu_parent'] : $settings['menu_parent'];
  switch ($switch_key) {
    case CRC_MENU_SIBBLING:
    case CRC_MENU_CHILD:
      $node->menu['menu_name'] = $base_node->menu['menu_name'];
      $node->menu['plid'] = ($switch_key == CRC_MENU_SIBBLING) ? $base_node->menu['plid'] : $base_node->menu['mlid'];
      break;
    case CRC_MENU_CUSTOM: // Custom
      if (!empty($settings['menu_parent_custom'])) {
        list($menu_name, $mlid) = explode(':', $settings['menu_parent_custom']);
        if (!empty($menu_name) && strlen((string)$mlid)) {
          $node->menu['menu_name'] = $menu_name;
          $node->menu['plid'] = $mlid;
        }
      }
      break;
    case CRC_MENU_UNRELATED:
    default:
      break;
  }
}

/**
 * Implementation of hook_crc_type_settings().
 *
 * This provides the form elements to show on the edit content
 * type form. These are used to set the defaults for the
 * content type.
 *
 * @param $type
 * @param $settings
 * @param $org_form
 * @param $form_state
 * @return $form
 */
function menu_crc_type_settings($type, $settings, $org_form, $form_state) {
  $form = array();

  if (user_access('administer menu')) {
    $menu_options = array(1 => t('None'), 2 => t('Copy title'), 3 => t('Custom'));
    $form['menu_name'] = array(
      '#title' => t('Default menu link title'),
      '#type' => 'radios',
      '#options' =>  $menu_options,
      '#default_value' => $settings['menu_name'],
    );

    $form['menu_name_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom menu link title'),
      '#default_value' => $settings['menu_name_custom'],
    );
    if (module_exists('token')) {
      $form['menu_name_custom']['#suffix'] = theme('token_help', 'crc');
    }

    $menu_parent = array(CRC_MENU_UNRELATED => t('Unrelated'), CRC_MENU_SIBBLING => t('Sibbling'), CRC_MENU_CHILD => t('Child'), CRC_MENU_CUSTOM => t('Custom'));
    $form['menu_parent'] = array(
      '#title' => t('Default parent item behaviour'),
      '#type' => 'radios',
      '#options' =>  $menu_parent,
      '#default_value' => $settings['menu_parent'],
      '#description' => t('This selects the default relationship of the new node to the exisitng node if the user does not have administrate menu permission.'),
    );

    // Generate a list of possible parents
    $form['menu_parent_custom'] = array(
      '#type' => 'select',
      '#title' => t('Custom parent item'),
      '#default_value' => $settings['menu_parent_custom'],
      '#options' => menu_parent_options(menu_get_menus(), crc_default_menu()),
      '#description' => t('The maximum depth for an item and all its children is fixed at !maxdepth. Some menu items may not be available as parents if selecting them would exceed this limit.', array('!maxdepth' => MENU_MAX_DEPTH)),
      '#attributes' => array('class' => 'menu-title-select'),
    );
  }
  else {
    // the values are saved directly, so add these here
    foreach (array('menu_name', 'menu_name_custom', 'menu_parent', 'menu_parent_custom') as $key) {
      $form[$key] = array(
        '#type' => 'value',
        '#value' => $settings[$key],
      );
    }
  }
  return $form;
}
