<?php

/**
 * @file
 * New York State Global Navigation Footer Social Media.
 *
 * See https://github.com/ny/global-navigation for documentation.
 *
 * Variables:
 * - social_media_urls, array of social media urls.
 *
 * @ingroup themeable
 */
?>
<div class="social-media">
  <div class="social-media-title">
    <h4>CONNECT WITH US</h4>
  </div>
  <div class="social-media-links">
    <ul>
    <?php print theme('nys_global_nav_footer_social_media_urls', array(
        'social_media_urls' => $social_media_urls,
      )
    );
    ?>
    </ul>
  </div>
</div>
