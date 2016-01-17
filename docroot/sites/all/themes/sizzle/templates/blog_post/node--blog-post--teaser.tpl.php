<?php
/**
 * @file
 * Template for Blog Post node in Teaser view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <div class="row">
    <div class="col-sm-5">
      <div class="blog-post__image margin--sm--bottom">
        <?php print render($content['field_blog_post_featured_image']); ?>
      </div>
    </div>
    <div class="col-sm-7">
      <?php if (!empty($content['posted_date'])): ?>
        <span class="blog-post__date">
          <?php print render($content['posted_date']); ?>
        </span>
      <?php endif; ?>

      <h1 class="blog-post__title clear-margin--top">
        <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
      </h1>

      <?php if (!empty($content['body'])): ?>
        <div class="blog-post__body margin--sm--top">
          <?php print render($content['body']); ?>
        </div>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($extra); ?>
</article>
