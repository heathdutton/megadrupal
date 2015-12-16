<?php if (isset($header_outside) && $header_outside): ?>

<?php if (!empty($content['header'])): ?>
<div class="page-header clearfix">
  <?php print render($content['header']); ?>
</div>
<?php endif; ?>

<div<?php print $css_id ? " id=\"$css_id\" class=\"page-wrapper\"" : " id=\"page-wrapper\""; ?>>

<?php else: ?>

<div<?php print $css_id ? " id=\"$css_id\" class=\"page-wrapper\"" : " id=\"page-wrapper\""; ?>>

  <?php if (!empty($content['header'])): ?>
  <div class="page-header clearfix">
    <?php print render($content['header']); ?>
  </div>
  <?php endif; ?>

<?php endif; ?>

  <?php if (!empty($content['main'])): ?>
  <div class="page-main clearfix">
    <?php print render($content['main']); ?>
  </div>
  <?php endif; ?>

<?php if (isset($footer_outside) && $footer_outside): ?>

</div>

<?php if (!empty($content['footer'])): ?>
<div class="page-footer clearfix">
  <?php print render($content['footer']); ?>
</div>
<?php endif; ?>

<?php else: ?>

  <?php if (!empty($content['footer'])): ?>
  <div class="page-footer clearfix">
    <?php print render($content['footer']); ?>
  </div>
  <?php endif; ?>

</div>

<?php endif; ?>


