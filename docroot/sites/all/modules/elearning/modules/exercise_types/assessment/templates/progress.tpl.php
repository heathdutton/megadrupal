<?php
/**
 * @file
 * Template file for the progress.
 */
?>
<div class="progress">
  <?php
  if ($percentage >= 100):
    print t('Completed!');
  else:
    print t('@percentage complete', array('@percentage' => $percentage . '%'));
  endif;
  ?>
  <div class="progress-bar-wrapper">
    <div class="progress-bar" style="width:<?php print $percentage; ?>%">
    </div>
  </div>
</div>
