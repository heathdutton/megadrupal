<?php

/**
 * @file
 * Example tpl file for theming a single pollim-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $pollim: The pollim object this status is derived from
 */
?>

<div class="pollim-status">
  <?php print '<strong>pollim Sample Data:</strong> ' . $pollim_sample_data = ($pollim_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>