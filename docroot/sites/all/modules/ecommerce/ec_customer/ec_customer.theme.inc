<?php

/**
 * @file
 * Customer theme functions.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_customer_ctypes_form($variables) {
  $form = $variables['form'];
  $output = '';

  $elements = element_children($form['ec_customer_ctypes']);
  if (!empty($elements)) {
    drupal_add_tabledrag('ctype-table', 'order', 'sibling', 'ctype-weight');
    $header = array(t('Name'), t('Type'), t('Description'), t('Weight'));

    foreach ($elements as $element) {
      $rows[] = array(
        'data' => array(
          drupal_render($form['ec_customer_ctypes'][$element]['name']),
          drupal_render($form['ec_customer_ctypes'][$element]['type']),
          drupal_render($form['ec_customer_ctypes'][$element]['description']),
          drupal_render($form['ec_customer_ctypes'][$element]['weight']),
        ),
        'class' => array(($element == 'anonymous') ? '' : 'draggable'),
      );
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'ctype-table')));
  }
  else {
    drupal_set_message(t('No customer modules have been enabled.'), 'error');
  }

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Themes the address form elements on the checkout review page
 */
function theme_ec_customer_address($variables) {
  $form =& $variables['form'];
  $title = drupal_render($form['title']);
  drupal_add_js(drupal_get_path('module', 'ec_customer') . '/js/checkout.js');
  drupal_add_css(drupal_get_path('module', 'ec_customer') . '/css/customer.css');
  return theme('ec_box', array('title' => $title, 'content' => drupal_render_children($form)));
}

/**
 * Insert the email field in case the user is not logged
 *
 * @ingroup themeable
 */
function theme_ec_mail_checkout_form($variables) {
  $form = $variables['form'];
  if (!empty($form['mail']['#markup'])) {
    global $user;

    $changeurl = NULL;
    $uid = ec_customer_get_uid($form['#txn']->customer);
    if (empty($uid)) {
      $changeurl = ' ' . l(t('(Login)'), 'user/login',
        array('class' => array('checkout-link')), 'destination=checkout/process');
    }

    return theme('ec_box', array('title' => t('Email address') . $changeurl, 'content' => drupal_render($form['mail'])));
  }
}
