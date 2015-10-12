<?php
// $Id: comment.tpl.php,v 1.2 2011/02/14 00:32:25 himerus Exp $

/**
 * @file
 * Default theme implementation for comments.
 *
 * @see template_preprocess()
 * @see template_preprocess_comment()
 * @see template_process()
 * @see theme_comment()
 */
?>
<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>
  <header>
    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
    <?php print render($title_suffix); ?>
  </header>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
    <?php if ($signature): ?>
    <div class="user-signature clearfix">
      <?php print $signature ?>
    </div>
    <?php endif; ?>
  </div>
  <footer class="clearfix comment-info">
    <?php print $picture; ?>
    <div class="submitted"><?php print $submitted; ?></div>
    <?php print render($content['links']) ?>
  </footer>
</article>
