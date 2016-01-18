<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $picture ?>

  <?php if ($new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>

  <header>
    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
    <?php print render($title_suffix); ?>

    <div class="submitted">
      <?php print $permalink; ?>
      <?php if ($author): ?>
        <div class="author">
          <?php print $author; ?>
        </div>
      <?php endif; ?>
      <?php if ($submitted_date): ?>
        <div class="submitted-date">
          <?php print $submitted_date; ?>
        </div>
      <?php endif; ?>
    </div>
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

  <?php print render($content['links']) ?>
</article>
