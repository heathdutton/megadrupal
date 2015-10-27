<?php

/**
 * @file
 * Default theme implementation to display a day.
 *
 * Available variables:
 * - $loc: The location of the space.
 * - $spaces: An array of all spaces.
 * - $header: An array of all spaces for displaying in header.
 * - $items: The array of each time slot.
 *
 * @see template_preprocess_studyroom_availability_day()
 *
 * @ingroup themeable
 */
?>
<div class="studyroom-space-calendar">
<?php print $loc; ?>
<?php print render($page['navigation']); ?>
<table>
  <thead>
    <tr>
      <th class="time"><?php print t('Time')?></th>
      <?php foreach ($spaces_header as $space): ?>
        <th class="space"><?php print $space; ?></th>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($items as $time): ?>
    <tr>
      <td class="time">
        <span class="calendar-hour"><?php print $time['hour']; ?></span><span class="calendar-ampm"><?php print $time['ampm']; ?></span>
      </td>
      <?php $curpos = 0; ?>
      <?php foreach ($spaces as $id => $space): ?>
        <td class="<?php print $time['values'][$id]['classes']; ?>">
          <div class="calendar-item">
            <?php print $time['values'][$id]['entry'] ?>
          </div>
        </td>
      <?php endforeach; ?>
    </tr>
   <?php endforeach; ?>
  </tbody>
</table>
</div>
