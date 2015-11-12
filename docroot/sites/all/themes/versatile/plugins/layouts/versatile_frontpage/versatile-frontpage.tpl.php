<div<?php print $css_id ? " id=\"$css_id\"" : ''; ?> class="layout-frontpage clearfix">
  <?php if (!empty($content['main_alpha'])): ?>
  <div class="column column-main column-alpha">
    <?php print render($content['main_alpha']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['column_first_alpha'])): ?>
  <div class="column column-first column-alpha">
    <?php print render($content['column_first_alpha']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['column_second_alpha'])): ?>
  <div class="column column-second column-alpha">
    <?php print render($content['column_second_alpha']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['main_beta'])): ?>
  <div class="column column-main column-beta">
    <?php print render($content['main_beta']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['column_first_beta'])): ?>
  <div class="column column-first column-beta">
    <?php print render($content['column_first_beta']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['column_second_beta'])): ?>
  <div class="column column-second column-beta">
    <?php print render($content['column_second_beta']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['column_third_beta'])): ?>
  <div class="column column-third column-beta">
    <?php print render($content['column_third_beta']); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($content['main_gamma'])): ?>
  <div class="column column-main column-gamma">
    <?php print render($content['main_gamma']); ?>
  </div>
  <?php endif; ?>
</div>
