<?php

/**
 * @file
 * Handle all checkout theme items.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_checkout_checkout_review_form($variables) {
  $form = $variables['form'];
  drupal_add_css(drupal_get_path('module', 'ec_checkout') . '/css/ec_checkout_form.css');

  $header = array(
    array(
      'data' => t('Item'),
      'class' => array('ec-checkout-review-form-title'),
    ),
    array(
      'data' => t('Qty'),
      'class' => array('ec-checkout-review-form-quantity'),
    ),
    array(
      'data' => t('Price'),
      'class' => array('ec-checkout-review-form-price'),
    ),
    array(
      'data' => t('Subtotal'),
      'class' => array('ec-checkout-review-form-subtotal'),
    ),
    array(
      'data' => '',
      'class' => array('ec-checkout-review-form-ops'),
    ),
  );
  $rows = array();
  if (!empty($form['items'])) {
    foreach ($form['items'] as $nid => $line) {
      if (is_numeric($nid)) {
        $rows[] = array(
          'data' => array(
            array(
              'data' => drupal_render($form['items'][$nid]['title']),
              'class' => array('ec-checkout-review-form-title'),
            ),
            array(
              'data' => drupal_render($form['items'][$nid]['qty']),
              'class' => array('ec-checkout-review-form-quantity'),
            ),
            array(
              'data' => drupal_render($form['items'][$nid]['price']),
              'class' => array('ec-checkout-review-form-price'),
            ),
            array(
              'data' => drupal_render($form['items'][$nid]['subtotal']),
              'class' => array('ec-checkout-review-form-subtotal'),
            ),
            array(
              'data' => drupal_render($form['items'][$nid]['options']),
              'class' => array('ec-checkout-review-form-ops'),
            ),
          ),
          'class' => array('ec-checkout-review-form-row-item'),
        );
      }
    }
  }

  $rows[] = array(
    'data' => array(
      'attributes' => array('colspan' => 5),
    ),
    'class' => array('ec-checkout-review-form-row-blank'),
  );
  foreach (element_children($form['totals']) as $id) {
    $row = array(
      'data' => array(
        array(
          'data' => drupal_render($form['totals'][$id]['title']),
          'class' => array('ec-checkout-review-form-title'),
        ),
        '',
        '',
        array(
          'data' => drupal_render($form['totals'][$id]['amount']),
          'class' => array('ec-checkout-review-form-subtotal'),
        ),
        '',
      ),
    );

    if (isset($form['totals'][$id]['#attributes'])) {
      $row += $form['totals'][$id]['#attributes'];
    }

    $rows[] = $row;
  }

  $content = theme('table', array('header' => $header, 'rows' => $rows));

  return theme('ec_box', array('title' => t('Order Summary'), 'content' => $content, 'attributes' => array('class' => array('ec-checkout-items'))));
}

/**
 * Themes the admin screen.
 * @ingroup themeable
 */
function theme_ec_checkout_admin_screen_form($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('screen-table', 'order', 'sibling', 'screen-weight', 'screen-weight');
  $output = '';
  $header = array(t('Type'), t('Description'), t('Weight'));

  foreach (element_children($form) as $type) {
    $rows[] = array(
      'data' => array(
        drupal_render($form[$type]['name']),
        drupal_render($form[$type]['description']),
        drupal_render($form[$type]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'screen-table')));
  $output .= drupal_render_children($form);

  return $output;
}
