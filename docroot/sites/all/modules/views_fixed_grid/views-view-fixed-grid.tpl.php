<?php
/**
 * @file views-view-fixed-grid.tpl.php
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains an array of fields. Each row a cell.
 * - $coordinates is an array keyed by row number nested by
 *   column number.The inside value is the index of the field
 *   on the $rows array in that coordinate.
 * - $numcols contains the numbers of columns as specified in
 *   the view's options
 * - $numrows contains the numbers of rows as specified in
 *   the view's options
 * - $class contains the class of the table.
 * - $attributes contains other attributes for the table.
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<table class="<?php print $class; ?>"<?php print $attributes; ?>>
  <tbody>
    <?php for ($row = 0; $row < $numrows; $row++): ?>
      <tr class="row-<?php print $row; ?><?php if ($row == 0): ?> row-first<?php elseif ($row == $numrows - 1): ?> row-last<?php endif; ?>">
        <?php for ($col = 0; $col < $numcols; $col++): ?>
            <?php if (isset($coordinates[$row][$col])): ?>
              <td class="full-cell full-cell-<?php print $coordinates[$row][$col]; ?> col-<?php print $col; ?><?php if ($col == 0): ?> col-first<?php elseif ($col == $numcols - 1): ?> col-last<?php endif; ?>">
              <?php print $rows[$coordinates[$row][$col]]; ?>
            <?php else: ?>
              <td class="empty-cell col-<?php print $col; ?><?php if ($col == 0): ?> col-first<?php elseif ($col == $numcols - 1): ?> col-last<?php endif; ?>">
            <?php endif ?>
          </td>
        <?php endfor; ?>
      </tr>
    <?php endfor; ?>
  </tbody>
</table>
