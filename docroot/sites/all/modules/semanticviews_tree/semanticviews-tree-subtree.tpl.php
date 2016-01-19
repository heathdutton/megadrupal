<?php
/**
 * @file
 * Template file for rendering a subtree in a nested list.
 *
 * Available variables:
 * - $container_element: HTML element for the container, empty if none.
 * - $container_attributes: An array of attributes for the container element.
 * - $rows: An array of rows (with rendered markup) to output.
 * - $row_element: HTML element for the row, empty if none.
 * - $row_attributes: An array of attributes for the row element.
 */
?>
<?php if (!empty($container_element)): ?>
<<?php print $container_element; ?><?php print drupal_attributes($container_attributes); ?>>
<?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
<?php if (!empty($row_element)): ?>
  <<?php print $row_element; ?><?php print drupal_attributes($row_attributes[$id]); ?>>
  <?php endif; ?>
<?php print $row; ?>
<?php if (!empty($row_element)): ?>
  </<?php print $row_element; ?>>
  <?php endif; ?>
<?php endforeach; ?>
<?php if (!empty($container_element)): ?>
</<?php print $container_element; ?>>
<?php endif; ?>
