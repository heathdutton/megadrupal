<?php

/**
 * @file
 * Manages PMB cart.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_reader_view_cart($user) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $cart_content = $pmb_data->reader_get_cart();
  $notices = $pmb_data->get_notices($cart_content);

  drupal_set_title(t('My reader account: Cart'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account: @name', array('@name' => $user->name)), 'reader/' . $user->uid);
  $breadcrumb[] = t('Cart');
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('pmb_reader_cart_form', $notices);
}

function pmb_reader_delete_notices_from_cart($user, $notices_ids) {
  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $pmb_data->reader_delete_notices_from_cart($notices_ids);
}

function pmb_reader_empty_cart($user) {
  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $pmb_data->reader_empty_cart();
}

function pmb_reader_add_notices_to_cart($user, $notices_ids) {
  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $pmb_data->reader_add_notices_to_cart($notices_ids);
}
