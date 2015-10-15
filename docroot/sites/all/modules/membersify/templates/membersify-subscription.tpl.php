<?php
/**
 * @file
 * Template file for the subscription item.
 *
 * Available variables:
 *
 * @var $subscription: The subscription object, including:
 *    -status: The status of the subscription.
 *    -expiration: The expiration timestamp.
 *    -start_date: The start date timestamp.
 *    -current_payments: The current number of payments made.
 *    -next_payment: The next payment timestamp.
 *    -total_occurrences: The total number of payments expected, or 0 for unlimited.
 *    -payment_plan: The payment plan array.
 * @var $plan: The plan object.
 * @var $change_billing_url: The url for the page where the user can change their billing info.
 * @var $change_plan_url: The url for the page where the user can change their plan.
 * @var $cancel_url: The url for the page where the user can cancel their subscription.
 * @var $renew_url: The url for the page where the user can renew their subscription.
 * @var $history_url: The url for the page where the user can view their payments history for the subscription.
 */

?>
<!-- membersify-subscription template -->
<div class="membersify-subscription membersify_button membersify-subscription-<?php print $subscription->status; ?>">

  <div class='membersify-subscription-actions'>
    <?php if ($change_billing_url) { ?>
      <a class='membersify-subscription-change-billing-link membersify_button' href='<?php print $change_billing_url; ?>'><?php print t("Change billing details"); ?></a>
    <?php } ?>

    <?php if ($cancel_url) { ?>
      <a class='membersify-subscription-cancel-link membersify_button' href='<?php print $cancel_url; ?>'><?php print t("Cancel payments"); ?></a>
    <?php } ?>

    <?php if ($renew_url) { ?>
      <a class='membersify-subscription-renew-link membersify_button' href='<?php print $renew_url; ?>'><?php print t("Reactivate"); ?></a>
    <?php } ?>

    <?php if ($change_plan_url) { ?>
      <a class='membersify-subscription-change-plan-link membersify_button' href='<?php print $change_plan_url; ?>'><?php print t("Change plan"); ?></a>
    <?php } ?>

    <?php if ($history_url) { ?>
      <a class='membersify-subscription-history-link membersify_button' href='<?php print $history_url; ?>'><?php print t("View history"); ?></a>
    <?php } ?>
  </div>

  <div class='membersify-subscription-plan-name'>
    <?php print t("Plan: @plan", array('@plan' => $plan->name)); ?>
  </div>

  <div class='membersify-subscription-status'>
    <?php print t("Status: @status", array('@status' => membersify_get_subscription_status($subscription->status))); ?>
  </div>

  <div class='membersify-subscription-start-date'>
    <?php print t("Start date: @start", array('@start' => format_date($subscription->start_date, 'short'))); ?>
  </div>

  <?php if ($subscription->expiration) { ?>
    <div class='membersify-subscription-expiration-date'>
      <?php print t("Expires: @expires", array('@expires' => format_date($subscription->expiration, 'short'))); ?>
    </div>
  <?php } ?>

  <div class='membersify-subscription-number-payments'>
    <?php print t("Payments so far: @current / @total",
      array(
        '@current' => $subscription->current_payments,
        '@total' => $subscription->total_occurrences ? $subscription->total_occurrences : t('Unlimited'),
      )); ?>
  </div>

  <?php if ($subscription->next_payment) { ?>
    <div class='membersify-subscription-next-payment-date'>
      <?php print t("Next payment: @next", array('@next' => format_date($subscription->next_payment, 'short'))); ?>
    </div>
  <?php } ?>

</div>
<!-- /membersify-subscription template -->
