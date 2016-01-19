<?php
/**
 * @file
 * Template for Event node in Full view mode.
 */
?>
<article class="<?php print $classes; ?> margin--lg--bottom">
  <h1 class="event__title margin--md--bottom hidden-sm hidden-md hidden-lg">
    <?php print $title; ?>
  </h1>

  <div class="row">
    <div class="col-sm-5">
      <?php if (!empty($content['field_event_images'])): ?>
        <div class="event__images margin--md--bottom">
          <?php print render($content['field_event_images'][0]); ?>
        </div>
      <?php endif; ?>
    </div>
    <div class="col-sm-7">
      <div class="text--center border--xs--bottom padding--sm--bottom margin--sm--bottom">
        <h1 class="event__title clear-margin--top hidden-xs">
          <?php print $title; ?>
        </h1>
        <?php if (!empty($content['field_event_location'])): ?>
          <h4 class="event__date">
            <?php print render($content['field_event_date']); ?>
          </h4>
        <?php endif; ?>
        <?php if (!empty($content['field_event_location'])): ?>
          <h5 class="event__location text--light">
            <?php print render($content['field_event_location']) ?>
          </h5>
        <?php endif; ?>
      </div>

      <?php if (!empty($content['body'])): ?>
          <div class="event__body">
            <?php print render($content['body']) ?>
          </div>
        <?php endif; ?>
    </div>

  </div>

  <?php print render($extra); ?>
</article>
