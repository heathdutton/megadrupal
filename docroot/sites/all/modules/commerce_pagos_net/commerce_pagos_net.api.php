<?php

/**
 * @file
 * Hook documentation for the Pagos Net Commerce module.
 */

/**
 * Act on a transaction that just finished
 *
 * Allows modules to execute an action after a pagos_net transaction finished
 *
 * @param array $params
 *   The params returned from pagos net.
 * @param object $order
 *   The full order object for the transaction.
 *
 * @see serverPagosNet.php
 */
function hook_commerce_pagos_net_finish_transaction($params, $order) {
  if (isset($params['NroRentaRecibo'])) {
    $order->field_nro_factura['und'][0]['value'] = $params['NroRentaRecibo'];
    $order->field_nro_autorizacion['und'][0]['value'] = $params['CodigoAutorizacion'];
    commerce_order_save($order);
  }
}
