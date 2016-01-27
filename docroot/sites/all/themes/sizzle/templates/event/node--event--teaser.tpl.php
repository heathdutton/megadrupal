<?php
/**
 * @file
 * Template for Event node in Featured view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <?php if (!empty($content['field_event_images'])): ?>
    <div class="event__images">
      <?php print render($content['field_event_images'][0]); ?>
    </div>
  <?php endif; ?>

  <a href="<?php print $node_url; ?>" class="link--overlay">
    <h3 class="event__title"><?php print $title; ?></h3>

    <div class="bottom">
      <?php if (!empty($content['field_event_location'])): ?>
        <span class="event__location">
          <?php print $field_event_location[0]['locality'] . ', ' . $field_event_location[0]['administrative_area']; ?>
        </span>
      <?php endif; ?>

      <?php if (!empty($content['field_event_date'])): ?>
        <span class="event__location">
          <?php print render($content['field_event_date']); ?>
        </span>
      <?php endif; ?>
    </div>
  </a>

  <?php print render($extra); ?>
</article>
