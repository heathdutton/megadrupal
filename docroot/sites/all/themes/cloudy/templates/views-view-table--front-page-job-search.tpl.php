<?php
/**
 * Views table template override. Removes the table header as the conditional
 * if-statement for hiding the header when empty does not seem to work.
 */
?>
<table <?php if ($classes) { print 'class="'. $classes . '" '; } ?><?php print $attributes; ?>>
  <?php if (!empty($title)) : ?>
  <caption><?php print $title; ?></caption>
  <?php endif; ?>
  <tbody>
  <?php foreach ($rows as $row_count => $row): ?>
  <tr class="<?php print implode(' ', $row_classes[$row_count]); ?>">
    <?php foreach ($row as $field => $content): ?>
    <td <?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print $field_attributes[$field][$row_count]; ?>>
      <?php print $content; ?>
    </td>
    <?php endforeach; ?>
  </tr>
    <?php endforeach; ?>
  </tbody>
</table>
