<?php
/**
 * @file
 * Customize the display of a campaign report with webform submission results.
 *
 * Available variables:
 * - $bounce_convert_filter_url: Filter if any applied.
 * - $bounce_convert_percentage: Total Percentage.
 * - $bounce_convert_conversions: Total Conversions.
 * - $bounce_convert_impression: Total Impressions.
 */
?>
<?php global $base_url; ?>
<?php if ($bounce_convert_filter_url == '/percentage'): ?>
  <div style="font-size:150px;"><?php print $bounce_convert_percentage; ?> %</div>
<?php endif; ?>
<div class="bounce-convert-main-wrapper">

  <div class="bounce-convert-tabs-wrapper">

    <a href="<?php print $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results/impression">
      <div class="bounce-convert-tab bounce-convert-impression">
        <div class="conversion-digit"><?php print $bounce_convert_impression; ?></div>
        <div class="conversion-title">Impressions</div>
      </div>
    </a>

    <a href="<?php print $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results/conversion">
      <div class="bounce-convert-tab bounce-convert-conversion">
        <div class="conversion-digit"><?php print $bounce_convert_conversions; ?></div>
        <div class="conversion-title">Conversions</div>
      </div>
    </a>

    <div class="bounce-convert-tab bounce-convert-percentage">
      <div class="conversion-digit"><?php print round($bounce_convert_percentage, 1); ?>%</div>
      <div class="conversion-title">Conversion Rate</div>
    </div>

  </div><!--bounce-convert-tabs-wrapper-->

  <div class="bounce-convert-filter-bar">
    <div class="bounce-convert-filter-title">
      <span><img src="<?php print '/' . drupal_get_path('module', 'bounce_convert'); ?>/images/chart.png" alt="Chart" title="Chart"/></span>
      Campaign Overview
    </div>
    <div class="bounce-convert-filter-options">
      <span><img src="<?php print '/' . drupal_get_path('module', 'bounce_convert'); ?>/images/calendar.png" alt="Calendar" title="Calendar"/></span>
      Filter
      <ul style="display: none;">
        <a href="<?php $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results<?php print $bounce_convert_filter_url; ?>/today"><li>Today</li></a>
        <a href="<?php $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results<?php print $bounce_convert_filter_url; ?>/yesterday"><li>Yesterday</li></a>
        <a href="<?php $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results<?php print $bounce_convert_filter_url; ?>/7days"><li>Last 7 Days</li></a>
        <a href="<?php $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results<?php print $bounce_convert_filter_url; ?>/30days"><li>Last 30 Days</li></a>
        <a href="<?php $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results<?php print $bounce_convert_filter_url; ?>/this-month"><li>This Month</li></a>
        <a href="<?php $base_url; ?>/admin/reports/bounce-convert/<?php print arg(3); ?>/results<?php print $bounce_convert_filter_url; ?>/last-month"><li>Last Month</li></a>
      </ul>
    </div>
  </div>
  
</div><!--bounce-convert-main-wrapper-->
