<?php
/**
 * @file
 * My Episodes My List default template.
 */
?>
<div<?php print $attributes; ?>>
  <span<?php print $title_attributes; ?>></span>
  <?php if (!empty($links)): ?>
  <?php print render($links); ?>
  <?php endif; ?>
  <?php if (!empty($items)): ?>
  <ul>
  <?php foreach ($items as $item): ?>
    <li><?php print render($item); ?></li>
  <?php endforeach; ?>
  </ul>
  <?php else: ?>
  <p><?php print t("This user list has no unseen episodes."); ?></p>
  <?php endif; ?>
</div>