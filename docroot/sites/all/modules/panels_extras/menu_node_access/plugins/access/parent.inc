<?php

/**
 * @file
 * Plugin to provide access control based upon if node being viewed belongs to a menu(s).
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Node Being Viewed, Belongs to a Selected Fixed Parent Item"),
  'description' => t('Control access by if the node being viewed belongs to a fixed parent menu item.'),
  'callback' => 'menu_node_access_parent_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'menu_node_access_parent_access_settings',
  'settings form submit' => 'menu_node_access_parent_access_settings_submit',
  'summary' => 'menu_node_access_parent_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Settings form for the 'by menus' access plugin
 */
function menu_node_access_parent_access_settings($form, &$form_state, $conf) {
  $menus = menu_get_menus();
  foreach ($menus as $menu => $info) {
    $options[$menu] = check_plain($info);
  }

  $form['settings']['parent_mlid'] = array(
    '#type' => 'select',
    '#title' => t('Fixed parent item'),
    '#default_value' => $conf['menu_name'] . ':' . $conf['parent_mlid'],
    '#options' => menu_parent_options($menus, array('mlid' => 0)),
    '#description' => t('The tree of links will only contain children of the selected menu item.'),
  );
  return $form;
}

/**
 * do nothing, what the default handler will do is good enough.
 */
function menu_node_access_parent_access_settings_submit($form, &$form_state) {
  // Determine the fixed parent item's menu and mlid.
  list($menu_name, $parent_mlid) = explode(':', $form_state['values']['settings']['parent_mlid']);
  if ($parent_mlid) {
    // If mlid is set, its menu overrides the menu_name option.
    $form_state['values']['settings']['menu_name'] = $menu_name;
    $form_state['values']['settings']['parent_mlid'] = $parent_mlid;
  }
  else {
    // Otherwise the menu_name overrides the parent item option.
    $form_state['values']['settings']['parent_mlid'] = $menu_name . ':0';
  }
  // The value of "parent" stored in the database/config array is the menu name
  // combined with the optional parent menu item's mlid.
  $form_state['values']['settings']['parent'] = $form_state['values']['settings']['parent_mlid'];
}

/**
 * Check for access.
 */
function menu_node_access_parent_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->menu_node_links)) {
    return FALSE;
  }
  foreach ($context->data->menu_node_links as $value ){
    if ($conf['menu_name'] == $value->menu_name ) {
      $value = (array)$value;
      foreach ( $value as $key => $plid ) {
        if ( in_array($key, array('p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8', 'p9')) ) { 
          if ($plid == $conf['parent']){
            return TRUE;
          }
        }
      }
    }
  }
  
  return FALSE;
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function menu_node_access_parent_access_summary($conf, $context) {
  if ( empty($conf['menu_name']) || empty($conf['parent']) ) {
    return;
  }
  $link = (menu_link_load($conf['parent']));
  if ( empty($link['link_title']) ){
    return;
  }
  return t('Menu is '. $conf['menu_name']) . '<br />' . t('Fix Parent Item is '. $link['link_title']);
}