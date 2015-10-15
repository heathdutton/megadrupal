<?php
/**
 * @file
 * Template file for the membership item.
 *
 * Available variables:
 *
 * @var $membership: The membership object, including:
 *    -status: The status of the membership.
 *    -expiration: The expiration timestamp.
 *    -start_date: The start date timestamp.
 *    -current_payments: The current number of payments made.
 *    -next_payment: The next payment timestamp.
 *    -total_occurrences: The total number of payments expected, or 0 for unlimited.
 * @var $plan: The plan object.
 * @var $change_billing_url: The url for the page where the user can change their billing info.
 * @var $change_plan_url: The url for the page where the user can change their plan.
 * @var $cancel_url: The url for the page where the user can cancel their membership.
 * @var $renew_url: The url for the page where the user can renew their membership.
 * @var $history_url: The url for the page where the user can view their payments history for the membership.
 */

?>
<!-- membersify-membership template -->
<div class="membersify-membership membersify_button membersify-membership-<?php print $membership->status; ?>">

  <div class='membersify-membership-actions'>
    <?php if ($change_billing_url) { ?>
      <a class='membersify-membership-change-billing-link membersify_button' href='<?php print $change_billing_url; ?>'><?php print t("Change billing details"); ?></a>
    <?php } ?>

    <?php if ($cancel_url) { ?>
      <a class='membersify-membership-cancel-link membersify_button' href='<?php print $cancel_url; ?>'><?php print t("Cancel payments"); ?></a>
    <?php } ?>

    <?php if ($renew_url) { ?>
      <a class='membersify-membership-renew-link membersify_button' href='<?php print $renew_url; ?>'><?php print t("Reactivate"); ?></a>
    <?php } ?>

    <?php if ($change_plan_url) { ?>
      <a class='membersify-membership-change-plan-link membersify_button' href='<?php print $change_plan_url; ?>'><?php print t("Change plan"); ?></a>
    <?php } ?>

    <?php if ($history_url) { ?>
      <a class='membersify-membership-history-link membersify_button' href='<?php print $history_url; ?>'><?php print t("View history"); ?></a>
    <?php } ?>
  </div>

  <div class='membersify-membership-plan-name'>
    <?php print t("Plan: @plan", array('@plan' => $plan->name)); ?>
  </div>

  <div class='membersify-membership-status'>
    <?php print t("Status: @status", array('@status' => membersify_get_membership_status($membership->status))); ?>
  </div>

  <div class='membersify-membership-start-date'>
    <?php print t("Start date: @start", array('@start' => format_date($membership->start_date, 'short'))); ?>
  </div>

  <?php if ($membership->expiration) { ?>
    <div class='membersify-membership-expiration-date'>
      <?php print t("Expires: @expires", array('@expires' => format_date($membership->expiration, 'short'))); ?>
    </div>
  <?php } ?>

  <div class='membersify-membership-number-payments'>
    <?php print t("Payments so far: @current / @total",
      array(
        '@current' => $membership->current_payments,
        '@total' => $membership->total_occurrences ? $membership->total_occurrences : t('Unlimited'),
      )); ?>
  </div>

  <?php if ($membership->next_payment) { ?>
    <div class='membersify-membership-next-payment-date'>
      <?php print t("Next payment: @next", array('@next' => format_date($membership->next_payment, 'short'))); ?>
    </div>
  <?php } ?>

</div>
<!-- /membersify-membership template -->
