<?php

/**
 * @file
 * Custom template file for all comments
 */
?>

<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ($comment->status == COMMENT_NOT_PUBLISHED) ? ' comment-unpublished' : ''; ?> clear-block">

  <?php if ($picture) {print $picture;} ?>

  <h3 class="title">
    <?php print $title ?>
  </h3>

  <?php if ($new != '') { ?>
    <span class="new">
      <?php print $new; ?>
    </span>
  <?php } ?>

  <div class="submitted">
    <?php print $submitted ?>
  </div>

  <div class="content">
    <?php print $content ?>
  </div>

  <?php if ($signature): ?>
    <div class="user-signature clear-block">
      <?php print $signature ?>
    </div>
  <?php endif; ?>

  <div class="links">
    &raquo;<?php print $links ?>
  </div>

</div>
