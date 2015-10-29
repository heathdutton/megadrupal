<div class='round round-<?php print $rid; ?>'>
  <div class="round-heading">
    <h4><?php print t('Round !rid', array('!rid' => $rid)); ?></h4>
    <div class="round-heading-rule"></div>
  </div>
  <?php print $matches; ?>
</div>
<div class="line-column line-column-<?php print $rid; ?>">
  <?php print $connectors; ?>
</div>