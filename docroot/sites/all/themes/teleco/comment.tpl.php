<?php // $Id$

/**
 * @file
 * comment.tpl.php
 *
 * Theme implementation for comments.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
?>
<div class="comment <?php print $comment_classes; ?>">
  <div class="comment-inner-0"><div class="comment-inner-1">
    <div class="comment-inner-2"><div class="comment-inner-3">

    <?php if ($title): ?>
      <h3 class="title"><?php print $title; if (!empty($new)): ?> <span class="new"><?php print $new; ?></span><?php endif; ?></h3>
    <?php endif; ?>

    <?php if ($unpublished): ?>
      <div class="unpublished"><?php print t('Unpublished'); ?></div>
    <?php endif; ?>

    <div class="submitted"><?php print $submitted; ?></div>

    <?php print $picture; ?>

    <div class="content <?php print $picture ? 'with-picture' : 'no-picture' ; ?>">

      <?php print $content; ?>

      <?php if ($signature): ?>
        <div class="user-signature clear">
          <?php print $signature ?>
        </div>
      <?php endif; ?>

    </div>

    <?php if ($links): ?>
      <div class="links clearfix"><?php print $links; ?></div>
    <?php endif; ?>

    </div></div>
  </div></div>
</div> <!-- /comment -->