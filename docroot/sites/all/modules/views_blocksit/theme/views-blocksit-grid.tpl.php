<?php
/**
 * @file
 * The Blocksit styles.
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="views-blocksit-grid">
  <?php foreach ($rows as $id => $row): ?>
    <div<?php if (isset($classes_array[$id])): ?> class="<?php print $classes_array[$id]; ?> blocksit-block" <?php endif; ?>>
      <?php print $row; ?>
    </div>
  <?php endforeach; ?>
</div>
