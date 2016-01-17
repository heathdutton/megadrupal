<?php
/**
 * @file
 * Default theme implementation to display an election.
 */
?>
<div id="election-<?php print $election->election_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if (!$page): ?>
    <?php if (isset($title_prefix)): ?>
      <?php print render($title_prefix); ?>
    <?php endif; ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $election_url; ?>"><?php print $title; ?></a></h2>
    <?php if (isset($title_suffix)): ?>
      <?php print render($title_suffix); ?>
    <?php endif; ?>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

</div>
