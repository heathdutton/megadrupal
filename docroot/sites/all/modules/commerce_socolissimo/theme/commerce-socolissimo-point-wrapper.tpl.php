<?php
/**
 * @file
 * Commerce So Colissimo Points list
 */

?>
<div class="<?php print $classes; ?>">
  <div class="title">
    <h3><?php print t('Delivery points list'); ?></h3>
  </div>
  <div class="content">
    <?php print render($content['points']); ?>
  </div>
</div>
