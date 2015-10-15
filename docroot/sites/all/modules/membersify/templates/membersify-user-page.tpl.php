<?php
/**
 * @file
 * Template file for the user subscriptions page.
 *
 * Available variables:
 *
 * @var $account: The user account.
 * @var $subscriptions_html: The rendered subscriptions html.
 * @var $purchase_links: The purchase links for subscriptions, if the user has
 *     permission to view them.
 */
?>
<!-- membersify-user-page template -->

<div class='membersify-user-page'>
  <?php if ($purchase_links) { ?>
    <div class="membersify-purchase-links">
      <h3>
        <?php print t('Available plans'); ?>
      </h3>
      <?php print $purchase_links; ?>
    </div>
  <?php } ?>

  <?php if ($purchase_links) { ?>
    <div class="membersify-subscriptions">
      <h3>
        <?php print t('Current subscription'); ?>
      </h3>
      <?php print $subscriptions_html; ?>
    </div>
  <?php } ?>
</div>
<!-- /membersify-user-page template -->
