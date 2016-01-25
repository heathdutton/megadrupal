<?php

/**
 * @file
 * Themeable functions for ec_receipt.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_receipt_admin_atypes_form($variables) {
  $form = $variables['form'];
  $output = '';
  $header = array(
    array(
      'data' => t('Name'),
      'colspan' => 2,
    ),
    t('Description'),
  );

  foreach (element_children($form['types']) as $child) {
    $rows[] = array(
      drupal_render($form['types'][$child]['name']),
      drupal_render($form['types'][$child]['type']),
      drupal_render($form['types'][$child]['description']),
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No allocation types enabled'),
        'colspan' => 3,
      ),
    );
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output . drupal_render_children($form);
}

/**
 * @ingroup themeable
 */
function theme_ec_receipt_admin_rtypes_form($variables) {
  $form = $variables['form'];
  $header = array(
    t('Name'),
    '',
    t('Description'),
    t('Payments'),
    t('3rd Party Payments'),
    t('Refunds'),
    t('Pay to'),
    t('Recurring Payments'),
    t('Weight'),
  );
  $children = element_children($form['types']);
  if (!empty($form['types'])) {
    foreach ($children as $element) {
      $form['types'][$element]['weight']['#attributes']['class'][] = 'receipt-weights';
      $rows[] = array(
        'data' => array(
          array('data' => drupal_render($form['types'][$element]['name'])),
          array('data' => drupal_render($form['types'][$element]['icon'])),
          array('data' => drupal_render($form['types'][$element]['description'])),
          array(
            'data' => drupal_render($form['types'][$element]['allow_payments']),
            'align' => 'center',
          ),
          array(
            'data' => drupal_render($form['types'][$element]['allow_admin_payments']),
            'align' => 'center',
          ),
          array(
            'data' => drupal_render($form['types'][$element]['allow_refunds']),
            'align' => 'center',
          ),
          array(
            'data' => drupal_render($form['types'][$element]['allow_payto']),
            'align' => 'center',
          ),
          array(
            'data' => drupal_render($form['types'][$element]['allow_recurring']),
            'align' => 'center',
          ),
          array(
            'data' => drupal_render($form['types'][$element]['weight']),
            'align' => 'center',
          ),
        ),
        'class' => array('draggable'),
      );
      if (!empty($form['types'][$element]['gateway_requirements'])) {
        $rows[] = array(
          'data' => array(
            array('data' => ''),
            array(
              'data' => drupal_render($form['types'][$element]['gateway_requirements']),
              'colspan' => '8',
            ),
          ),
          'class' => array('draggable'),
        );
      }
    }
  }
  /*
   else {
   $rows[] = array(
   'data' => array('data' => t('No payment gateways have been enabled'),
   'colspan' => 7, 'align' => 'center'),
   'class' => array('draggable'),
   );
   } */
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'ec-admin-rtypes')));
  $output .= drupal_render_children($form);
  drupal_add_tabledrag('ec-admin-rtypes', 'order', 'sibling', 'receipt-weights');
  return $output;
}

/**
 * @ingroup themeable
 */
function theme_ec_receipt_icon($variables) {
  $icon = $variables['icon'];
  $icon['attributes'] = empty($icon['attributes']) ? array() : $icon['attributes'];
  $icon['attributes'] += array(
    'align' => 'left',
  );
  $icon['attributes']['class'][] = 'ec-receipt-icon';
  return '<img ' . (!empty($icon['src']) ? ' src="' . base_path() . $icon['src'] . '"' : '') . (!empty($icon['attributes']) ? ' ' . drupal_attributes($icon['attributes']) : '') . ' />';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_ec_receipt_review_form($variables) {
  $form = $variables['form'];
  $content = '';
  if (!empty($form['ec_receipt_name']['#markup'])) {

    $content .= drupal_render_children($form);

    return theme('ec_box', array('title' => t('Payment details'), 'content' => $content, 'attr' => array('id' => 'ec-receipt-form')));
  }
}
