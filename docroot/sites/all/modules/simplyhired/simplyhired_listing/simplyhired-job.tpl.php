<?php
/** 
 * @file simplyhired-job.tpl.php
 * Default theme implementation for a SimplyHired job.
 * 
 * Available variables:
 * - $title - The title for the job posting.
 * - $company - The company that posted the original job.
 * - $source - The original source for the job posting.
 * - $post_date - The date the job was originally posted.
 * - $location - The job location.
 * - $description - A description of the job position.
 */

?>
<div class="simplyhired-job <?php print $zebra; ?>">
 <h3><?php print render($title); ?></h3>
 <?php if ($company): ?>
   <div class="simplyhired-job-company">Company: <?php print render($company); ?></div>
 <?php endif; ?>
 <div class="simplyhired-job-source">Source: <?php print render($source); ?></div>
 <div class="simplyhired-job-post-date">Posted on: <?php print render($post_date); ?></div>
 <?php print render($location); ?>
 <div class="simplyhired-job-description"><?php print render($description); ?></div>
</div>