<?php

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
?>
 <div class="comment <?php print comment_classes($comment) .' '. $zebra .' '. $status ?> clearfix">
    
  <?php if ($picture): ?>
    <?php print $picture ?>
  <?php endif; ?>

  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h3 class="title"><?php print $title ?>
      <?php if ($comment->new): ?><span class="new"><?php print $new ?></span><?php endif; ?>
    </h3>
  <?php endif; ?>
 <?php print render($title_suffix); ?>

  <div class="submitted">
    <?php print $submitted ?>
  </div>

  <div class="content">

    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
   
    


     <?php if ($signature): ?>
    <div class="user-signature clearfix">
      <?php print $signature ?>
    </div>
    <?php endif; ?>
  </div>

  <?php if ($content['links']): ?>
     <?php print render($content['links']) ?>
  <?php endif; ?>  
</div>