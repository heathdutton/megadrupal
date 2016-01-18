<?php
/**
 * @file
 * Template for Blog Post node in Full view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <h1 class="blog-post__title clear-margin--top margin--md--bottom hidden-sm hidden-md hidden-lg">
    <?php print $title; ?>
  </h1>

  <div class="row margin--lg--bottom">
    <div class="col-sm-5">
      <?php if (!empty($content['field_blog_post_featured_image'])): ?>
        <div class="blog-post__image margin--sm--bottom">
          <?php print render($content['field_blog_post_featured_image']); ?>
        </div>
      <?php endif; ?>
    </div>
    <div class="col-sm-7">
      <h1 class="blog-post__title clear-margin--top margin--sm--bottom hidden-xs">
        <?php print $title; ?>
      </h1>
      <?php if (!empty($content['posted_date'])): ?>
        <span class="blog-post__date">
          <?php print render($content['posted_date']); ?>
        </span>
      <?php endif; ?>

      <?php if (!empty($content['body'])): ?>
        <div class="blog-post__body margin--md--top">
          <?php print render($content['body']); ?>
        </div>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($extra); ?>
</article>
