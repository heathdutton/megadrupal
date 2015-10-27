<?php
/**
 * @file
 * Template for a MapBox map
 */
?>
<div id="<?php print $map_id ?>" style="height: <?php print $height ?>">
  <?php if ($extras): ?>
    <?php print $extras; ?>
  <?php endif ?>
</div>
