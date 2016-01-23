<?php
/**
 * Comment Wrapper Template
 * @theme Boot Press
 * @author Pitabas Behera
 *
 **/
?>
<section id="comments" class="animated fadeInDown <?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($content['comments']): ?>
    <?php print render($title_prefix); ?>
    <h2 class="title">
    <?php print (_boot_press_comment_count($content['comments'])) ? _boot_press_comment_count($content['comments']) . t(' Comments') : t('Comments'); ?>
    </h2>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

  <?php if ($content['comment_form']): ?>
    <h2 class="title comment-form"><?php print t('Add new comment'); ?></h2>
    <?php print render($content['comment_form']); ?>
  <?php endif; ?>
</section>
