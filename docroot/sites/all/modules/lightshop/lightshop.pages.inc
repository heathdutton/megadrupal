<?php

/**
 * Menu callbacks.
 */

/**
 * Submit actions for add2basket link.
 */
function lightshop_add2basket($node, $type) {
  // Verification of the absence of this node in basket.
  if (_lightshop_in_basket($node->nid)) {
    drupal_set_message(t('"@title" is already in your basket', array('@title' => $node->title)
      ), 'warning');
  }
  else {
    global $user;
    if ($user->uid == 0 && !drupal_session_started()) {
      // Must initialize sessions for anonymous users.
      drupal_session_start();
    }
    $item = array();
    $item['title'] = $node->title;
    if (variable_get('lightshop_show_qty', TRUE)) {
      $item['qty'] = 1;
    }
    if (variable_get('lightshop_show_price', FALSE) && ($price = _lightshop_get_price($node))) {
      if ($price <= 0 || !is_numeric($price)
      ) {
        drupal_set_message(t('Sorry, but this product currently unavailable
        for ordering'), 'warning');
        $item = FALSE;
      }
      else {
        if (!isset($_SESSION['basket']['currency'])) {
          $_SESSION['basket']['currency'] = variable_get('lightshop_price_suffix', '');
        }
        if (!isset($_SESSION['basket']['total_sum'])) {
          $_SESSION['basket']['total_sum'] = 0;
        }
        $_SESSION['basket']['total_sum'] += $price;
        $item['price'] = $_SESSION['basket']['store'][$node->nid]['unit_price'] = $price;
      }
    }
    if ($item) {
      $_SESSION['basket']['items'][$node->nid] = $item;
      drupal_set_message(t("'@title' added in your basket", array('@title' => $node->title)));
    }
  }
  if ($type == 'ajax') {
    $basket_block = lightshop_block_content();
    $commands = array();
    $commands[] = ajax_command_replace(NULL, lightshop_add2basket_link($node->nid));
    $commands[] = ajax_command_replace('#lightshop-ajax-block-content', render($basket_block));
    $output = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($output);
  }
  else {
    drupal_goto();
  }
}

/**
 * Shared Ajax callback for basket page actions.
 */
function lightshop_ajaxform_process() {
  if (!isset($_SESSION['basket']['items'])) {
    drupal_set_message(t('The form is invalid. Your basket is currently empty.'), 'error');
    return theme('status_messages');
  }
  $commands = array();
  $title = '';

  list($form, $form_state) = ajax_get_form();
  $element_name = $form_state['input']['_triggering_element_name'];

  switch ($element_name) {
    case 'clear':
      $form = drupal_get_form('_lightshop_clear_confirm');
      $form['#action'] = 'basket/clear';
      $title = t('Do you really want to delete all items from the basket?');
      break;

    case 'checkout':
      $form = lightshop_checkout(TRUE);
      $form['#action'] = 'basket/checkout';
      $title = t('Checkout');
      break;

    default:
      drupal_process_form($form['#form_id'], $form, $form_state);
  }

  if ($element_name == 'new_order' && !form_get_errors()) {
    $form = array(
      'type' => 'item',
      'value' => node_view($form_state['node']),
    );
    $title = t('Your order has been successfully created');
  }

  $commands[] = ajax_command_replace(NULL, render($form));

  if ($title) {
    $commands[] = ajax_command_html('#lightshop-page-title *:first-child', $title);
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * The form that displays the contents of basket and control buttons.
 */
function lightshop_basket_form($form, &$form_state) {
  // Build Breadcrumbs.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '');
  // Link to current URL.
  $breadcrumb[] = l(drupal_get_title(), current_path());
  // Set Breadcrumbs.
  drupal_set_breadcrumb($breadcrumb);

  // Check for empty session.
  if (!isset($_SESSION['basket']['items'])) {
    drupal_set_title(t('Your basket is empty'));
    $form['empty'] = array(
      '#type' => 'markup',
      '#markup' => t('There is nothing in your basket at the moment.'),
    );
  }
  else {
    $form['#id'] = drupal_html_id('lightshop_basket_form');
    $form['#attributes'] = array(
      'class' => array('lightshop_basket'),
    );
    $ajax_settings = array(
      'path' => 'lightshop/ajax',
      'wrapper' => $form['#id'],
    );

    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear the basket'),
      '#name' => 'clear',
      '#ajax' => $ajax_settings,
      '#attributes' => array(
        'class' => array('clear-button'),
      ),
    );

    $form['items'] = array(
      '#tree' => TRUE,
      '#theme' => 'lightshop_items_form',
    );

    foreach ($_SESSION['basket']['items'] as $nid => $data) {
      $form['items'][$nid]['title'] = array(
        '#type' => 'link',
        '#title' => $data['title'],
        '#href' => 'node/' . $nid,
      );
      if (isset($data['qty'])) {
        $form['items'][$nid]['qty'] = array(
          '#type' => 'textfield',
          '#default_value' => $data['qty'],
          '#size' => 4,
          '#element_validate' => array('_lightshop_qty_element_validate'),
          '#attributes' => array(
            'class' => array('qty'),
          ),
        );
      }
      if (isset($data['price'])) {
        $form['items'][$nid]['price'] = array(
          '#markup' => _lightshop_get_price_numberformat($data['price']),
          '#size' => 5,
        );
      }
      $form['items'][$nid]['del'] = array(
        '#type' => 'checkbox',
      );
    }

    $form['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array(
        'class' => array('lightshop-form-actions'),
      ),
    );
    $form['actions']['update'] = array(
      '#type' => 'submit',
      '#submit' => array('_lightshop_basket_update'),
      '#value' => t('Update'),
      '#ajax' => $ajax_settings,
      '#attributes' => array(
        'class' => array('update-button'),
      ),
    );

    $form['actions']['checkout'] = array(
      '#type' => 'submit',
      '#name' => 'checkout',
      '#value' => t('Checkout'),
      '#ajax' => $ajax_settings,
      '#attributes' => array(
        'class' => array('checkout-button'),
      ),
    );
    $form['#validate'][] = 'lightshop_check_empty_basket';
    $form['#attached'] = array(
      'css' => array(
        drupal_get_path('module', 'lightshop') . '/theme/lightshop.css',
      ),
      'js' => array(
        drupal_get_path('module', 'lightshop') . '/theme/lightshop.js',
      ),
    );
  }
  return $form;
}

/**
 * Basket page form qty element validate callback.
 */
function _lightshop_qty_element_validate($element, &$form_state, $form) {
  if (!filter_var($element['#value'], FILTER_VALIDATE_INT) || $element['#value'] <= 0) {
    form_error($element, t('Incorrect value of qty'));
  }
}

/**
 * Basket page form submit callback.
 */
function lightshop_basket_form_submit($form, &$form_state) {
  switch ($form_state['triggering_element']['#name']) {
    case 'clear':
      $form_state['redirect'] = 'basket/clear';
      break;

    case 'checkout':
      $form_state['redirect'] = 'basket/checkout';
      break;
  }
}

/**
 * Handling data update form the basket page.
 */
function _lightshop_basket_update($form, &$form_state) {
  $values = $form_state['values']['items'];
  $updated = FALSE;
  foreach ($values as $key => $val) {
    if ($val['del']) {
      drupal_set_message(t('"%title" was removed from your basket', array(
            '%title' => $_SESSION['basket']['items'][$key]['title'],
          )));
      unset($_SESSION['basket']['items'][$key]);
      if (isset($_SESSION['basket']['store'][$key])) {
        unset($_SESSION['basket']['store'][$key]);
      }
      $updated = TRUE;
    }
    elseif (variable_get('lightshop_show_qty', TRUE) && $_SESSION['basket']['items'][$key]['qty'] != $val['qty']) {
      $_SESSION['basket']['items'][$key]['qty'] = $val['qty'];
      if (variable_get('lightshop_show_price', FALSE)) {
        $_SESSION['basket']['items'][$key]['price'] = $val['qty'] * $_SESSION['basket']['store'][$key]['unit_price'];
      }
      drupal_set_message(t('"%title": successfully updated', array(
            '%title' => $_SESSION['basket']['items'][$key]['title'],
          )));
      $updated = TRUE;
    }
  }
  if ($updated) {
    $form_state['rebuild'] = TRUE;
    _lightshop_update_total();
  }
}

/**
 * Updating total sum of basket items.
 */
function _lightshop_update_total() {
  if (empty($_SESSION['basket']['items'])) {
    unset($_SESSION['basket']);
  }
  elseif (variable_get('lightshop_show_price', FALSE) && variable_get('lightshop_price_field', '')) {
    $sum = 0;
    foreach ($_SESSION['basket']['items'] as $item) {
      $sum += $item['price'];
    }
    $_SESSION['basket']['total_sum'] = $sum;
  }
}

/**
 * Calling the checkout form.
 */
function lightshop_checkout($ajax = FALSE) {
  module_load_include('inc', 'node', 'node.pages');
  $add_form = node_add('lightshop_order');
  return $ajax ? $add_form : drupal_render($add_form);
}

/**
 * Confirmation form when cleaning basket.
 */
function _lightshop_clear_confirm($form, &$form_state) {
  if (isset($_SESSION['basket']['items'])) {
    return confirm_form($form,
      t('Do you really want to delete all items from the basket?'),
      'basket',
      t('This operation is irreversible'),
      t('Yes, clear'),
      t('Cancel')
    );
  }
}

/**
 * Confirmation form submit callback.
 */
function _lightshop_clear_confirm_submit($form, &$form_state) {
  unset($_SESSION['basket']);
  drupal_set_message(t('Your basket is cleared'));
  drupal_goto('basket');
}

/**
 * Helper function to check the access code when referring to a specific order.
 */
function _lightshop_get_my_order($node) {
  $code = isset($_GET['c']) && !empty($_GET['c']) ? check_plain($_GET['c']) : FALSE;
  $node_code = isset($node->order_code) ? $node->order_code : FALSE;
  if ($code && $node_code && ($code === $node_code)) {
    $_SESSION['order_codes'][] = $code;
    drupal_goto('node/' . $node->nid);
  }
  drupal_not_found();
}

/**
 * Menu callback; displays page containing recent orders of a given user.
 */
function lightshop_my_orders_page($account) {
  drupal_set_title(t("@name's Order History", array('@name' => format_username($account))), PASS_THROUGH);

  $build = array();
  $query = db_select('node', 'n')->extend('PagerDefault');
  $nids = $query->fields('n', array('nid', 'sticky', 'created'))->condition('type', LIGHTSHOP_ORDER_NODETYPE)->condition('uid', $account->uid)->condition('status', 1)->orderBy('created', 'DESC')->limit(variable_get('default_nodes_main', 10))->addTag('node_access')->execute()->fetchCol();
  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  }
  else {
    drupal_set_message(t('You have no orders for this account.'));
  }

  return $build;
}

