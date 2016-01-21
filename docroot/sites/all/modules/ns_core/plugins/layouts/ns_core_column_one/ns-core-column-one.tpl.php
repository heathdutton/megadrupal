<?php
?>

<?php if (!empty($css_id)): ?>
  <div id="<?php print $css_id; ?>" class="clearfix">
<?php endif; ?>

<?php if (!empty($content['main'])): ?>
  <div class="page-main">
      <?php print render($content['main']); ?>
  </div>
<?php endif; ?>

<?php if (!empty($css_id)): ?>
  </div>
<?php endif; ?>
