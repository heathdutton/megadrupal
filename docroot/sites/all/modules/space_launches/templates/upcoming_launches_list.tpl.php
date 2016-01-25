<?php
/**
 * @file
 * Template for displaying upcoming launches in a list.
 *
 * Available variables:
 * - $full: Boolean indicating whether complete info should be displayed.
 * - $date_format: PHP date format string for launch date.
 * - $time_format: PHP date format string for launch time.
 * - $timezone: The timezone to use when display launch date / time.
 * - $launches: Array of SpaceLaunchesLaunch entities.
 */
?>

<ul>

  <?php foreach ($launches as $launch) : /* date_default_timezone_get() */ ?>

    <li>
      <?php if (!empty($launch->url)) : ?>
        <p><?php print l($launch->title, $launch->url); ?></p>
      <?php else : ?>
        <p><?php print $launch->title; ?></p>
      <?php endif; ?>

      <p>
        <?php print format_date($launch->time, 'custom', $date_format, $timezone); ?>
        <?php if ($launch->time_is_exact) : ?>
          <?php print format_date($launch->time, 'custom', $time_format, $timezone); ?>
        <?php endif; ?>
      </p>

      <?php if ($full) : ?>
        <p><?php print $launch->description; ?></p>
      <?php endif; ?>

    </li>

  <?php endforeach; ?>

</ul>
