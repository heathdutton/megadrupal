<?php

/**
 * @file
 * Themeable function for ec_product module.
 */

/**
 * @ingroup themeable
 */
function theme_ec_product_autocomplete($variables) {
  $product = $variables['product'];
  $output .= "<b>{$product->nid}</b> - " . check_plain($product->title) .
    (isset($product->sku) ? '[<em>' . check_plain($product->sku) . '</em>]' : '');
  return $output;
}

/**
 * @ingroup themeable
 */
function theme_ec_product_edit_form($variables) {
  $form = $variables['form'];
  $output .= drupal_render($form['products']);
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @ingroup themeable
 */
function theme_ec_product_edit_form_product($variables) {
  $form = $variables['form'];
  $children = element_children($form);
  if (!empty($children)) {
    $extra = FALSE;

    foreach ($children as $nid) {
      if (!empty($form[$nid]['data'])) {
        $extra = TRUE;
        break;
      }
    }

    foreach ($children as $nid) {
      $total += $form[$nid]['#total'];
      $desc = drupal_render($form[$nid]['title']) . '<br />';
      if (!empty($form[$nid]['recurring'])) {
        $desc .= '<div class="recurring-details">' . drupal_render($form[$nid]['recurring']) . '</div>';
      }
      if (!empty($form[$nid]['availability'])) {
        $desc .= drupal_render($form[$nid]['availability']);
      }
      $desc .= '<p>' . format_currency($form[$nid]['#total']) . '</p>';
      $row = array(
        array('data' => $desc),
        array('data' => drupal_render($form[$nid]['qty'])),
      );
      if (!empty($extra)) {
        if (!empty($form[$nid]['node'])) {
          $row[] = array('data' => drupal_render($form[$nid]['node']));
          $extra = TRUE;
        }
        else {
          $row[] = '';
        }
      }
      $row[] = array(
        'data' => drupal_render($form[$nid]['delete']),
        'align' => 'center',
      );
      $rows[] = $row;
    }
    $header = array(
      array('data' => t('Items')),
      array('data' => t('Qty')),
    );
    if (!empty($extra)) {
      $header[] = array('data' => '');
    }
    $header[] = array('data' => t('Delete'));
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('No Products Currently on this Invoice') . '</p>';
  }
  $output .= drupal_render_children($form);
  return $output;
}
