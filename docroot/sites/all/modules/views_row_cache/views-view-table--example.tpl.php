<?php

/**
 * @file
 * Example template to display a view as a table using Views Row Cache.
 *
 * You just need to inject the boundary items so that the markup of a single row
 * can be identified and isolated. Always put the 'end_rows' boundary item after
 * the last row.
 */
?>
<table <?php if ($classes) { print 'class="'. $classes . '" '; } ?><?php print $attributes; ?>>
   <?php if (!empty($title) || !empty($caption)) : ?>
     <caption><?php print $caption . $title; ?></caption>
  <?php endif; ?>
  <?php if (!empty($header)) : ?>
    <thead>
      <tr>
        <?php foreach ($header as $field => $label): ?>
          <th <?php if ($header_classes[$field]) { print 'class="'. $header_classes[$field] . '" '; } ?>>
            <?php print $label; ?>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>
  <?php endif; ?>
  <tbody>
    <?php foreach ($rows as $row_count => $row): ?>
      <!-- This is the boundary item separating a single row from the next one. -->
      <?php print $views_row_cache_boundary[$row_count] ?>
      <tr <?php if ($row_classes[$row_count]) { print 'class="' . implode(' ', $row_classes[$row_count]) .'"';  } ?>>
        <?php foreach ($row as $field => $content): ?>
          <td <?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
    <!-- This is the closing boundary item: markup above the first boundary -->
    <!-- item and below this one is never cached. -->
    <?php print $views_row_cache_boundary['end_rows'] ?>
  </tbody>
</table>
