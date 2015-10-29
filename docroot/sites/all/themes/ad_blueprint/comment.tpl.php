<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ($comment->status == COMMENT_NOT_PUBLISHED) ? ' comment-unpublished' : ''; ?> clear-block">
    <?php if ($picture) {
    print $picture;
  } ?>
<h3 class="title"><?php print $title; ?></h3><?php if ($new != '') { ?><span class="new"><?php print $new; ?></span><?php } ?>
    <div class="submitted"><?php print $submitted; ?></div>
    <br clear="all"/>
    <div class="content">
  <?php print $content ?>
  <?php if ($signature && $comment->cid > 3443): // The highest comment ID before upgrading to Drupal 6 ?>
    <div class="user-signature clear-block">
      <?php print $signature ?>
    </div>
  <?php endif; ?>
</div>
    <div class="links"><?php print $links; ?></div>
  </div>
