<?php
/*
 * @file hotel_booking_search_results.tpl.php
 * template file for search results - makes theming easier
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 * @see template_preprocess_hotel_booking_search_form
 *
 */
?>
<div id="hotel-booking-search-form-wrap">
  <div id="hotel-booking-search-<?php print ($page ? 'page' : 'block'); ?>">
    <?php if (isset($link)) : ?>
      <div class="text-center">
        <?php print $link; ?>
      </div>
    <?php endif; ?>
    <div class="hotel-booking-search-field-wrap" id="check-in-wrap">
      <?php print $check_in; ?>
    </div>

    <div class="hotel-booking-search-field-wrap small-field" id="nights-wrap">
      <?php print $nights; ?>
    </div>

    <div class="hotel-booking-search-field-wrap small-field" id="adults-wrap">
      <?php print $adults; ?>
    </div>

    <?php if ($children) : ?>
      <div class="hotel-booking-search-field-wrap small-field" id="children-wrap">
        <?php print $children; ?>
      </div>
    <?php endif; ?>

    <?php if ($smoking) : ?>
      <div class="hotel-booking-search-field-wrap" id="smoking-wrap">
        <?php print $smoking; ?>
      </div>
    <?php endif; ?>

    <div class="hotel-booking-search-field-wrap" id="submit-wrap">
      <?php print $submit; ?>
    </div>

    <?php if (isset($results)) : ?>
      <div id="hotel-booking-search-results" class="clear-left">
        <?php print $results; ?>
      </div>
    <?php endif; ?>

    <?php if (!$page) : ?>
      <div class="clear">&nbsp;</div>
    <?php endif; ?>
  </div>
  <?php echo drupal_render_children($form) ?>
</div>
