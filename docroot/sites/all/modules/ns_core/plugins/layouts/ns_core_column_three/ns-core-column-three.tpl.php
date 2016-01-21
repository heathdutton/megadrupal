<?php

/**
 * @file
 * This layout is intended to be used inside the page content pane. Thats why
 * there is not wrapper div by default.
 */
?>

<?php if (!empty($content['header_alpha'])): ?>
  <div class="page-header-alpha">
      <?php print render($content['header_alpha']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['header_beta'])): ?>
  <div class="page-header-beta">
    <?php print render($content['header_beta']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['main'])): ?>
  <div class="page-main">
    <?php print render($content['main']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['aside_alpha'])): ?>
  <div class="page-aside-alpha">
    <?php print render($content['aside_alpha']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['footer_alpha'])): ?>
  <div class="page-footer-alpha grid-36 alpha omega">    
    <?php print render($content['footer_alpha']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['aside_beta'])): ?>
<div class="page-aside-beta">
  <?php print render($content['aside_beta']); ?>
</div>
<?php endif; ?>

<?php if (!empty($content['footer_beta'])): ?>
  <div class="page-footer-beta">
      <?php print render($content['footer_beta']); ?>
  </div>
<?php endif; ?>
