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
  'title' => t("Node Being Viewed: Belongs to a Selected Menu"),
  'description' => t('Control access by if the being viewed belongs to a menu(s).'),
  'callback' => 'menu_node_access_mna_access_check',
  'default' => array('menu' => array()),
  'settings form' => 'menu_node_access_mna_access_settings',
  'settings form submit' => 'menu_node_access_mna_access_settings_submit',
  'summary' => 'menu_node_access_mna_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Settings form for the 'by menus' access plugin
 */
function menu_node_access_mna_access_settings($form, &$form_state, $conf) {
  $menus = menu_get_menus();
  foreach ($menus as $menu => $info) {
    $options[$menu] = check_plain($info);
  }

  $form['settings']['menu'] = array(
    '#title' => t('Menu(s) Node Being Viewed belongs to'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => t('Only the checked menus will be valid.'),
    '#default_value' => $conf['menu'],
  );
  return $form;
}

/**
 * Compress the menus allowed to the minimum.
 */
function menu_node_access_mna_access_settings_submit($form, &$form_state) {
  $form_state['values']['settings']['menu'] = array_filter($form_state['values']['settings']['menu']);
}

/**
 * Check for access.
 */
function menu_node_access_mna_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->menu_node_links)) {
    return FALSE;
  }
  foreach ($context->data->menu_node_links as $value ){
    $temp[$value->menu_name] = $value->menu_name;
  }
  $check_node_in_selected_menus = 0;
  $check_node_in_selected_menus = count(array_intersect($conf['menu'], $temp));

  if ( $check_node_in_selected_menus > 0 ){
    return TRUE;
  }

  return FALSE;
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function menu_node_access_mna_access_summary($conf, $context) {
  if (!isset($conf['menu'])) {
    $conf['menu'] = array();
  }
  $menus = menu_get_menus();

  $names = array();
  // If a menu doesn't exist, let the user know, but prevent a notice.
  $missing_menus = array();

  foreach (array_filter($conf['menu']) as $menu) {
    if (!empty($menus[$menu])) {
      $names[] = check_plain($menus[$menu]);
    }
    else {
      $missing_types[] = check_plain($menu);
    }
  }

  if (empty($names) && empty($missing_menus)) {
    return t('@identifier is any menu(s)', array('@identifier' => $context->identifier));
  }

  if (!empty($missing_menus)) {
    $output = array();
    if (!empty($names)) {
      $output[] = format_plural(count($names), '@identifier is inside menu(s) "@menus"', '@identifier menu is one of "@menus"', array('@menus' => implode(', ', $names), '@identifier' => $context->identifier));
    }
    $output[] = format_plural(count($missing_menus), 'Missing/ deleted menu "@menus"', 'Missing/ deleted menu is one of "@menus"', array('@menus' => implode(', ', $missing_menus)));
    return implode(' | ', $output);
  }

  return format_plural(count($names), '@identifier is inside menu(s)  "@menus"', '@identifier menu is one of "@menus"', array('@menus' => implode(', ', $names), '@identifier' => $context->identifier));
}