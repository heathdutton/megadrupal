<?php
/**
 * @file
 * Template file for the subscription item.
 *
 * Available variables:
 *
 * @var $items: An array of items ready for payment. Each item array has the following fields:
 *    -name: The name of the item.
 *    -description: The description of the item.
 *    -payment_plan: The payment plan array, including the following fields:
 *      -main_unit: The main unit.
 *      -main_amount: The main amount.
 *      -main_length: The number of units in the main period.
 *      -trial_unit: The trial unit.
 *      -trial_amount: The trial amount.
 *      -trial_length: The number of units in the trial period.
 *      -total_occurrences: How many times the payment recurs.
 *      -has_trial: Whether or not there is a trial period.
 *      -recurring: Whether or not the payment is recurring.
 *      -currency: The currency code.
 *    -price_string: The rendered price string.
 * @var $adjustments: An array of adjustments that are applied to the order. Each item has the following fields:
 *    -name: The name of the adjustment.
 *    -description: The description of the adjustment.
 *    -price_string: The total amount of adjustment (whether positive or negative).
 * @var $total: The total amount due today.
 */

?>

<!-- membersify-checkout-summary template -->
<div class="membersify_checkout_summary">
  <table>
    <thead>
      <tr>
        <th class="item_header"><?php print t("Item"); ?></th>
        <th class="line_total_header"><?php print t("Line Total"); ?></th>
      </tr>
    </thead>

    <tbody>
      <?php foreach ($items as $item) { ?>
        <tr class="membersify_item">
          <td><div class='membersify_name'><?php print $item['name']; ?></div><div class='membersify_description'><?php print $item['description']; ?></div></td>
          <td><div class='membersify_price'><?php print $item['price_string']; ?></div></td>
        </tr>
      <?php } ?>

      <?php foreach ($adjustments as $adjustment) { ?>
        <tr class="membersify_adjustment">
          <td><div class='membersify_name'><?php print $adjustment->display; ?></div></td>
          <td><div class='membersify_price'><?php print membersify_format_money($adjustment->amount); ?></div></td>
        </tr>
      <?php } ?>

      <tr class="membersify_total">
        <td><div class='membersify_name'><?php print t("Due today"); ?></div></td>
        <td><div class='membersify_price'><?php print $total; ?></div></td>
      </tr>
    </tbody>
  </table>
</div>
<!-- /membersify-checkout-summary template -->
