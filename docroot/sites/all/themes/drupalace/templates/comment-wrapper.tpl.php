<div id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($content['comments'] && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    <h2 class="title"><?php print t('Comments'); ?></h2>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

  <?php if (isset($content['comment_form'])): ?>

    <?php $comment_form = render($content['comment_form']); ?>
    <?php if (trim($comment_form)): ?>
      <h2 class="title comment-form"><?php print t('Add new comment'); ?></h2>
      <?php print $comment_form; ?>
    <?php endif; ?>

  <?php endif; ?>

</div>