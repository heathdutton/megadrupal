<?php

/**
 * @file contest-list-page.tpl.php
 * Template for a contest's list page.
 *
 * Available variables:
 * - $data (array)
 * - - content (render array)
 */
?>
<div class="contest-list-page">
  
<?php foreach ($data as $content): ?>
  <?php print render($content); ?>
<?php endforeach; ?>

</div>
