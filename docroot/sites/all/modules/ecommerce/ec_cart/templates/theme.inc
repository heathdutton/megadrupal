<?php

/**
 * @file
 * Provide preprocess functions for all the theme templates.
 */

/**
 * Create preprocessed variables for ec_cart_empty().
 */
function template_preprocess_ec_cart_empty(&$variables) {
  $variables['empty_text'] = t('Your shopping cart is empty. You may !start_shopping.', array('!start_shopping' => l(t('start shopping'), variable_get('ec_goto_cart_empty', 'products'))));
}

/**
 * Provide preprocessing for the cart view form
 */
function template_preprocess_ec_cart_view_form(&$variables) {
  drupal_add_css(drupal_get_path('module', 'ec_cart') . '/css/cart-view.css');

  $form = & $variables['form'];
  $variables['items'] = array();

  foreach (element_children($form['products']) as $nid) {
    $variables['items'][$nid] = array(
      'title' => render($form['products'][$nid]['title']),
      'price' => render($form['products'][$nid]['price']),
      'qty' => render($form['products'][$nid]['qty']),
      'total' => render($form['products'][$nid]['total']),
      'ops' => render($form['products'][$nid]['ops']),
    );

    $extra = render($form['products'][$nid]);
    if ($extra) {
      $variables['items'][$nid]['extra'] = $extra;
      $has_extra = TRUE;
    }
  }

  $variables['total'] = render($form['total']);

  $variables['output'] = drupal_render_children($form);

  if (isset($has_extra) && $has_extra) {
    $variables['template_files'][] = 'ec-cart-view-form-extra';
  }

  drupal_add_js('misc/tableheader.js', array('type' => 'file', 'weight' => JS_LIBRARY));
}

/**
 * Provide variables for the cart block
 */
function template_preprocess_ec_cart_display_block(&$variables) {
  $flip = array(
    'odd' => 'even',
    'even' => 'odd',
  );
  $class = 'even';

  if (!$variables['cart']) {
    $variables['cart_link'] = l(t('View your cart'), 'cart/view');
    $variables['theme_hook_suggestions'][] = 'ec_cart_display_block_cached';
  }
  else {
    $variables['item_count'] = t('%items in !your_cart', array('%items' => format_plural(count($variables['items']), '1 item', '@count items'), '!your_cart' => l(t('Your cart'), 'cart/view')));
    $variables['total'] = format_currency(ec_cart_get_total($variables['items']));

    if (!empty($variables['items'])) {
      $items = array();
      foreach ($variables['items'] as $item) {
        $class = $flip[$class];
        $attributes = array('class' => array($class));
        $items[$item->nid] = theme('ec_cart_display_item', array('item' => $item, 'attributes' => $attributes));
      }
      $variables['items'] = $items;
      $variables['checkout'] =  t('Ready to <a href="!checkout-url">checkout</a>?', array('!checkout-url' => url('cart/checkout')));
      $variables['checkout_link'] = url('cart/checkout');
    }
    else {
      $variables['theme_hook_suggestions'][] = 'ec_cart_display_block_empty';
    }
  }
}

function template_preprocess_ec_cart_display_item(&$variables) {
  $item = $variables['node'] = $variables['item'];

  $menu = menu_get_item('node/' . $item->nid);

  $variables += array(
    'nid' => $item->nid,
    'title' => $item->title,
    'qty' => ec_product_has_quantity($item) ? $item->qty : '',
    'qty_multiplier' => ec_product_has_quantity($item) ? ' x ' : '',
    'price' => format_currency(ec_cart_get_item_total($item)),
    'path' => ($menu['access'] ? 'node/'. $item->nid : ''),
    'link' => ($menu['access'] ? l($item->title, 'node/'. $item->nid, array('html' => TRUE)) : $item->title),
    'remove_path' => url('cart/' . $item->nid . '/0', array('query' => drupal_get_destination())),
    'remove_link' => l(t('X'), 'cart/' . $item->nid . '/0', array('query' => drupal_get_destination())),
  );
  $variables += (array) $variables['node'];

  //$variables['attributes_array']['class'][] = 'clearfix';
  $variables['attributes_array']['class'][] = 'ec-cart-item-wrapper';

  $variables['template_files'][] = 'ec-cart-display-row-' . $item->ptype;
  $variables['template_files'][] = 'ec-cart-display-row-' . $item->type;
  $variables['template_files'][] = 'ec-cart-display-row-' . $item->type . '-' . $item->ptype;
}
