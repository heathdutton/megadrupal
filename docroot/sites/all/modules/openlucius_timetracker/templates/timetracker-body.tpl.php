<?php
/**
 * @file
 * This file contains the body of the timetracker form.
 *
 * The opening span and content is generated before this template.
 * So it might appear to be incomplete but it's not.
 */
?>

  <span class="current-time <?php if (isset($current_time2)): ?> hide-initially <?php endif; ?>">
    <?php print $current_time1; ?>
  </span>
  <span class="current-active-todo <?php if (isset($current_time2)): ?> hide-initially <?php endif; ?>">
    <?php print $title1; ?>
  </span>

  <span class="timer-unfold pull-right glyphicon" data-toggle="collapse" href="#collapsetimer" aria-expanded="false" aria-controls="collapsetimer"></span>

  <?php if (isset($current_time2)): ?>
    <span class="other-timer">
      <?php print $submit2; ?>
      <span class="current-time2">
        <?php print $current_time2; ?>
      </span>
      <span class="current-active-todo2">
      <?php print $title2; ?>
      </span>
    </span>
  <?php endif; ?>
</span>

<div class="collapse" id="collapsetimer">
  <span class="your-time <?php if (isset($current_time2)): ?> hide-initially <?php endif; ?>">
    <?php print $your_time1; ?>
  </span>
  <span class="total-progress <?php if (isset($current_time2)): ?> hide-initially <?php endif; ?>">
    <?php print $total_progress1; ?>
  </span>
  <?php if (isset($current_progress1)): ?>
    <span class="current-progress <?php if (isset($current_time2)): ?> hide-initially <?php endif; ?>">
      <?php print $current_progress1; ?>
    </span>
  <?php endif; ?>
  <?php if (isset($maximum1)): ?>
    <span class="maximum <?php if (isset($current_time2)): ?> hide-initially <?php endif; ?>">
      <?php print $maximum1; ?>
    </span>
  <?php endif; ?>

  <?php
  /**
   * Secondary timer.
   */
  ?>
  <?php if (isset($current_time2)): ?>
    <span class="other-timer">
      <span class="your-time">
        <?php print $your_time2; ?>
      </span>
      <span class="total-progress">
        <?php print $total_progress2; ?>
      </span>
    <?php if (isset($current_progress2)): ?>
      <span class="current-progress">
        <?php print $current_progress2; ?>
      </span>
    <?php endif; ?>
    <?php if (isset($maximum2)): ?>
      <span class="maximum">
      <?php print $maximum2; ?>
      </span>
    <?php endif; ?>
    </span>
  <?php endif; ?>
</div>
