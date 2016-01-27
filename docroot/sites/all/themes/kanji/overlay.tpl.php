<div id="overlay" <?php print $attributes; ?>>
  <div id="overlay-titlebar" class="clearfix">
    <div id="overlay-title-wrapper" class="clearfix">
      <h1 id="overlay-title"<?php print $title_attributes; ?>><?php print $title; ?></h1>
    </div>
    <div id="overlay-close-wrapper">
      <a id="overlay-close" href="#" class="overlay-close"><span class="element-invisible"><?php print t('Close overlay'); ?></span></a>
    </div>
    <?php if ($tabs): ?><h2 class="element-invisible"><?php print t('Primary tabs'); ?></h2><ul id="overlay-tabs"><?php print render($tabs); ?></ul><?php endif; ?>
  </div>
  <div id="overlay-content"<?php print $content_attributes; ?>>
    <?php print $page; ?>
  </div>
</div>
