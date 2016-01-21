<?php

/**
 * @file
 * This layout is intended to be used inside the page content pane. Thats why
 * there is not wrapper div by default.
 */
?>

<?php if (!empty($content['header_alpha'])): ?>
  <div class="page-header">
    <?php print render($content['header_alpha']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['main'])): ?>
  <div class="page-main">
    <?php print render($content['main']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['aside_beta'])): ?>
  <div class="page-aside">
    <?php print render($content['aside_beta']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($content['footer_alpha'])): ?>
  <div class="page-footer">
    <?php print render($content['footer_alpha']); ?>
  </div>
<?php endif; ?>
