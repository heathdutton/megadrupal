<?php
/**
 * @file
 * Template for MixItUp sorting items.
 */
?>
<div class="sort_wrapper form-item">
  <strong><?php print t('Sort by'); ?></strong>
  <div class="sort default" data-sort="default"><?php print t('Default'); ?></div>
  <?php foreach ($sorts as $sort_field => $label): ?>
    <div class="sort asc sort_item" data-sort="<?php print $sort_field; ?>"> <?php print $label; ?></div>
  <?php endforeach; ?>
</div>
