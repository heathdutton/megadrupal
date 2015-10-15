<?php

/**
 * @file
 * Default theme implementation for displaying a calendar event.
 *
 * Each event is a list item in an unordered list.
 *
 * Available variables:
 * - $created: The time at which the event was created.
 * - $description: A description of the event.
 * - $hangout_link: A link to the Google Hangout associated with the event.
 * - $html_link: A link to the event on google.com/calendar.
 * - $location: The location of the event.
 * - $summary: A summary of the event (the even't title).
 * - $updated: The time at which the event was last updated.
 * - $creator_email: The email address of the even't creator.
 * - $creator_display_name: The display name of the event's creator.
 * - $start: The time at which the event starts.
 * - $end: The time at which the event ends.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<div class="event">
  <?php if ($summary): ?>
    <div class="summary">
      <?php print $summary; ?>
    </div>
  <?php endif; ?>
  <?php if ($location): ?>
    <div class="location">
      <?php print $location; ?>
    </div>
  <?php endif; ?>
  <?php if ($start): ?>
    <div class="date">
      <?php print $start; ?>
    </div>
  <?php endif; ?>
  <?php if ($description): ?>
    <div class="description">
      <?php print $description; ?>
    </div>
  <?php endif; ?>
</div>
