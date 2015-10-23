<?php
/**
 * @file
 * Default theme implementation for Fullscreen gallery.
 *
 * Available variables:
 * - $counter: The image counter string in format  "current/all".
 * - $thumbnails: The HTML output of thumbnail images.
 * - $back_link: Link to close the gallery.
 * - $prev: Link to go back to previous image.
 * - $next: Link to go forward to next image.
 * - $image: The rendered image.
 * - $image_title: The (sanitized) image title.
 * - $error: The error string, if exists.
 * - $fullscreen_gallery_right: The HTML output of Fullscreen gallery
 *   right sidebar, if exists.
 *
 * Other variables:
 * - $main_classes: Array of html class attribute values.
 *
 * @see template_preprocess()
 * @see template_preprocess_fullscreen_gallery()
 * @see template_process()
 */
?>
<div id="fullscreen_gallery" class="<?php print $main_classes; ?>">
  <div class="gallery-top">
    <div class="inner">
      <div class="counter">
        <?php print $counter; ?>
      </div>
      <div class="gallery-title">
        <div class="ginner">
          <div class="thumbnails">
            <div class="left inactive">
              <a href="#"></a>
            </div>
            <div class="thumbnails-inner">
              <div class="thumbnails-images">
                <?php print $thumbnails; ?>
                <div class="clearfix"></div>
              </div>
            </div>
            <div class="right">
              <a href="#"></a>
            </div>
          </div>
        </div>
      </div>
      <div class="back-button">
        <?php print $back_link; ?>
      </div>
      <div class="clearfix"></div>
    </div>
  </div>
  <div class="gallery-content">
    <div class="inner">
      <div class="gallery-left">
        <div class="main-content">
          <div id="fs_loading"></div>
          <div class="img_with_nav">
            <div class="nav">
              <div class="prev-button">
                <?php print $prev; ?>
              </div>
              <div class="next-button">
                <?php print $next; ?>
              </div>
            </div>
            <div class="current-image">
              <div class="cinner">
                  <div class="img-container">
                    <?php print $image; ?>
                    <?php if ($error): ?>
                      <h3><?php print $error; ?></h3>
                    <?php endif; ?>
                  </div>
                <div class="image-title">
                  <?php print $image_title; ?>
                </div>
              </div>
            </div>
          </div>
          <div class="clearfix"></div>
        </div>
      </div>
      <div class="gallery-right">
        <?php print $fullscreen_gallery_right; ?>
      </div>
      <div class="clearfix"></div>
    </div>
  </div>
</div>
