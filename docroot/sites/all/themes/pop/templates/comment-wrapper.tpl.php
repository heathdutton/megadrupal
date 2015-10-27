<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--comment--comment-wrapper.tpl.php/7
 */
?>
<section id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($content['comments'] && $node->type != 'forum'): ?>
    <?php print render($title_prefix); ?>
    <h2 class="title"><?php print t('Comments'); ?></h2>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

  <?php if ($content['comment_form']): ?>
    <section id="comment-form-wrapper">
      <h2 class="title"><?php print t('Add new comment'); ?></h2>
      <?php print render($content['comment_form']); ?>
    </section>
  <?php endif; ?>
</section>
