<?php
/**
 * @file views-jqm-basic-list.tpl.php
 * Default jQM view template to display a grid.
 *
 * - $title : The title of this group of rows.  May be empty.
 */
?>

<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="<?php print $class; ?>"<?php print $attributes; ?>>
    <?php foreach ($rows as $row_number => $columns): ?>
        <?php foreach ($columns as $column_number => $item): ?>
          <div class="<?php print $column_classes[$row_number][$column_number]; ?>">
            <div <?php print drupal_attributes($row_attributes); ?>><?php print $item; ?></div>
          </div>
        <?php endforeach; ?>
    <?php endforeach; ?>
</div>
