<?php

/**
 * @file Google Civic Locations Template.
 * Renders an array of locations.
 */
?>

<?php if ($locations) : ?>
  <?php foreach ($locations as $location) : ?>
    <div class="<?php print $class_prefix; ?>-location vcard">
      <p class="org"><?php print $location->address->locationName; ?></p>
      <div class="adr">
        <p class="street-address">
          <?php print $location->address->line1; ?>
          <?php if (!empty($location->address->line2)) print $location->address->line2; ?>
          <?php if (!empty($location->address->line3)) print $location->address->line3; ?>
        </p>
        <div>
          <span class="locality"><?php print $location->address->city; ?></span>,
          <span class="region"><?php print $location->address->state; ?></span>
          <span class="postal-code"><?php print $location->address->zip ?></span>
          <span class="country-name"></span>
        </div>
      </div>
      <div class="<?php print $class_prefix; ?>-hours">
    <?php if ($location->opening_time) : ?>
      <p><?php print t('Polls are open from '); ?>
        <span class="<?php print $class_prefix; ?>-opening-time">
          <?php print $location->opening_time; ?>
        </span> -
        <span class="<?php print $class_prefix; ?>-closing-time">
          <?php print $location->closing_time; ?>
        </span>
	<?php if (!empty($location->startDate) && !empty($location->endDate)): ?>
	    <?php print t('between'); ?>
              <span class="<?php print $class_prefix; ?>-start-date">
                <?php print $location->startDate; ?>
	      </span>
	    <?php print t('and'); ?>
	      <span class="<?php print $class_prefix; ?>-end-date">
                <?php print $location->endDate; ?>
             </span>
        <?php endif; ?>
      </p>
    <?php endif; ?>
    </div>
    </div>
 <?php endforeach; ?>
 <?php endif; ?>