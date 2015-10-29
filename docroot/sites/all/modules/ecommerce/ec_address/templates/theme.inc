<?php

/**
 * @file
 */
function template_preprocess_ec_address_user_list(&$variables) {
  module_load_include('inc', 'ec_address', 'ec_address.admin');
  drupal_add_css(drupal_get_path('module', 'ec_address') . '/css/address.css');

  $variables['address_list'] = views_embed_view('ec_address_list');
  $variables['address_form'] = drupal_render(drupal_get_form('ec_address_form', $variables['user_id']));
}
