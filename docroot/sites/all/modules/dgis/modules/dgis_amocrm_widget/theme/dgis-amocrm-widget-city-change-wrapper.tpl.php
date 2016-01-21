<?php
/**
 * @file
 * Theming for wrapper of city change interface.
 */
?>
<div class="widget-2gis-list">
  <div class="widget-2gis-list-title">
    <?php print t('Choose the city') ?>
  </div>
  <?php if (!empty($cities)): ?>
    <?php print $cities; ?>
  <?php endif; ?>
</div>
