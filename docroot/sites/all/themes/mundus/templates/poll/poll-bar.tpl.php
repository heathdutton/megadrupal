<?php

/**
 * @file
 * Default theme implementation to display the bar for a single choice in a
 * poll.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $votes: The number of votes for this choice
 * - $total_votes: The number of votes for this choice
 * - $percentage: The percentage of votes for this choice.
 * - $vote: The choice number of the current user's vote.
 * - $voted: Set to TRUE if the user voted for this choice.
 *
 * @see template_preprocess_poll_bar()
 */
?>
<?php print $title ?>
<span class="percent">
  <?php print $percentage; ?>%
</span>
<div class="radius progress">
  <div class="meter" style="width: <?php print $percentage ?>%"></div>
</div>
