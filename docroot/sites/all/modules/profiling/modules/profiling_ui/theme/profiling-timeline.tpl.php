<?php
/**
 * @file
 * Pure CSS timeline chart.
 */
?>
<div class="profiling-timeline">
  <?php foreach ($slices as $slice): ?>
  <div class="profiling-timeline-line"">
    <div class="profiling-timeline-header">
      <?php if ($slice['link'] != $_GET['q']): ?>
        <a href="<?php print $slice['link']; ?>"><?php print $slice['title']; ?></a>
      <?php else: ?>
        <?php print $slice['title']; ?>
      <?php endif; ?>
    </div>
    <?php
      $left   = $slice['left'] . '%';
      $width  = $slice['width'] . '%';
    ?>
    <div class="profiling-timeline-container" style="position: relative;">
      <div class="profiling-timeline-slice" title="<?php print $slice['title']; ?>" style="position: absolute; left:<?php print $left; ?>; top: 0; width: <?php print $width; ?>;">
        <!-- <a href="<?php print $slice['link']; ?>"> -->
          <?php print $slice['label']; ?>
        <!-- </a> -->
      </div>
    </div>
  </div>
  <?php endforeach; ?>
</div>