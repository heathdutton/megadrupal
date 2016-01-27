<?php
/**
 * @file
 * Donations Thermometer base output code.
 *
 * Available variables:
 * - $block_id (int): Contains the ID number of this block.
 * - $size (string): Size of the the thermometer used in CSS: 'small', 'large'
 * - $orientation (string): Orientation of the the thermometer used in CSS:
 * 'vertical', 'horizontal'
 * - $inlinecss (string): CSS that contins the size of the thermometer.
 * - $percent (double): Percentage of the thermometer.
 * - $goal (double): Thermometer goal.
 * - $current (double): Current thermometer value.
 * - $prefix (string): Prefix to 'Current' and 'Goal'.
 * - $postfix (string): Prefix to 'Current' and 'Goal'.
 * - $url_enabled (bool): Is the URL enabled.
 * - $url (string): URL.
 *
 * @see template_preprocess_forum_list()
 *
 * @ingroup themeable
 */
 ?>


<div id="donations_thermometer-<?php print $block_number; ?>" class="donations_thermometer_wrapper">
  <?php if($url_enabled == TRUE):?>
  <a href="<?php print $url ?>">
  <?php endif?>
  <div class="gauge <?php print $size ?> <?php print $orientation ?> <?php print $color ?>">
    <span class="current-value"><?php print $percent; ?>%</span>
    <span class="current-meter" style="<?php print $inlinecss ?>"></span>
  </div>
  <p class="donations_thermometer-current">
    <label><?php print t('Current:'); ?></label>
    <span><?php print $prefix; ?><?php print $current; ?><?php print $postfix; ?></span>
  </p>
  <p class="donations_thermometer-goal">
    <label><?php print t('Goal:'); ?></label>
    <span><?php print $prefix; ?><?php print $goal; ?><?php print $postfix; ?></span>
  </p>
  <?php if($url_enabled == TRUE):?>
  </a>
  <?php endif?>
</div>
