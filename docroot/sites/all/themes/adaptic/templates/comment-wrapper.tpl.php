<?php
/**
 * @file
 * adaptIC's implementation to display a comment.
 */

 /**
  * Render the comments and form first to see if we need headings.
  */
$comments = render($content['comments']);
$comment_form = render($content['comment_form']);
?>

<div id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($comments && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    <h2 class="title"><?php print t('Comments'); ?></h2>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

  <?php print $comments; ?>

  <?php if ($comment_form): ?>
    <h2 class="title comment-form"><?php print t('Add new comment'); ?></h2>
    <?php print $comment_form; ?>
  <?php endif; ?>
</div>
