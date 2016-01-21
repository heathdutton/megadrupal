<?php

/**
 * @file simplyhired-location.tpl.php
 * Default theme implementation for a job location.
 * 
 * Available variables:
 * - $source: The two letter country code for the API source used.
 * - $city:
 * - $state:
 * - $province:
 * - $postal_code:
 * - $county:
 * - $country:
 * - $raw: The raw string value for the location. Usually in the form "City, State" 
 * 
 */

?>
<div class="simplyhired-job-location simplyhired-job-location-<?php print $source; ?>">
	<?php print t('Location'); ?>:
	<span class="simplyhired-job-location-city"><?php print $city; ?></span>,
	<span class="simplyhired-job-location-province"><?php print $province; ?></span>
</div>