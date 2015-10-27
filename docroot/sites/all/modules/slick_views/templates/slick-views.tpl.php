<?php
/**
 * @file
 * Default theme implementation for the Slick views template.
 *
 * Available variables:
 * - $rows: The array of items.
 * - $options: Array of available settings via Views UI.
 */
?>
<?php print $wrapper_prefix; ?>
  <?php foreach ($rows as $id => $row): ?>
    <?php print render($row); ?>
  <?php endforeach; ?>
<?php print $wrapper_suffix; ?>
