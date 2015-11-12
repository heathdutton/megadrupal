<?php

/**
 * @file
 * Page callbacks for Commerce Cobros en Línea.
 */

/**
 * Page callback to simulate payment responses for Cobros en Línea local mode.
 */
function commerce_cel_local($order) {
  $data = commerce_cel_local_submitted_data($_POST);
  $verif = isset($_POST['rccpagosVerificacion']) ? $_POST['rccpagosVerificacion'] : FALSE;
  $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
  $checksum = commerce_cel_checksum($data, $payment_method);

  if ($verif !== FALSE && $verif == $checksum) {
    $output['description'] = array(
      '#markup' => t('Use the links below to simulate the return urls the user would get after completing the payment in Cobros en Línea.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
    $output['links'] = array(
      '#theme' => 'item_list',
      '#items' => commerce_cel_local_response_links($data, $checksum, $payment_method),
      '#type' => 'ul',
    );
  }
  else {
    $output['description'] = array(
      '#markup' => t('Invalid checksum.'),
    );
  }

  return drupal_render($output);
}

/**
 * Cobros en Línea local mode: get relevant submitted data.
 */
function commerce_cel_local_submitted_data($raw_input) {
  $keys = array(
    'rccpagosEmpresa',
    'rccpagosImporte',
    'rccpagosMoneda',
    'rccpagosOrden',
    'rccpagosURLError',
    'rccpagosURLOk',
    'rccpagosUsuario',
    'rccpagosMedioDePago',
    'rccpagosVencimiento',
    'rccpagosCorreoElectronico',
    'rccpagosCuotas',
    'rccpagosPromocion',
  );

  foreach ($keys as $key) {
    if (isset($raw_input[$key])) {
      $data[$key] = $raw_input[$key];
    }
  }

  return $data;
}

/**
 * Cobros en Línea local mode: generate response links.
 *
 * Builds a list of valid redirection links to simulate possible responses.
 */
function commerce_cel_local_response_links($data, $checksum, $payment_method) {
  module_load_include('inc', 'commerce_cel', 'includes/commerce_cel.pages');
  $verif = strtolower(md5($checksum . $payment_method['settings']['secret_key']));

  $url_ok = urldecode($data['rccpagosURLOk']);
  $url_ok = str_replace('###VERIF###', $verif, $url_ok);
  $links[] = l(t('Successful payment'), $url_ok, array('absolute' => TRUE));

  $url_failed = urldecode($data['rccpagosURLError']);
  $url_failed = str_replace('###VERIF###', $verif, $url_failed);

  foreach (array('3', '4', '5', '6', '7', '8') as $error) {
    $url = str_replace('###ERROR###', $error, $url_failed);
    $text = t('Failed payment') . ': ' . commerce_cel_error_message($error);
    $links[] = l($text, $url, array('absolute' => TRUE));
  }
  return $links;
}
