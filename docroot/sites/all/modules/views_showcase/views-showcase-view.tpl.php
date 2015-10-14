<?php
/**
 * @file
 *  Views Showcase theme wrapper.
 *
 * Variables available:
 * - $skin: The skin chosen in the settings, either 'left' or 'right'
 * - $views_showcase_id: A unique identifier for the current display of the current view
 * - $structured_rows: The resulted rows, with each field properly flagged to specify where it should be displayed
 * - $view: An object representing the view being used
 * - $display_title : Title of the display
 *
 * @ingroup views_templates
 */
// dpm($structured_rows );
?>
<div class="skin-<?php print $skin; ?> item-list views-showcase clear-block">

  <div class="views-showcase-navigation-container">
    <?php if ($display_title): ?>
      <h2 class="views-showcase-title"><?php print $display_title; ?></h2>
    <?php endif; ?>
    <h3 class="views-showcase-naviation-title"><?php print t('Navigation'); ?></h3>
    <ul class="views-showcase-mini-list">
      <?php foreach ($structured_rows as $row_index => $structured_row): ?>
        <li class="<?php print $structured_row['nav_box_classes']; ?>">
          <div class="views-showcase-pager-item-sublist">
            <?php foreach ($structured_row as $field_id => $field): ?>
              <?php if (is_object($field) && $field->navigation_box): ?>
                <div class="views-showcase-pager-subitem views-showcase-navigation-box-<?php print $field_id; ?>">
                  <?php if ($field->link_anchor): ?>
                    <a href="<?php print '#' . $structured_row['anchor_name']; ?>"><?php print $field->content; ?></a>
                  <?php else: ?>
                    <?php print $field->content; ?>
                  <?php endif; ?>
                </div>
              <?php endif; ?>
            <?php endforeach; ?>
          </div>
        </li>
      <?php endforeach; ?>
    </ul>
  </div>

  <div class="views-showcase-content-container">
    <h3 class="views-showcase-content-title"><?php print t('Content'); ?></h3>
    <ul class="views-showcase-big-panel">
      <?php foreach ($structured_rows as $row_index => $structured_row): ?>
        <li class="<?php print $structured_row['big_box_classes']; ?>">
          <a name="<?php print $structured_row['anchor_name']; ?>" id="<?php print $structured_row['anchor_name']; ?>"></a>
          <?php foreach ($structured_row as $field_id => $field): ?>
            <?php if (is_object($field) && $field->big_box): ?>
              <div class="views-showcase-subitem views-showcase-big-box-<?php print $field_id; ?>">
                <?php if ($field->label): ?>
                  <label class="views-label-<?php print $field_id; ?>"><?php print $field->label; ?></label>
                <?php endif; ?>
                <span class="field-content"><?php print $field->content; ?></span>
              </div>
            <?php endif; ?>
          <?php endforeach; ?>
        </li>
      <?php endforeach; ?>
    </ul>
  </div>
</div>
