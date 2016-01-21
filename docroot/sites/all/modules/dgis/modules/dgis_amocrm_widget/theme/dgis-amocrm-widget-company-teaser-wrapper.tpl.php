<?php
/**
 * @file
 * Theming for wrapper of the list of 2gis companies (teaser view mode).
 */
?>
<div class="widget-2gis-list">
  <?php if (!empty($data['summary'])): ?>
    <div class="widget-2gis-list-title">
      <?php print $data['summary']; ?>
    </div>
  <?php endif; ?>
  <ul class="widget-2gis-list-wrap">
    <?php if (!empty($data['results'])): ?>
      <?php print $data['results']; ?>
    <?php else: ?>
      <?php print t('No results were found'); ?>
    <?php endif; ?>
  </ul>
</div>
