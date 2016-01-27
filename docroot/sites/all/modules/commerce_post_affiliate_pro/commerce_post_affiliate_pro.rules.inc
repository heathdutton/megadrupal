<?php
/*
 * @file commerce_post_affiliate_pro.rules.inc
 * Provides rules actions/integration
 * @copyright Copyright(c) 2011 Rowlands Group
 * @license GPL v3 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlandsgroup dot com
 * 
 */


/**
 * Implements hook_rules_action_info();
*/
function commerce_post_affiliate_pro_rules_action_info() {
  $actions = array();
  $actions['commerce_post_affiliate_pro_sales_notify'] = array(
    'label' => t('Notify the PAP server of a sale.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to notify'),
      ),
    ),
    'group' => t('Commerce Order'),
  );
  return $actions;
}

/**
 * Rules action: Notifies PAP of a sale
 */
function commerce_post_affiliate_pro_sales_notify($commerce_order) {
  $account_id = variable_get('commerce_post_affiliate_pro_merchant_id', NULL);
  if (!$account_id) {
    watchdog('commerce_post_affiliate_pro', 'Could not register sale for order @order, merchant id is not set, please visit admin pages.', array('@order' => $commerce_order->order_id));
    return;
  }
  $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
  $amount = $order_wrapper->commerce_order_total->value();

  $path = libraries_get_path('pap');
	require_once($path . '/PapApi.class.php');
	module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');

  $sale_tracker = new Pap_Api_SaleTracker(variable_get('commerce_post_affiliate_pro_merchant_url_prefix', 'http://demo.qualityunit.com/pax4/'). "scripts/sale.php");
  
  $sale_tracker->setAccountId($account_id);
  
  $sale = $sale_tracker->createSale();
  $sale->setTotalCost($amount['amount'] / 100);
  $sale->setOrderID($commerce_order->order_id);
  
  $result = $sale_tracker->register();
  watchdog('commerce_post_affiliate_pro', 'Result: sale registration for order @order: @result', array('@order' => $commerce_order->order_id, '@result' => $result));
}