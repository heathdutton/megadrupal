<?php
/**
 * @file
 * Template file for splash_offer entities
 *
 * @see splash_offer_preprocess_entity() for variable names
 *
 * @ingroup splash_offer
 * @{
 */
?>
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="content">
    <div class="body"><?php print render($content) ?></div>
    <?php if ($button_mode == SPLASH_OFFER_BUTTON_MODE_HTML) :?>
      <div class="buttons"><?php print render($buttons) ?></div>
    <?php endif; ?>
  </div>
  <?php if ($cookies) :?>
    <div class="cookie-alert">
      <input type="checkbox" id="splash-offer-set-cookie" value="1"<?php print $cookie_default ? ' checked="checked"' : '' ?>" />
      <label>Don't show again</label>
    </div>
  <?php endif; ?>
</div>
