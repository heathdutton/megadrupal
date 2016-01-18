<?php

/**
 * @file
 * Template for PageFlip HTML/JavaScript viewer.
 */

?>
<div class="pageflip-html-viewer-container">
  <div class="pageflip-html-viewer-content-container"<?php print $content_container_attributes; ?>>
    <?php print $content; ?>
  </div>

  <div id="pageflip-control-bar">
    <div id="pageflip-status">
      <span class="pageflip-status-message">You are currently reading</span> <span class="pageflip-title"><?php print $title; ?></span>
    </div>
    <div id="pageflip-navigation">
      <div id="pageflip-share">
        <?php print $share_widget; ?>
      </div>
      <div id="pageflip-switch-link">
        <?php print $switch_link; ?>
      </div>
      <a class="pageflip-navigation-prev" href="#"><span class="arrow">&nbsp;</span><span class="text">Previous Page</span></a>
      <?php print render($chapter_form); ?>
      <a class="pageflip-navigation-next" href="#"><span class="text">Next Page</span><span class="arrow">&nbsp;</span></a>
      <div id="pageflip-site-link">
        <?php print $site_link; ?>
      </div>
    </div>
  </div>
</div>
