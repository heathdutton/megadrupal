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
  $account = user_load(array('uid'=>$comment->uid));
?>
<div class="comment<?php print render($content['comments']) ? ' comment-new' : ''; print ' '. $status; ?> clearfix">
  <div class="comment-header clearfix">
    <div class="submitted"><?php print $submitted; ?></div>
    <div class="comment-link"><?php print $permalink; ?></div>
  </div>
  
  <div class="author-data">
    <?php print $picture; ?>
	  <?php if (!$account->created == 0) { ?>
      <div class="user-member"><?php print t('<strong>Member since:</strong><br /> !date', array('!date' => format_date($account->created, 'custom', 'j F Y'))); ?></div>  
      <div class="user-access"><?php print t('<strong>Last activity:</strong><br /> !ago', array('!ago' => format_interval(time() - $account->access))); ?></div>  
    <?php } ?>
  </div>

  <div class="comment-main">
    <?php if ($content['comments']): ?>
      <span class="new"><?php print render($content['comments']); ?></span>
    <?php endif; ?>

    <div class="content">  
        <?php hide($content['links']); ?>
        <?php print render($content); ?>
        <?php if ($signature): ?>
        <div class="user-signature clearfix">
          <div>&mdash;</div>
          <?php print $signature; ?>
        </div>
        <?php endif; ?>
    </div>
  </div>
  <?php print render($content['links']); ?>
</div>
