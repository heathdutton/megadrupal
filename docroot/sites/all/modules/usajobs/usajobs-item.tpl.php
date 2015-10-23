<?php

/**
 * @file
 * Default theme implementation to format an individual job item for display
 * on the block content.
 *
 * Available variables:
 * - $job_title: Position title.
 * - $job_url: URL to the originating job item.
 * - $job_organization: Government organization name.
 * - $job_location: Location of the job.
 * - $job_salary_min: Minimum salary job offer.
 * - $job_salary_max: Maximum salary job offer.
 * - $job_start_date: Start date for applying job.
 * - $job_end_date: End date for applaying job.
 *
 * To make your custom template with all available variables above, copy this
 * file and place in your theme folder and clear caches to see your changed.
 *
 * @see template_preprocess()
 * @see template_preprocess_usajobs_item()
 *
 * @ingroup themeable
 */
?>
<div class="job-item">

  <div class="job-item-title">
    <a href="<?php print $job_url; ?>" target="_blank"><?php print $job_title; ?></a>
  </div>

  <div class="job-item-organization"><?php print $job_organization; ?></div>

  <div class="job-item-location-salary">
    <span class="job-item-location"><?php print $job_location; ?></span> | 
    <span class="job-item-salary-min">$<?php print $job_salary_min; ?>+</span>
  </div>

  <div class="job-item-end-date">Apply by <?php print $job_end_date; ?></div>

</div>
