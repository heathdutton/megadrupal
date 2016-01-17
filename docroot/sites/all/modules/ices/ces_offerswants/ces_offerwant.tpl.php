<?php
/**
 * @file
 * Template for rendering offers and wants.
 */

/**
 * @defgroup ces_import4ces_templates Templates from OffersWants
 * @ingroup ces_offerswants
 * @{
 * Template for rendering offers and wants.
 */
?>
<div class="ces-offerwant">
  <div class="ces-offerwant-display" >
    <div class="ces-offerwant-send_notify"><?php echo $ces_offerwant_send_notify_link; ?></div>
    <div class="ces-offerwant-title"><?php echo $ces_offerwant_title; ?></div>
    <div class="ces-offerwant-content">
      <div class="ces-offerwant-image"><?php echo $ces_offerwant_image; ?></div>
      <div class="ces-offerwant-body"><?php echo $ces_offerwant_body; ?></div>
      <?php if (isset($ces_offer_rate)): ?>
      <div class="ces-offerwant-rate"><?php echo $ces_offer_rate ?></div>
      <?php endif; ?>
    </div>
  </div>
  <?php if ($view_mode_full): ?>
  <div class="ces-offerwant-properties">
    <dl class="ces-offerwant-properties-list">
      <?php if (!empty($ces_offerwant_category)): ?>
      <dt><?php echo t('Category'); ?></dt>
      <dd><?php echo $ces_offerwant_category; ?></dd>
      <?php endif; ?>
      <dt><?php echo t('Status'); ?></dt>
      <dd><?php echo $ces_offerwant_state; ?></dd>
      <dt><?php echo t('Updated'); ?></dt>
      <dd><?php echo $ces_offerwant_modified; ?></dd>
      <dt><?php echo t('Expire'); ?></dt>
      <dd><?php echo $ces_offerwant_expire; ?></dd>
      <dt><?php echo t('Keywords'); ?></dt>
      <dd><?php echo $ces_offerwant_keywords; ?></dd>
    </dl>
  </div>
  <?php endif; ?>
  <div class="ces-offerwant-footer">
    <div class="ces-offerwant-seller">
      <?php global $base_path; ?>
      <div class="ces-offerwant-seller-picture">
        <a href="<?php print $base_path; ?>user/<?php echo $ces_offerwant_seller_uid; ?>"  title="<?php echo t("Seller's info"); ?>">
      <?php print theme('user_picture', array('account' => user_load($ces_offerwant_seller_uid))); ?>
        </a>
      </div>
      <div class="ces-offerwant-seller-name-and-phone">
        <div class="ces-offerwant-seller-name">
          <a href="<?php print $base_path; ?>user/<?php echo $ces_offerwant_seller_uid; ?>" title="<?php echo t("Seller's info"); ?>">
          <?php echo $ces_offerwant_seller_name; ?>
          </a>
        </div>
        <div class="ces-offerwant-seller-phone">
            <?php echo $ces_offerwant_seller_phone; ?>
        </div>
        <?php if ($view_mode_full && $ces_offerwant_belongs_to_me == FALSE) : ?>
            <div class="ces-offerwant-seller-email">
            <?php echo $ces_offerwant_seller_mail; ?>
            </div>
        <?php endif; ?>
            <div class="ces-offerwant-seller-address">
            <?php echo $ces_offerwant_seller_address; ?>
            </div>
            <div class="ces-offerwant-seller-exchange-code">
            <?php echo $ces_exchange_seller_code; ?>
            </div>
      </div>
    </div>
    <div class="ces-offerwant-actions">
      <?php echo $ces_offerwant_actions; ?>
    </div>
    <div class="clearfix"></div>
  </div>
</div>

<?php /** @} */ ?>
