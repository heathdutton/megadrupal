<?php

/**
 * @file
 * Example tpl file for theming a single swortoon-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $swortoon: The Model object this status is derived from
 */
?>

<div class="swortoon-status">
  <?php print '<strong>Model Sample Data:</strong> ' . $wowguild_sample_data = ($wowguild_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>