<?php

/**
 * @file
 * Define Pick-pack pont Pane to checkout process.
 */

/**
 * Pick-pack pont Pane: form callback.
 */
function commerce_pickpackpoints_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  if (!commerce_pickpackpoints_shipping_is_pickpackpoints($order)) {
    // No pick-pack shipping.
    $referer = explode('/', $_SERVER['HTTP_REFERER']);
    if (end($referer) == $form_state['checkout_page']['next_page']) {
      $order = commerce_order_status_update($order, 'checkout_' . $form_state['checkout_page']['prev_page']);
      drupal_goto(commerce_checkout_order_uri($order));
    }
    if (end($referer) == $form_state['checkout_page']['prev_page']) {
      $order = commerce_order_status_update($order, 'checkout_' . $form_state['checkout_page']['next_page']);
      drupal_goto(commerce_checkout_order_uri($order));
    }
  }
  $form_state['checkout_pane'] = $checkout_pane['pane_id'];
  $lists = commerce_pickpackpoints_get_lists();

  $county = 'none';
  $city = 'none';
  $shop = 'none';

  $form['county'] = array(
    '#type' => 'select',
    '#title' => t('County'),
    '#default_value' => isset($form_state['values']['checkout_pane_pickpackpoints']['county']) ? $form_state['values']['checkout_pane_pickpackpoints']['county'] : $county,
    '#options' => $lists['pp_county'],
    '#ajax' => array(
      'callback' => 'commerce_pickpackpoints_choice_county_js',
      'wrapper' => 'ppp-datapicker-city-wrapper',
    ),
  );
  $form['city'] = array(
    '#type' => 'select',
    '#title' => t('City'),
    '#default_value' => isset($form_state['values']['checkout_pane_pickpackpoints']['city']) ? $form_state['values']['checkout_pane_pickpackpoints']['city'] : $city,
    '#options' => _commerce_pickpackpoints_get_city_options($lists, $form_state),
    '#ajax' => array(
      'callback' => 'commerce_pickpackpoints_choice_city_js',
      'wrapper' => 'ppp-datapicker-shop-wrapper',
    ),
    '#prefix' => '<div id="ppp-datapicker-city-wrapper">',
    '#suffix' => '</div>',
    '#disabled' => isset($form_state['values']['checkout_pane_pickpackpoints']['county']) && $form_state['values']['checkout_pane_pickpackpoints']['county'] != 'none' ? FALSE : TRUE,
  );
  $form['shop'] = array(
    '#type' => 'select',
    '#title' => t('Shop'),
    '#default_value' => isset($form_state['values']['checkout_pane_pickpackpoints']['shop']) ? $form_state['values']['checkout_pane_pickpackpoints']['shop'] : $shop,
    '#options' => _commerce_pickpackpoints_get_shop_options($lists, $form_state),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'commerce_pickpackpoints_choice_shop_js',
      'wrapper' => 'ppp-datapicker-shop-table',
    ),
    '#prefix' => '<div id="ppp-datapicker-shop-wrapper">',
    '#suffix' => '</div>',
    '#disabled' => isset($form_state['values']['checkout_pane_pickpackpoints']['city']) && $form_state['values']['checkout_pane_pickpackpoints']['city'] != 'none' ? FALSE : TRUE,
  );

  $form['shop_info'] = array(
    '#markup' => _commerce_pickpackpoints_get_shop_render($lists, $form_state),
    '#prefix' => '<div id="ppp-datapicker-shop-table">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Ajax callback from county select.
 */
function commerce_pickpackpoints_choice_county_js($form, $form_state, $checkout_pane, $order) {
  return $form[$form_state['checkout_pane']]['city'];
}

/**
 * Ajax callback from city select.
 */
function commerce_pickpackpoints_choice_city_js($form, $form_state, $checkout_pane, $order) {
  return $form[$form_state['checkout_pane']]['shop'];
}

/**
 * Ajax callback from shop select.
 */
function commerce_pickpackpoints_choice_shop_js($form, $form_state) {
  return $form[$form_state['checkout_pane']]['shop_info'];
}

/**
 * Helper for generating city options.
 */
function _commerce_pickpackpoints_get_city_options($lists, $form_state) {
  if (isset($form_state['values'][$form_state['checkout_pane']]['county']) && $form_state['values'][$form_state['checkout_pane']]['county'] != 'none') {
    $county = $form_state['values'][$form_state['checkout_pane']]['county'];
    return $lists['pp_city'][$county];
  }
  return $lists['pp_all_cities'];
}

/**
 * Helper for generating shop options.
 */
function _commerce_pickpackpoints_get_shop_options($lists, $form_state) {
  if (isset($form_state['values'][$form_state['checkout_pane']]['city']) && $form_state['values'][$form_state['checkout_pane']]['city'] != 'none') {
    $county = $form_state['values'][$form_state['checkout_pane']]['county'];
    $city = $form_state['values'][$form_state['checkout_pane']]['city'];
    return $lists['pp_list'][$county][$city];
  }
  return $lists['pp_all_shops'];
}

/**
 * Helper for display rendered shop data.
 */
function _commerce_pickpackpoints_get_shop_render($lists, $form_state) {
  if (isset($form_state['values'][$form_state['checkout_pane']]['shop']) && $form_state['values'][$form_state['checkout_pane']]['shop'] != 'none') {
    $shop = $form_state['values'][$form_state['checkout_pane']]['shop'];
    return $lists['pp_rendered_shop'][$shop];
  }
  return '';
}

/**
 * Pick-pack pont pane: validation callback.
 */
function commerce_pickpackpoints_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $shop_id = $form_state['values'][$checkout_pane['pane_id']]['shop'];
  if ($shop_id == 'none') {
    form_set_error($checkout_pane['pane_id'] . '][shop', t('Please choose a shop.'));
    return FALSE;
  }
  return TRUE;
}

/**
 * Pick-pack pont pane: checkout form submission callback.
 */
function commerce_pickpackpoints_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if (!empty($form_state['values'][$checkout_pane['pane_id']])) {
    $pane_values = $form_state['values'][$checkout_pane['pane_id']];
    $order->data['pick_pack_shop'] = $pane_values['shop'];
    if (module_exists('commerce_pickpackpoints_field')) {
      $order->pickpack_field_pickpack[LANGUAGE_NONE][0]['value'] = $pane_values['shop'];
    }
  }
}

/**
 * Pick-pack pont pane.
 *
 * Presents the information we've collected in the Review checkout pane.
 */
function commerce_pickpackpoints_pane_review($form, $form_state, $checkout_pane, $order) {
  if (!commerce_pickpackpoints_shipping_is_pickpackpoints($order)) {
    return '';
  }
  $data = commerce_pickpackpoints_get_data();
  $content = array(
    'nickname' => array(
      '#type' => 'item',
      '#title' => t('Products delivery to Pick-pack pont') . ':',
      '#markup' => commerce_pickpackpoints_get_rendered_data($order->data['pick_pack_shop'], $data[$order->data['pick_pack_shop']]),
    ),
  );
  return drupal_render($content);
}
