<?php

/**
 * @file
 * Default theme implementation for a link/button to update an issue.
 *
 * Available variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $update_link: Either a link to update the issue or a 'Login or register'
 *   set of links for anonymous users.
 * - $last_update: Text and formatted timestamp for the last issue update.
 *
 * @see template_preprocess_project_issue_issue_update_link()
 */
?>

<div class="issue-update">
 <?php if ($update_link): ?>
   <div class="update-link"><?php print $update_link;?></div>
 <?php endif; ?>
 <?php if ($last_update): ?>
   <div class="last-update"><?php print $last_update;?></div>
 <?php endif; ?>
</div>
