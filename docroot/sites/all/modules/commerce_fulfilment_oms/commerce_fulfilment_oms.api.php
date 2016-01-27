<?php

/**
 * @file Document the API
 */


/**
 * Implements fulfilment_sendorder().
 *
 * @var $order The wrapped commerce order
 * @var $parameters The SalesOrder object containing the parameters
 */
function hook_fulfilment_sendorder(EntityDrupalWrapper $order, SalesOrder $parameters) {

  // Set the shipping method
  $shipaddr = $order->commerce_customer_shipping->commerce_customer_address;
  $country = $shipaddr->country->value();
  $postcode = $shipaddr->postal_code->value();
  switch ($country) {
    case 'AU':
      $method = 'AUSPOST';
      break;

    case 'NZ':
      $method = 'NZPOST';
      break;

    default:
      $method = 'FEDEX';
      break;
  }
  $parameters->ShipMethod = $method;


  // Set the order priority (for priority handling / express)
  $shipping_lineitem = commerce_fulfilment_get_shipping_lineitem($order_wrapper);
  if ($shipping_lineitem
      && $shipping_lineitem->commerce_shipping_service->value() === 'express') {
    $parameters->order->Priority = 'High';
  }

  // Set the order comments
  $packcomment = new stdClass;
  $packcomment->Comment = $order_wrapper->field_order_instructions->value();
  $packcomment->CommentType = 'Private';

  $deliverycomment = new stdClass;
  $deliverycomment->Comment = $order_wrapper->field_order_deliveryinstructions->value();
  $deliverycomment->CommentType = 'Public';

  $parameters->order->OrderComments = array($packcomment, $deliverycomment);
}
