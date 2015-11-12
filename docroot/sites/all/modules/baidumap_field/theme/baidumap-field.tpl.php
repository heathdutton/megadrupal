<?php

/**
 * @file
 * Default theme implementation for baidu map fields.
 *
 * Available variables:
 * - $name: the display name of the map
 * - $map_id: a unique ID for the map.
 *   to identify the map container.
 */

?>
<div class="baidumap-field">
  <?php if(!empty($name)) : ?>
    <div class="baidumap-field-label">
      <?php print $name; ?>
    </div>
  <?php endif; ?>
  <div id="baidumap_field_<?php print $map_id; ?>" class="baidumap_field_display"></div>
</div>
