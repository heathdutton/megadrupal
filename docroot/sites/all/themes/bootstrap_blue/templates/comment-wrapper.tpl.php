<?php
/**
 * @file
 * Comment wrapper comment-wrapper.tpl override from Bootstrap parent theme
 */
?>
<section id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($content['comments'] && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    <h2 class="comments-title title">
      <?php if ($node->comment == 1 || $node->comment == 2): ?>
        <?php if ($node->comment_count == 0): ?>
        <?php else: ?>
          <?php print t('@comments for @title', array(
            '@comments' => format_plural($node->comment_count, '1 comment', '@count comments'),
            '@title' => (check_plain($node->title)))); ?> <a class="btn pull-right" href="#comment-form" title="Add comment"><i class="icon-plus"></i></a>
        <?php endif; ?>
      <?php endif; ?>
    </h2>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

<div class="comments-content">
  <?php print render($content['comments']); ?>
  </div>

  <?php if ($content['comment_form']): ?>
    <section id="comment-form-wrapper">
      <h2 class="title element-invisible"><?php print t('Add new comment'); ?></h2>
      <?php print render($content['comment_form']); ?>
    </section> <!-- /#comment-form-wrapper -->
  <?php endif; ?>
</section> <!-- /#comments -->
