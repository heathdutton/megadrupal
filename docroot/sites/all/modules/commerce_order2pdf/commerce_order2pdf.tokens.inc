<?php

/**
 * Implements hook_token_info().
 */
function commerce_order2pdf_token_info() {
  $order = array();
  $order['hashed_pdf_link'] = array(
    'name' => t('Hashed link to pdf'),
    'description' => t("Hashed link for pdf generation."),
  );
  return array(
    'tokens' => array('commerce-order' => $order),
  );
}

/**
 * Implements hook_tokens().
 */
function commerce_order2pdf_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
 if ($type == 'commerce-order' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'hashed_pdf_link':
        $hash = hash('md5', $order->order_id . $order->created);
        $replacements[$original] = url("order2pdf/{$order->order_id}/" . $hash, array('absolute' => TRUE));
        break;
      }
    }
  }
  return $replacements;
}
