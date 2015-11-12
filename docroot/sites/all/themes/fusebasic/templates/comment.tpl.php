<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="comment-inner">

    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
    <?php print render($title_suffix); ?>

    <?php if ($new): ?>
      <span class="new"><?php print $new; ?></span>
    <?php endif; ?>

    <div class="submitted">

      <?php if ($picture): ?>
        <span class="user-picture">
          <?php print $picture; ?>
        </span>
      <?php endif; ?>

        <span class="created">Posted by <span class="commenter-name"><?php print $author; ?></span> - <span class="comment-time"><?php print $created;?></span></span>
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

    <?php print render($content['links']); ?>

  </div> <!-- /comment-inner -->
</div> <!-- /comment -->