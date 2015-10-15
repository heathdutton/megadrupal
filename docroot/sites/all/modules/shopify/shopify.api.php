<?php
/**
 * @file
 * Contains examples and information regarding hooks.
 */

/**
 * Hook into any Shopify webhook firing.
 *
 * @param string $webhook
 *   Name of the Shopify webhook.
 * @param array $payload
 *   Data sent through the webhook.
 */
function hook_shopify_webhook($webhook, $payload) {
  switch ($webhook) {
    case 'shop_update':
      // Update shop info on the site.
      break;
  }
}

/**
 * Hook into a specific webhook that fires.
 *
 * Valid WEBHOOK values are:
 * orders_create, orders_delete, orders_updated, orders_paid, orders_cancelled,
 * orders_fulfilled, orders_partially_fulfilled, order_transactions_create,
 * carts_create, carts_update, checkouts_create, checkouts_update,
 * checkouts_delete, refunds_create, products_create, products_update,
 * products_delete, collections_create, collections_update, collections_delete,
 * customer_groups_create, customer_groups_update, customer_groups_delete,
 * customers_create, customers_enable, customers_disable, customers_update,
 * customers_delete, fulfillments_create, fulfillments_update, shop_update,
 * disputes_create, disputes_update, app_uninstalled
 *
 * @param array $payload
 *   Data sent through the webhook.
 */
function hook_shopify_webhook_WEBHOOK($payload) {
  // React on specific webhook values.
}
