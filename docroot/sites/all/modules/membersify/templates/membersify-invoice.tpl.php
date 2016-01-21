<?php
/**
 * @file
 * Template file for the subscription item.
 *
 * Available variables:
 *
 * @var Membersify_Subscription $subscription: The subscription object, including:
 *    -status: The status of the subscription.
 *    -expiration: The expiration timestamp.
 *    -start_date: The start date timestamp.
 *    -current_payments: The current number of payments made.
 *    -next_payment: The next payment timestamp.
 *    -total_occurrences: The total number of payments expected, or 0 for unlimited.
 * @var Membersify_HistoryItem $history_item: The history item object, including:
 *    -type: The type of history item.
 *    -created: When the history item took place.
 *    -amount: The amount, if applicable.
 *    -currency: The currency, if applicable.
 *    -txn_id: The transaction id.
 * @var $account: The related user account associated with the subscription.
 */

?>
<!-- membersify-invoice template -->
<div class="membersify-invoice membersify_button membersify-invoice-<?php print $history_item->type; ?>">

  <div class='membersify-invoice-created'>
    <?php print t("Date: @val", array('@val' => format_date($history_item->created, 'short'))); ?>
  </div>

  <div class='membersify-invoice-type'>
    <?php print t("Type: @val", array('@val' => membersify_get_history_item_type($history_item->type))); ?>
  </div>

  <div class='membersify-invoice-Amount'>
    <?php print t("Amount: @val", array('@val' => membersify_format_money($history_item->amount, $history_item->currency))); ?>
  </div>

  <div class='membersify-invoice-txn-id'>
    <?php print t("Transaction ID: @val", array('@val' => $history_item->txn_id)); ?>
  </div>

</div>
<!-- /membersify-invoice template -->
