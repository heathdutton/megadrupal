<?php

/**
 * @file comment-wrapper.tpl.php
 */
?>
<div<?php print $attributes; ?>>
  <?php if ($title): ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
    
  <h2 class="title-comments">Comments</h2>
  <?php print $comment_count; ?>
  <?php print $content; ?>
</div>
