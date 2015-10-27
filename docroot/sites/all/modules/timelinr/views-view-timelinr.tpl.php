<?php
/**
 * @file
 * This template includes the timelinr view.
 */
?>
<div class="timelinr-wrapper">
  <?php if ($rows): ?>
    <div id="<?php print $timelinr_id ?>" class="timelinr-container clearfix">
      <ul id="<?php print $timelinr_dates_id ?>" class="timelinr-dates">
        <?php foreach ($row_dates as $row_date): ?>
          <li><?php print $row_date ?></li>
        <?php endforeach; ?>
      </ul>  
      <ul id="<?php print $timelinr_issues_id ?>" class="timelinr-issues">
        <?php foreach ($rows as $row): ?>
          <?php print $row ?>
        <?php endforeach; ?>
      </ul>
       <a href="#" id="next" class="timelinr-next">+</a> <!-- optional -->
       <a href="#" id="prev" class="timelinr-prev">-</a> <!-- optional -->
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>
</div>
