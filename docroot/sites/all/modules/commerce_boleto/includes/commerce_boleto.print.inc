<?php
/**
 * @file
 * Print out the Boleto.
 */

/**
 * Callback Function for checkout/%commerce_order/payment/boleto/print.
 */
function commerce_boleto_print($order) {
  if (!empty($order->data['commerce_boleto'])) {
    $output = unserialize($order->data['commerce_boleto']);
    $bank_code = $output['bank_code'];
    // Format the CPF/CNPJ display.
    $display = array('type' => 'number_cnpj_cpf');
    $items = array(
      array('value' => $output['cpf_cnpj'])
    );
    $cpf_cnpj = br_tax_number_fields_field_formatter_view('', '', '', '', '', $items, $display);
    $output['cpf_cnpj'] = $cpf_cnpj[0]['#markup'];

    $boleto = CommerceBoletoLib::load_boleto(array('bank_code' => $bank_code));

    // Give custom modules the opportunity to perform their changes.
    $template_file = '';
    foreach (module_implements('boleto_alter') as $module) {
      $hook = $module . '_boleto_alter';
      $hook($output, $template_file, $order);
    }

    if (!empty($template_file)) {
      $boleto->settingsPropertySetter(array('template' => $template_file));
    }

    $boleto->output(FALSE);
    $boleto->reprintBoleto($output);
  }
  else {
    print t('This order is not payable by Boleto.');
  }
}
