<?php

/**
 * @file
 * Template file for taxonomy term.
 */

?>

<div class="<?php print $classes; ?>">
  <?php if (!$page): ?>
    <h2><a href="<?php print $term_url; ?>"><?php print $term_name; ?></a></h2>
  <?php endif; ?>

  <?php print render($content); ?>
</div>
