<?php

/**
 * @file
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="<?php print $class; ?>"<?php print $attributes; ?>>
    <?php foreach ($rows as $row_number => $columns): ?>
      <div class="row <?php print $row_classes[$row_number]; ?>">
        <?php foreach ($columns as $column_number => $item): ?>
          <div class="col <?php print $column_classes[$row_number][$column_number]; ?>">
            <?php print $item; ?>
          </div>
        <?php endforeach; ?>
      </div>
    <?php endforeach; ?>
</div>
