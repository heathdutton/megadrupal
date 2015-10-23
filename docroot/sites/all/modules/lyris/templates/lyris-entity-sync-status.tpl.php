<?php
/**
 * @file
 * Display information about the sync status of a given entity.
 */
?>
<div class="<?php print $classes; ?>">
  <div class="lyris-sync-status"><?php print $status; ?></div>
  <div class="lyris-entity-changed"><span class="label"><?php print t('Last Changed'); ?>:</span> <?php print $changed; ?></div>
  <div class="lyris-entity-synced"><span class="label"><?php print t('Last Synced'); ?>:</span> <?php print $synced; ?></div>
</div>
