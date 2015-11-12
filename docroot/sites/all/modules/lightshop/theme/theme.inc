<?php

/**
 * Theme API functions file.
 */

/**
 * Preprocess for view basket in block mode.
 */
function template_preprocess_lightshop_block_display(&$vars) {
  drupal_add_css(drupal_get_path('module', 'lightshop') . '/theme/lightshop.css');
  $basket_items = isset($_SESSION['basket']['items']) ? $_SESSION['basket']['items'] : array();
  $vars['not_empty'] = !empty($basket_items);
  $items = array();
  if ($basket_items) {
    foreach ($basket_items as $nid => $item) {
      $items[] = l($item['title'], 'node/' . $nid);
    }
  }

  $vars['count_items'] = $basket_items ? count($_SESSION['basket']['items']) : 0;

  $items_list = array(
    '#theme' => 'fieldset',
    '#title' => t('Items in basket: ') . $vars['count_items'],
    '#attached' => array(
      'js' => array(
        'misc/form.js',
        'misc/collapse.js',
      ),
    ),
    '#attributes' => array(
      'class' => array('collapsible', 'collapsed'),
    ),
    '#value' => theme('item_list', array('items' => $items)),
  );

  $vars['basket_items_list'] = $basket_items ? drupal_render($items_list) : '';

  $vars['total_sum'] = isset($_SESSION['basket']['total_sum']) ? _lightshop_get_price_numberformat($_SESSION['basket']['total_sum']) : FALSE;
  $vars['basket_page_link'] = $basket_items ? l(t('Go to basket page'), 'basket') : '';
}

function theme_lightshop_items_form($vars) {
  $header = _lightshop_basket_items_header();
  $header['del'] = t('Del.');

  $element = $vars['element'];

  $rows = array();
  foreach (element_children($element) as $key) {
    $row = array();

    $row['data'] = array();
    foreach ($header as $field => $title) {
      $row['data'][] = drupal_render($element[$key][$field]);
    }

    $rows[] = $row;
  }
  if (isset($_SESSION['basket']['total_sum'])) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => t('Order total sum'),
          'colspan' => 2,
        ),
        array(
          'data' => _lightshop_get_price_numberformat($_SESSION['basket']['total_sum']),
          'colspan' => 2,
        ),
      ),
      'id' => array('total-sum-tr'),
    );
  }
  return theme('table', array(
      'header' => $header,
      'rows' => $rows,
    ));
}

function theme_lightshop_items_table($vars) {
  $header = _lightshop_basket_items_header();
  $rows = $vars['basket']['items'];

  if (isset($vars['basket']['total_sum'])) {
    $currency = isset($vars['basket']['currency']) ? $vars['basket']['currency'] : '';

    foreach ($rows as $nid => $row) {
      $rows[$nid]['price'] = _lightshop_get_price_numberformat($row['price'], $currency);
    }

    $rows[] = array(
      'data' => array(
        array(
          'data' => t('Order total sum'),
          'colspan' => count($header) - 1,
        ),
        _lightshop_get_price_numberformat($vars['basket']['total_sum'], $currency),
      ),
    );
  }
  $items_table = array(
    '#theme' => 'fieldset',
    '#title' => t('Order items'),
    '#attached' => array(
      'js' => array(
        'misc/form.js',
        'misc/collapse.js',
      ),
    ),
    '#attributes' => array(
      'class' => array('collapsible', 'collapsed'),
    ),
    '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  return drupal_render($items_table);
}

function theme_lightshop_mail_notice($vars) {
  $output = array();
  if ($vars['destination'] == 'site') {
    $output[] = t('New order from the site "@site"', array('@site' => variable_get('site_name')));
    $output[] = t('On site "@site" has been added a new order.', array('@site' => variable_get('site_name')));
  }
  else {
    $output[] = t('You or someone entered your email address in a feedback, the order was made on the site "@site".', array('@site' => variable_get('site_name')));
  }

  $output[] = t('Link to the order page: @link', array('@link' => $vars['view_order_link']));
  $output[] = "==========================================================";
  $output[] = t("Order items:\n\n@order_items", array('@order_items' => $vars['order_items']));
  if ($vars['total_sum']) {
    $output[] = "----------------------------------------------------------";
    $output[] = t("Order total sum: @total_sum", array('@total_sum' => $vars['total_sum']));
  }
  $output[] = "==========================================================";
  if (isset($vars['fields_data'])) {
    $output[] = $vars['fields_data'];
  }
  return $output;
}

/**
 * Preprocess for mail notice template.
 */
function template_preprocess_lightshop_mail_notice(&$vars, $hook) {
  $node = $vars['node'];
  $vars['view_order_link'] = url('order/' . $node->nid, array('query' => array('c' => $node->order_code), 'absolute' => TRUE));
  $vars['order_mail'] = $node->order_mail;
  $vars['order_items'] = '';
  foreach ($node->order_items['items'] as $item) {
    $output = $item['title'];
    if (isset($item['qty'])) {
      $output .= ' | ' . t('qty: @qty', array('@qty' => $item['qty']));
    }
    if (isset($item['price'])) {
      $output .= ' | ' . t('price: @price ', array('@price' => _lightshop_get_price_numberformat($item['price'])));
    }
    $vars['order_items'] .= $output . "\n";
  }

  $vars['total_sum'] = isset($node->order_items['total_sum']) ? _lightshop_get_price_numberformat($node->order_items['total_sum']) : '';

  if ($fields = variable_get('lightshop_mail_fields_' . $vars['destination'], array())) {
    $instances = field_info_instances('node', LIGHTSHOP_ORDER_NODETYPE);
    $fields_data = '';
    foreach ($fields as $field) {
      if ($field_array = $node->{$field}[LANGUAGE_NONE]) {
        $fields_data .= $instances[$field]['label'] . ': ';
        $fields_data .= isset($field_array[1]) ? "\n" : '';
        for ($i = 0; $i < count($field_array); $i++) {
          if (isset($field_array[$i]['value'])) {
            $fields_data .= $field_array[$i]['value'] . "\n";
          }
        }
        $fields_data .= "\n";
      }
    }
    $vars['fields_data'] = $fields_data;
  }
}
