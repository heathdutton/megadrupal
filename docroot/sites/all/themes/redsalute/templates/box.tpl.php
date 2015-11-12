<?php

/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */
?>
<div class="box">
<?php if ($title): ?>
<h2 class="title"><?php print $title; ?></h2>
<?php endif; ?>
<div class="content"><?php print $content; ?></div>
</div>
