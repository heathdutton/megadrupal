<div<?php print $css_id ? " id=\"$css_id\"" : ''; ?> class="layout-frontpage clearfix">
  <?php if (!empty($content['main_alpha'])): ?>
  <section class="column column-main column-alpha clearfix">
    <?php print render($content['main_alpha']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['column_first_alpha'])): ?>
  <section class="column column-first column-alpha clearfix">
    <?php print render($content['column_first_alpha']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['column_second_alpha'])): ?>
  <section class="column column-second column-alpha clearfix">
    <?php print render($content['column_second_alpha']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['main_beta'])): ?>
  <section class="column column-main column-beta clearfix">
    <?php print render($content['main_beta']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['column_first_beta'])): ?>
  <section class="column column-first column-beta clearfix">
    <?php print render($content['column_first_beta']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['column_second_beta'])): ?>
  <section class="column column-second column-beta clearfix">
    <?php print render($content['column_second_beta']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['column_third_beta'])): ?>
  <section class="column column-third column-beta clearfix">
    <?php print render($content['column_third_beta']); ?>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['main_gamma'])): ?>
  <section class="column column-main column-gamma clearfix">
    <?php print render($content['main_gamma']); ?>
  </section>
  <?php endif; ?>
</div>
