<?php
/**
 * @file
 * Template commerce_otp-label-icons.tpl.php.
 *
 * Default template implementation to display
 * icons for Commerce OTP Payment label.
 *
 * Available variables:
 * - $icons['cards']: An array of card icons
 * - $icons['verifications']: An array of verifications icons
 */
?>
<div class="commerce-otp-icons">
  <span class="label"><?php echo t('Accepted cards:'); ?></span>
  <span class="cards"><?php echo implode(' ', $icons['cards']); ?></span>
  <span class="verifications"><?php echo implode(' ', $icons['verifications']); ?></span>
</div>
