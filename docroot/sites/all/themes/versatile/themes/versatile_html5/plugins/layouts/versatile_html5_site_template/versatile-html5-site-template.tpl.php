<?php if (isset($header_outside) && $header_outside): ?>

<?php if (!empty($content['header'])): ?>
<header class="page-header clearfix">
  <?php print render($content['header']); ?>
</header>
<?php endif; ?>

<div<?php print $css_id ? " id=\"$css_id\" class=\"page-wrapper\"" : " id=\"page-wrapper\""; ?>>

<?php else: ?>

<div<?php print $css_id ? " id=\"$css_id\" class=\"page-wrapper\"" : " id=\"page-wrapper\""; ?>>

  <?php if (!empty($content['header'])): ?>
  <header class="page-header clearfix">
    <?php print render($content['header']); ?>
  </header>
  <?php endif; ?>

<?php endif; ?>

  <?php if (!empty($content['main'])): ?>
  <section class="page-main clearfix">
    <?php print render($content['main']); ?>
  </section>
  <?php endif; ?>

<?php if (isset($footer_outside) && $footer_outside): ?>

</div>

<?php if (!empty($content['footer'])): ?>
<footer class="page-footer clearfix">
  <?php print render($content['footer']); ?>
</footer>
<?php endif; ?>

<?php else: ?>

  <?php if (!empty($content['footer'])): ?>
  <footer class="page-footer clearfix">
    <?php print render($content['footer']); ?>
  </footer>
  <?php endif; ?>

</div>

<?php endif; ?>


