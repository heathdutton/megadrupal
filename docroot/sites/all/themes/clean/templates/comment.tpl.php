<?php
/**
 * @file
 * Default theme implementation to display a comment.
 */
?>
<div<?php print $attributes; ?>>
  <?php print $picture; ?>

  <?php if ($new): ?>
    <span class="new"><?php print $new; ?></span>
  <?php endif; ?>

  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <div class="submitted">
    <?php print $permalink; ?>
    <?php print $submitted; ?>
  </div>

  <div<?php print $content_attributes; ?>>
    <?php
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php if ($signature): ?>
    <div class="user-signature clearfix">
      <?php print $signature; ?>
    </div>
  <?php endif; ?>

  <?php print render($content['links']); ?>
</div>
