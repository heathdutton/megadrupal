<?php

/**
 * @file
 * PMB browse serials.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_catalog_serials() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $serials = $pmb_data->get_serials();

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Serials');
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_catalog_serials', array(
    'serials' => $serials,
    'parameters' => array(),
  ));
}
