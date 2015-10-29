<?php

/**
 * @file contest-list-block.tpl.php
 * Template for a contest's list block.
 *
 * Available variables:
 * - $data (array)
 * - - content (render array)
 */
?>
<?php foreach ($data as $content): ?>
  <?php print render($content); ?>
<?php endforeach; ?>

