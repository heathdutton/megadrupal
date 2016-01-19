<?php

/**
 * @file relevant_content_block.tpl.php
 * Theme template for the block output
 */

// Do not print anything if empty
if ($is_empty) {
  return;
}
?>
<?php if ($header): ?>
<div class="header-text">
  <?php print $header; ?>
</div>
<?php endif; ?>

<div class="item-list">
  <ul>
  <?php foreach ($nodes as $node): ?>
    <li class="<?php print implode(' ', $node['classes']); ?>"><?php print $node['output']; ?></li>
  <?php endforeach; ?>
  </ul>
</div>
