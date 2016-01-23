<?php ?>

<div id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($content['comments'] && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    <h3 class="title"><?php print t('Comments'); ?></h3>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

  <?php if ($content['comment_form']): ?>
    <h4 class="title comment-form"><?php print t('Add new comment'); ?></h4>
    <?php print render($content['comment_form']); ?>
  <?php endif; ?>
</div>