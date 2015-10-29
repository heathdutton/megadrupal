<?php
/**
 * @file
 */
?>
<div<?php echo !empty($attr) ? ' ' . drupal_attributes($attr) : ''; ?>>
<?php if ($title): ?>
  <h2><?php print $title ?></h2>
<?php endif; ?>

  <div class="content"><?php print $content ?></div>
</div>
