<?php

/**
 * @file
 * Default template for an advanced poll bar.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $votes: The number of votes for this choice
 * - $percentage: The percentage of votes for this choice.
 * - $vote: The choice number of the current user's vote.
 * - $voted: Set to TRUE if the user voted for this choice.
 * - $media: rendered media field
 *   The actual images can be themed via theme_advpoll_field_image_image().
 *
 * @see
 * - advpoll-bar.tpl.php provided by advpoll module.
 * - theme_advpoll_media().
 */
$voted_class = '';
if ($voted):
  $voted_class = ' voted';
endif;
?>
<div class="poll-bar<?php print $voted_class; ?>">
  <?php print $media; ?>
  <div class="text"><?php print $title; ?></div>
  <div class="bar">
    <div style="width: <?php print $percentage; ?>%;" class="foreground"></div>
  </div>
  <div class="percent">
    <?php print $percentage; ?>% (<?php print format_plural($votes, '1 vote', '@count votes'); ?>)
  </div>
</div>
