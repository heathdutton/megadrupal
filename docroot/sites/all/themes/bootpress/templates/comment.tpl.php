<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

<?php if($comment->picture && !is_numeric($comment->picture)): ?>
  <?php print render($variables['picture']); ?>
<?php endif; ?>

<div class="comment-detail-wrap clearfix">
  <header class="comment-header">
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h5<?php print $title_attributes; ?>>
        <?php print $title; ?>
        <?php if ($new): ?>
          <mark class="new label label-default"><?php print $new; ?></mark>
        <?php endif; ?>
      </h5>
    <?php elseif ($new): ?>
      <mark class="new label-default"><?php print $new; ?></mark>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    
    <p class="submitted">
      <?php print $submitted; ?>
      <?php print $permalink; ?>
    </p>
  </header>

  <?php
    // We hide the comments and links now so that we can render them later.
    hide($content['links']);
    print render($content);
  ?>
</div>

  <?php if ($signature): ?>
    <footer class="user-signature clearfix">
      <?php print $signature; ?>
    </footer>
  <?php endif; ?>

  <?php print render($content['links']) ?>
</article>
