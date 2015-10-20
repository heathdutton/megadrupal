<?php
/**
 * @file
 * ChargeBee module subscription info template.
 *
 * Available variables:
 * - $base_path - drupal base path.
 * - $user - drupal user.
 * - $subscription - subscription array.
 * - $customer - customer array.
 * - $card - card array.
 */
?>

<div class="chargebee-info">
  <?php if ($subscription): ?>
    <div class="chargebee-subscription-info">
      <div class="chargebee-info-header">
        <h2><?php print t('Subscription'); ?></h2>
        <div class="chargebee-subscription-status <?php print $subscription['status']; ?>"><?php print ucfirst($subscription['status']); ?></div>
        <a href="<?php print $base_path . 'user/' . $user->uid . '/chargebee/subscription'  ?>">Edit</a>
      </div>
      <div class="chargebee-subscription-next">
        <p><?php print t('Your next billing is on @date', array('@date' => $subscription['date'])); ?></p>
      </div>
      <div class="chargebee-subscription-plan">
        <span class="chargebee-subscription-plan-name"><?php print $subscription['plan_name']; ?></span>
        <span class="chargebee-subscription-plan-cost"><?php print '$' . $subscription['plan_cost']; ?></span>
        <div class="chargebee-subscription-plan-period">
          <?php print $subscription['cost_message']; ?>
        </div>
      </div>
    </div>
  <?php endif; ?>
</div>
