<?php

/**
 * @file
 * Provide preprocess functions for all the theme templates.
 */

function template_preprocess_ec_charge_variable_description(&$variables) {
  $descriptions = array();
  foreach ($variables['descriptions'] as $key => $value) {
    $descriptions[] = theme('ec_charge_variable_description_row', array('key' => $key, 'description' => $value));
  }
  $variables['descriptions'] = $descriptions;
}

function template_preprocess_ec_charge_add_component(&$variables) {
  ctools_include('ajax');
  ctools_include('modal');

  drupal_add_css(drupal_get_path('module', 'ec_charge') . '/css/admin.css');

  $categories = ec_charge_component_get_categories($variables['type']);

  $cname = $variables['category'] ? $variables['category'] : ec_charge_build_cname(reset(array_keys($categories)));

  $clinks = array();
  $links = array();
  foreach ($categories as $category => $types) {
    $name = ec_charge_build_cname($category);
    $clinks[] = ctools_modal_text_button($category, 'store/nojs/charge-component-add/' . $variables['name'] . '/' . $variables['type'] . '/' . $name, t('View category !category', array('!category' => $category)), $name == $cname ? 'active' : '');

    if (ec_charge_build_cname($category) == $cname) {
      foreach ($types as $id => $type) {
        $links[] = ctools_modal_text_button($type['name'], 'store/nojs/charge-component/' . $variables['name'] . '/' . $variables['type'] . '/add/' . $id, '') . '<dt>' . $type['description'] . '</dt>';
      }
    }
  }

  $variables['categories'] = theme('item_list', array('items' => $clinks));
  $variables['components'] = theme('item_list', array('items' => $links));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_build_cname($name) {
  return trim(drupal_strtolower(str_replace(' ', '-', $name)), '-');
}
