<?php
/**
 * Theme the results of a quiz.
 * 
 * Available variables:
 *   $results: 					an array of result arrays
 *   $totals_view:			rendered HTML of the totals in the $results array
 *   $results_view:		  rendered HTML of the $results array
 */
?>
<div class='quizzler quizzler-results-report'>
	<div class='quizzler-results'>
  	<?php print $results_view; ?>
	</div>
	<div class='quizzler-totals'>
  	<?php print $totals_view; ?>
	</div>
</div>