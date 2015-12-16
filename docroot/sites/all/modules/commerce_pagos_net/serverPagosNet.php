<?php

// log!
openlog('pagos_net', LOG_PID | LOG_ODELAY, LOG_LOCAL1);
syslog(LOG_NOTICE, 'serverPagosNet accessed at ' . $_SERVER['REQUEST_URI']);
closelog();

define('DRUPAL_ROOT',  getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/update.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';
require_once DRUPAL_ROOT . '/includes/file.inc';
require_once DRUPAL_ROOT . '/includes/entity.inc';
require_once DRUPAL_ROOT . '/includes/unicode.inc';
require_once DRUPAL_ROOT . '/includes/errors.inc';
require_once DRUPAL_ROOT . '/includes/language.inc';
require_once DRUPAL_ROOT . '/includes/locale.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
module_load_all();

// Pull in the NuSOAP code
libraries_load('nusoap');
// Create the server instance
$server = new soap_server();
// Initialize WSDL support
$server->configureWSDL('wsComelecServer', 'urn:wsComelecServer');
// Register the data structures used by the service
$server->wsdl->addComplexType(
  'WsTransaccion',
  'complexType',
  'struct',
  'all',
  '',
  array(
    'CodigoEmpresa' => array('name' => 'CodigoEmpresa', 'type' => 'xsd:int'),
    'CodigoRecaudacion' => array('name' => 'CodigoRecaudacion', 'type' => 'xsd:string'),
    'CodigoProducto' => array('name' => 'CodigoProducto', 'type' => 'xsd:string'),
    'NumeroPago' => array('name' => 'NumeroPago', 'type' => 'xsd:int'),
    'Fecha' => array('name' => 'Fecha', 'type' => 'xsd:int'),
    'Secuencial' => array('name' => 'Secuencial', 'type' => 'xsd:int'),
    'Hora' => array('name' => 'Hora', 'type' => 'xsd:int'),
    'OrigenTransaccion' => array('name' => 'OrigenTransaccion', 'type' => 'xsd:string'),
    'Pais' => array('name' => 'pais', 'type' => 'xsd:int'),
    'Departamento' => array('name' => 'Departamento', 'type' => 'xsd:int'),
    'Ciudad' => array('name' => 'Ciudad', 'type' => 'xsd:int'),
    'Entidad' => array('name' => 'Entidad', 'type' => 'xsd:string'),
    'Agencia' => array('name' => 'Agencia', 'type' => 'xsd:string'),
    'Operador' => array('name' => 'Operador', 'type' => 'xsd:int'),
    'Monto' => array('name' => 'Monto', 'type' => 'xsd:double'),
    'LoteDosificacion' => array('name' => 'LoteDosificacion', 'type' => 'xsd:int', 'minOccurs'=>'0'),
    'NroRentaRecibo' => array('name' => 'NroRentaRecibo', 'type' => 'xsd:string'),
    'MontoCreditoFiscal' => array('name' => 'MontoCreditoFiscal', 'type' => 'xsd:double', 'minOccurs'=>'0'),
    'CodigoAutorizacion' => array('name' => 'CodigoAutorizacion', 'type' => 'xsd:string', 'minOccurs'=>'0'),
    'CodigoControl' => array('name' => 'CodigoControl', 'type' => 'xsd:string', 'minOccurs'=>'0'),
    'NitFacturar' => array('name' => 'NitFacturar', 'type' => 'xsd:string', 'minOccurs'=>'0'),
    'NombreFacturar' => array('name' => 'NombreFacturar', 'type' => 'xsd:string', 'minOccurs'=>'0'),
    'Transaccion' => array('name' => 'Transaccion', 'type' => 'xsd:string')
  )
);
$server->wsdl->addComplexType(
  'RespTransaccion',
  'complexType',
  'struct',
  'all',
  '',
  array(
    'CodError' => array('name' => 'CodError', 'type' => 'xsd:int'),
    'Descripcion' => array('name' => 'Descripcion', 'type' => 'xsd:string')
  )
);
// Register the method to expose
$server->register('datosTransaccion',                    // method name
  array('datos' => 'tns:WsTransaccion', 'user'=> 'xsd:string', 'password' => 'xsd:string'),   // input parameters
  array('return' => 'tns:RespTransaccion'),      // output parameters
  'urn:wsComelecServer',                         // namespace
  'urn:wsComelecServer#datosTransaccion',        // soapaction
  'rpc',                                    // style
  'encoded',                                     // use
  'Aqu&iacute; se describe la documentaci&oacute;n y tipos de error posibles'     // documentation
);

// Define the method as a PHP function
function datosTransaccion($datos, $user, $password) {
  if (($user=='demo')&&($password=='demo')) {
    $retval = array(
      'CodError' => 0,
      'Descripcion' => 'OK datos enviados:'
				. $datos['CodigoEmpresa'] . "|"
				. $datos['CodigoRecaudacion'] . "|"
				. $datos['CodigoProducto'] . "|"
				. $datos['NumeroPago'] . "|"
				. $datos['Fecha'] . "|"
				. $datos['Secuencial'] . "|"
				. $datos['Hora'] . "|"
				. $datos['Pais'] . "|"
				. $datos['Departamento'] . "|"
				. $datos['Ciudad'] . "|"
				. $datos['Entidad'] . "|"
				. $datos['Agencia'] . "|"
				. $datos['Operador'] . "|"
				. $datos['Monto'] . "|"
				. $datos['LoteDosificacion'] . "|"
				. $datos['NroRentaRecibo'] . "|"
				. $datos['MontoCreditoFiscal'] . "|"
				. $datos['CodigoAutorizacion'] . "|"
				. $datos['CodigoControl'] . "|"
				. $datos['NitFacturar'] . "|"
				. $datos['NombreFacturar'] . "|"
				. $datos['Transaccion'] . "|"
    );
  } else {
    $retval = array(
      'CodError' => 99,
      'Descripcion' => 'Usuario o Contraseña erroneos',
    );
	}

  watchdog('pagos_net', '[%time] %data .', array('%time' => date('l jS \of F Y h:i:s A'), '%data' => $retval['Descripcion']));

  if (strpos($datos['CodigoRecaudacion'], '-') === false) {
    // we pad the order number to the last 6 digits of the CodigoRecaudacion
    $order_number = (int) substr($datos['CodigoRecaudacion'], -6);
  } else {
    list($prefix, $order_number) = explode('-', $datos['CodigoRecaudacion']);
  }

  $order = commerce_order_load($order_number);

  if ($order) {
    commerce_pagos_net_transaction($order, $datos['Monto']);
    // Allow other modules to react when the payment is received
    module_invoke_all('commerce_pagos_net_finish_transaction', $datos, $order);
    return $retval; //new soapval('return', 'RespTransaccion', $retval, false, 'urn:wsComelecServer');
  } else {
    watchdog('pagos_net', "Order $order_number not found");
    return array(
      'CodError' => 99,
      'Descripcion' => "Orden $order_number no encontrada",
    );
  }
}

// Use the request to (try to) invoke the service
$HTTP_RAW_POST_DATA = isset($HTTP_RAW_POST_DATA) ? $HTTP_RAW_POST_DATA : '';
$server->service($HTTP_RAW_POST_DATA);

// reference: http://www.scottnichol.com/nusoapprogwsdl.htm

/**
 * Creates a pagosnet payment transaction for the specified charge amount.
 *
 * @param $payment_method
 *   The payment method instance object used to charge this payment.
 * @param $order
 *   The order object the payment applies to.
 * @param $charge
 *   An array indicating the amount and currency code to charge.
 */
function commerce_pagos_net_transaction($order, $charge) {
  $transaction = commerce_payment_transaction_new('pagos_net', $order->order_id);
  $transaction->instance_id = 'pagos_net|commerce_payment_pagos_net';
  $transaction->amount = commerce_currency_decimal_to_amount($charge, 'BOB');
  $transaction->currency_code = 'BOB';
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;

  $transaction->message = 'Payment received.';
  $transaction->message_variables = array();

  $transaction_saved = commerce_payment_transaction_save($transaction);
  watchdog('pagos_net', "Transaction for order {$order->order_id} saved: %saved .", array( '%saved' => $transaction_saved));

  return $transaction_saved;
}
