<?php
/**
 * @file
 * Invoice ChargeBee API functions.
 */

/**
 * Convert ChargeBee invoice object to array.
 *
 * @param object $invoice
 *   ChargeBee invoice object.
 *
 * @return array
 *   Invoice array.
 */
function _chargebee_invoice_to_array($invoice) {
  $invoice_array = array();
  if ($invoice) {
    $attributes = array(
      'customer_id' => 'customerId',
      'subscription_id' => 'subscriptionId',
      'recurring' => 'recurring',
      'status' => 'status',
      'vat_number' => 'vatNumber',
      'start_date' => 'startDate',
      'end_date' => 'endDate',
      'amount' => 'amount',
      'paid_on' => 'paidOn',
      'next_retry' => 'nextRetry',
      'sub_total' => 'subTotal',
      'tax' => 'tax',
    );
    $linked_transactions = isset($invoice->linkedTransactions) ? $invoice->linkedTransactions : NULL;
    $line_items = isset($invoice->lineItems) ? $invoice->lineItems : NULL;
    $discounts = isset($invoice->discounts) ? $invoice->discounts : NULL;
    $taxes = isset($invoice->taxes) ? $invoice->taxes : NULL;
    $invoice_array = _chargebee_object_to_array_convert($invoice, $attributes);
    $invoice_array['linked_transactions'] = $linked_transactions;
    $invoice_array['line_items'] = _chargebee_invoice_line_items_to_array($line_items);
    $invoice_array['discounts'] = _chargebee_invoice_discounts_to_array($discounts);
    $invoice_array['taxes'] = _chargebee_invoice_taxes_to_array($taxes);
  }
  return $invoice_array;
}

/**
 * Convert invoice line items objects to array.
 *
 * @param array $line_items
 *   Invoice Line items objects array.
 *
 * @return array
 *   Converted array.
 */
function _chargebee_invoice_line_items_to_array($line_items) {
  $line_items_array = array();
  if ($line_items) {
    foreach ($line_items as $item) {
      $attributes = array(
        'date_from' => 'dateFrom',
        'date_to' => 'dateTo',
        'unit_amount' => 'unitAmount',
        'quantity' => 'quantity',
        'tax' => 'tax',
        'tax_rate' => 'taxRate',
        'amount' => 'amount',
        'description' => 'description',
        'type' => 'type',
        'entity_type' => 'entityType',
        'entity_id' => 'entityId',
      );
      $line_items_array[] = _chargebee_object_to_array_convert($item, $attributes);
    }
  }
  return $line_items_array;
}

/**
 * Convert invoice discounts objects to array.
 *
 * @param array $discounts
 *   Invoice Discounts objects array.
 *
 * @return array
 *   Converted array.
 */
function _chargebee_invoice_discounts_to_array($discounts) {
  $discounts_array = array();
  if ($discounts) {
    foreach ($discounts as $item) {
      $attributes = array(
        'amount' => 'amount',
        'description' => 'description',
        'type' => 'type',
        'entityId' => 'entityId',
      );
      $discounts_array[] = _chargebee_object_to_array_convert($item, $attributes);
    }
  }
  return $discounts_array;
}

/**
 * Convert invoice taxes objects to array.
 *
 * @param array $taxes
 *   Invoice Taxes objects array.
 *
 * @return array
 *   Converted array.
 */
function _chargebee_invoice_taxes_to_array($taxes) {
  $taxes_array = array();
  if ($taxes) {
    foreach ($taxes as $item) {
      $attributes = array(
        'amount' => 'amount',
        'description' => 'description',
      );
      $taxes_array[] = _chargebee_object_to_array_convert($item, $attributes);
    }
  }
  return $taxes_array;
}
