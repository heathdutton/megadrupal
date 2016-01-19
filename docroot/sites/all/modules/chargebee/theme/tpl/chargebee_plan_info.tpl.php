<?php
/**
 * @file
 * ChargeBee module plan info template.
 *
 * Available variables:
 * - $plan - full plan object.
 */
?>

<div class="chargebee-plan-info">
  <?php if (is_object($plan)): ?>
    <div class="chargebee-plan-table"><?php print $plan_table; ?></div>
    <div class="chargebee-plan-description"><?php print $plan->description; ?></div>
  <?php endif; ?>
</div>
