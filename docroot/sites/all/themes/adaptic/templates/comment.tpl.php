<?php
/**
 * @file
 * adaptIC's implementation to display a comment.
 */

 ?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $picture; ?>

  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h3<?php print $title_attributes; ?>>
      <?php print $title; ?>
      <?php if ($new): ?>
        <span class="new"><?php print $new; ?></span>
      <?php endif; ?>
    </h3>
  <?php elseif ($new): ?>
    <div class="new"><?php print $new; ?></div>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($status == 'comment-unpublished'): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <div class="submitted">
    <?php print $permalink; ?>
    <?php print $submitted; ?>
  </div>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
    <?php if ($signature): ?>
      <div class="user-signature clearfix">
        <?php print $signature; ?>
      </div>
    <?php endif; ?>
  </div>

  <?php print render($content['links']) ?>
</div><!-- /.comment -->
