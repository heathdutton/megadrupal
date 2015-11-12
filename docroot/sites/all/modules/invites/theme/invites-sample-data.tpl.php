<?php

/**
 * @file
 * Example tpl file for theming a single invites-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $invites: The Invites object this status is derived from
 */
?>

<div class="invites-status">
  <?php print '<strong>Invites Status:</strong> ' . $invites_data = ($invites->status) ? 'Switch On' : 'Switch Off' ?>
</div>