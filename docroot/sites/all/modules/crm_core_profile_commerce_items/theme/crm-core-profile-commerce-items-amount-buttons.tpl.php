<?php

/**
 * @file
 * Template crm-core-profile-commerce-items-amount-buttons.
 *
 * Available variables:
 * - $amounts: An array of recommended numeric amounts for buttons.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?><div class="amounts">
<?php foreach ($amounts as $amount):?>
  <input type="button" amount="<?php print $amount;?>" value="<?php print _crm_core_profile_commerce_items_currency_format($amount);?>" class="amount">
<?php endforeach; ?>
</div>
