
<div id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($content['comments'] && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    <h2 class="title"> <?php print $node->comment_count; ?> <?php print t('Responses'); ?></h2>
    <?php print render($title_suffix); ?> 
    <div class="comment-text">
      <?php print render($content['comments']); ?>
    </div>
   <?php endif; ?>
  <?php if ($content['comment_form']): ?>
    <h2 class="title comment-form"><?php print t('Add new comment'); ?></h2>
    <?php print render($content['comment_form']); ?>
  <?php endif; ?>
</div>
