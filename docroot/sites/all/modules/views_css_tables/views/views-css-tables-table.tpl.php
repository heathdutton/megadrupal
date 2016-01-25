<?php
/**
 * @file
 * Views CSS Table template.
 */
?>
<div class="table views-css-tables-table <?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if (!empty($title) || !empty($caption)) : ?>
    <div class="views-css-tables-table--caption"><?php print $caption . $title; ?></div>
  <?php endif; ?>
  <?php if (!empty($header)) : ?>
    <div class="views-css-tables-table--thead">
      <div class="views-css-tables-table--row">
        <?php foreach ($header as $field => $label): ?>
          <div class="views-css-tables-table--cell views-css-tables-table--cell-header <?php print $header_classes[$field]; ?>">
            <?php print $label; ?>
          </div>
        <?php endforeach; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="views-css-tables-table--tbody">
  <?php foreach ($rows as $row_count => $row): ?>
    <div class="views-css-tables-table--row <?php print implode(' ', $row_classes[$row_count]); ?>">
      <?php foreach ($row as $field => $content): ?>
        <div class="views-css-tables-table--cell <?php print $field_classes[$field][$row_count];?>" <?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
          <?php print $content; ?>
        </div>
      <?php endforeach; ?>
    </div>
  <?php endforeach; ?>
  </div>
</div>
