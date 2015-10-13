<?php
/**
 * @file
 * Template to render petition signatories summary.
 */
?>
<div class="petition-signatures">
  <span class="received"><?php print check_plain($received) ?></span>
  <?php if (!empty($goal)) :  print t('of') ?>
    <span class="goal"><?php print check_plain($goal) ?></span>
  <?php endif;
  print t('signatures for this petition.'); ?>
</div>
