<?php
/**
 * @file
 * Comment comment.tpl override from Bootstrap parent theme
 */
?>
<article class="<?php print $classes . ' ' . $zebra; ?>"<?php print $attributes; ?>>
  <header>
    <?php print $picture; ?>
    <?php print render($title_prefix); ?>
    <h3 class="comment-title element-invisible"<?php print $title_attributes; ?>><?php print $title; ?></h3>
    <?php print render($title_suffix); ?>
    <?php if ($submitted): ?>
      <div class="submitted">
        <?php if ($new): ?>
          <span class="new label label-important"><?php print $new; ?></span>
        <?php endif; ?>
        <?php print t('<span class="label">!author</span> on !date', array(
          '!date' => format_date($comment->created, 'short'),
          '!author' => $author)); ?>
        <span class="new label label-info"><?php print $permalink; ?></span>
      </div>
    <?php endif; ?>
  </header>
  <div class="comment-content content"<?php print $content_attributes; ?>>
    <?php
    hide($content['links']);
    print render($content);
    ?>
    <?php if ($signature): ?>
      <div class="user-signature clearfix">
        <?php print $signature; ?>
      </div>
    <?php endif; ?>
  </div>
  <?php if ($content['links']): ?>
    <div class="comment-links clearfix">
      <?php print render($content['links']); ?>
    </div>
  <?php endif; ?>
</article> <!-- /.comment -->
