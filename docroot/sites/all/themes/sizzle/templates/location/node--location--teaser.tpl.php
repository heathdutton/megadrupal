<?php

/**
 * @file
 * Template for Location node in Teaser view mode.
 */
?>
<article class="<?php print $classes; ?> margin--lg--bottom padding--lg--bottom border--xs--bottom">
  <div class="row">
    <div class="col-sm-6">
      <?php if (!empty($content['field_location_images'])): ?>
        <div class="location__image margin--sm--bottom">
          <?php print render($content['field_location_images']); ?>
        </div>
      <?php endif; ?>
    </div>
    <div class="col-sm-6">
      <h2 class="location__title clear-margin--top text--large">
        <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
      </h2>

      <div class="row">
        <div class="col-md-6">
          <?php if (!empty($content['field_location_address'])): ?>
            <div class="location__address margin--sm--bottom">
              <?php print render($content['field_location_address']); ?>
            </div>
          <?php endif; ?>

          <?php if (!empty($content['field_location_opening_hours'])): ?>
            <div class="location__opening-hours margin--sm--bottom">
              <?php print render($content['field_location_opening_hours']); ?>
            </div>
          <?php endif; ?>
        </div>
        <div class="col-md-6">
          <?php if (!empty($content['field_location_contact'])): ?>
            <div class="location__contact margin--sm--bottom">
              <?php print render($content['field_location_contact']); ?>
            </div>
          <?php endif; ?>
        </div>
      </div>

      <?php if (!empty($content['menu_link'])): ?>
        <span class="location__menu-link">
          <?php print render($content['menu_link']); ?>
        </span>
        <span class="margin--sm--left margin--sm--right"><?php print t('or'); ?></span>
      <?php endif; ?>

      <?php if (!empty($content['more_link'])): ?>
        <span class="location__more-link">
          <?php print render($content['more_link']); ?>
        </span>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($extra); ?>
</article>
