<?php

/**
 * @file simplyhired-listings.tpl.php
 * Default theme implementation a job listings. 
 * 
 * Available variables:
 * - $title:
 * - $message: If any error message is returned by the API call, it will be display
 *             using this variable.
 * - $jobs: Array of jobs to be displayed
 */
?>
<div id="simplyhired-listing-title" class="element-invisible"><?php print $title; ?></div>
<?php if($message): ?>
<?php print render($message); ?>
<?php endif; ?>
<?php print render($jobs); ?>