<?php

/**
 * @file
 * Theme implementation for open graph comment.
 */
?>
<div class="open-graph-comment-outer">
  <?php if ($og_data['img']): ?>
    <div class="open-graph-comment-img">
      <img src='<?php print $og_data['img']; ?>'>
    </div>
  <?php endif; ?>
  <?php if ($og_data['url'] && $og_data['title']): ?>
    <h3 class="open-graph-comment-title">
      <a href="<?php print $og_data['url']; ?>" target="_blank"><?php print $og_data['title']; ?></a>
    </h3>
  <?php endif; ?>
  <?php if ($og_data['desc']): ?>
    <p class="open-graph-comment-desc"><?php print $og_data['desc']; ?></p>
  <?php endif; ?>
</div>
