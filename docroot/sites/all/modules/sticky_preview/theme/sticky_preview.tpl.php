<?php
/**
 * @file
 * sticky_preview.tpl.php file.
 *
 *  Available custom variables:
 *  - $live_preview_title.
 *  - $live_preview_fields.
 */
?>

<div class="live_preview-wrapper">
  <div class="control-bar">
    <div class="button minimize">
      <div class="icon"></div>
    </div>
  </div>
  <div class="content">
    <h2 class="live-preview-title"><?php print t($live_preview_title . ' preview'); ?></h2>
    <ul class="fields-listr">
      <?php foreach ($live_preview_fields as $key => $field_info): ?>
        <li class="field-item <?php print str_replace('_', '-', $field_info['field_name']); ?>">
          <h3 class="field-title"><?php print t($field_info['label']); ?></h3>
          <div id="<?php print $field_info['field_name'] . '-placeholder'; ?>"></div>
        </li>
      <?php endforeach; ?>
    </ul>
  </div>
</div>
