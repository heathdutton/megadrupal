<?php
/**
 * @file
 * Template for Blog Post node in Featured view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <div class="row">
    <div class="col-xs-5">
      <?php if (!empty($content['field_blog_post_featured_image'])): ?>
        <div class="blog-post__image">
          <?php print render($content['field_blog_post_featured_image']); ?>
        </div>
      <?php endif; ?>
    </div>
    <div class="col-xs-7">
      <?php if (!empty($content['posted_date'])): ?>
        <span class="blog-post__date">
          <?php print render($content['posted_date']); ?>
        </span>
      <?php endif; ?>

      <h3 class="blog-post__title">
        <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
      </h3>

      <?php if (!empty($content['body'])): ?>
        <p class="blog-post__body">
          <?php print render($content['body']); ?>
        </p>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($extra); ?>
</article>
