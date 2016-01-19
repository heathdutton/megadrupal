<?php
/**
 * @file
 * My Episodes My List item default template.
 */
?>
<div<?php print $attributes; ?>>
  <h4>
    <span<?php print $title_attributes; ?>><?php print $title; ?></span>
    - <strong><?php print $episode_number; ?></strong>
    - <cite><?php print $episode_title; ?></cite>
  </h4>
  <?php if (!empty($links)): ?>
  <?php print render($links); ?>
  <?php endif; ?>
</div>