<?php
/**
 * @file
 * Template file for the thank you page.
 *
 * Available variables:
 *
 * @var $account: The user account.
 * @var $subscription: The subscription object.
 * @var $history_item: The history item object.
 */
?>
<!-- membersify-thankyou-page template -->
<div class="membersify_thankyou_page">
  <div class='membersify_thankyou_text'>
    <?php print t("Thank you! Your subscription was successful and is now active."); ?>
  </div>

  <?php if ($subscription->next_payment) { ?>
    <div class='membersify_next_payment'>
      <?php
        print t("You will be billed @amount on @date to continue your subscription.",
          array(
            '@amount' => membersify_format_money($subscription->payment_plan['main_amount'], $subscription->payment_plan['currency']),
            '@date' => format_date($subscription->next_payment, 'medium')
          ));
      ?>
    </div>
  <?php } ?>

  <?php if ($subscription->expiration) { ?>
    <div class='membersify_expiration'>
      <?php
        print t("Your subscription will expire on @date.",
          array('@date' => format_date($subscription->expiration, 'medium')));
      ?>
    </div>
  <?php } ?>
</div>
<!-- /membersify-thankyou-page template -->
