<?php
/**
 * @file
 * Template file to display lists.
 */
?>
<div class="wunderlist-list">
<?php foreach ($list as $item) : ?>
  <h2><?php print $item['title']; ?></h2>
  <?php print $item['tasks']; ?>
<?php endforeach; ?>
</div>
