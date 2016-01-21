<?php

/**
 * @file
 * This layout is designed to be the site template layout when using
 * the Panels Everywhere module.
 */
?>
<div<?php print $css_id ? " id=\"$css_id\"" : ''; ?> class="page-wrapper">

  <?php if (!empty($content['branding'])): ?>
    <div class="branding">
      <?php print render($content['branding']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($content['branding_left'])): ?>
    <div class="branding-left">
      <?php print render($content['branding_left']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($content['branding_right'])): ?>
    <div class="branding-right">
      <?php print render($content['branding_right']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($content['nav'])): ?>
    <div class="page-nav">
      <?php print render($content['nav']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($content['main'])): ?>
    <div class="page-body">
      <?php print render($content['main']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($content['footer'])): ?>
    <div class="page-closure">
      <?php print render($content['footer']); ?>
    </div>
  <?php endif; ?>

</div>
