<?php
/**
 * @file
 * Pane messages template.
 */
?>
<?php if (!empty($tabs)): ?>
  <div class="tabs"><?php print render($tabs); ?></div>
<?php endif; ?>
<?php if ($action_links): ?>
  <ul class="action-links">
    <?php print render($action_links); ?>
  </ul>
<?php endif; ?>
<?php print $messages; ?>
<?php print $help; ?>
