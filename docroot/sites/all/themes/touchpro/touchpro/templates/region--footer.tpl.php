<?php
/**
 * @file
 * Implementation to display the footer region.
 */
?>

<?php if ($content): ?>
  <footer class="<?php print $classes; ?> clearfix" role="contentinfo">
    <div class="mast">
      <?php print $content; ?>
    </div>
  </footer>
<?php endif; ?>
