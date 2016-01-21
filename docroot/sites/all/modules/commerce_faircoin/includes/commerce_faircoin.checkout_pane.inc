<?php
/**
 * @file
 * Setup the checkout pane that shows the faircoin address.
 */

/**
 * Implements base_checkout_form()
 */
function commerce_faircoin_pane_checkout_form($form, $form_state, $checkout_pane, $order) {
  if ($faircoin_transaction = db_query("SELECT * FROM {commerce_faircoin} WHERE order_id = :id", array(':id' => $order->order_number))->fetchAssoc()) {
    if (($library = libraries_load('qrcode')) && !empty($library['loaded'])) {
      $total_amount = commerce_currency_amount_to_decimal($order->commerce_order_total[LANGUAGE_NONE]['0']['amount'], 'FAC');
      $currency_code = $order->commerce_order_total[LANGUAGE_NONE]['0']['currency_code'];
      $text_qrcode = 'faircoin:' . $faircoin_transaction['faircoin_address'];
      if ($currency_code == 'FAC') {
        $text_qrcode .= '?amount=' . $total_amount;
        $currency_code = 'FAIR';
      }
      $text_qrcode .= '&label=Order' . urlencode(trim($order->order_id));
      // Attach js file and pass $text_code to js.
      drupal_add_js(drupal_get_path('module', 'commerce_faircoin') . '/commerce_faircoin.js');
      drupal_add_js(array('commerce_faircoin' => array('text_qrcode' => $text_qrcode)), 'setting');
    }
    drupal_add_css(drupal_get_path('module', 'commerce_faircoin') . '/css/commerce_faircoin.css');
    $output = array(
      'faircoin-address-qrcode-intro' => array(
        '#type' => 'html_tag',
        '#tag' => 'span',
        '#sufix' => '<br />',
        '#attributes' => array(
          'class' => 'faircoin-address-qrcode-intro',
        ),
        '#value' => t('Please, make a payment of %amount to this FairCoin address:', array('%amount' => $total_amount . ' ' . $currency_code)),
      ),
      'faircoin-address-qrcode-text' => array(
        '#type' => 'html_tag',
        '#tag' => 'code',
        '#attributes' => array(
          'class' => 'faircoin-address-qrcode-text',
        ),
        '#value' => $faircoin_transaction['faircoin_address'],
      ),
      'faircoin-address-qrcode-image' => array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#attributes' => array(
          'class' => 'faircoin-address-qrcode-wrapper',
        ),
        '#value' => '<br /><span>' . t('You can scan this QR code with your wallet to make the payment:') .
          '</span><br /><div class="faircoin-address-qrcode-image"></div>',
      ),
    );
    $output = drupal_render($output);
    $checkout_form['commerce_faircoin_pane_field_display'] = array(
      '#markup' => $output
    );
    return $checkout_form;
  }
}
