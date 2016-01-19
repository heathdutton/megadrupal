<?php

/**
 * @file
 * Default theme implementation to display a sharedcontent.
 *
 */

hide($content['comments']);
hide($content['links']);
?>
<div id="sharedcontent-<?php print $uuid; ?>"
     class="<?php print $classes; ?> !article_classes clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
    <h2<?php print $title_attributes; ?>>
        !open_link<?php print $title; ?>!close_link</h2>
  <?php print render($title_suffix); ?>

    <div class="content"<?php print $content_attributes; ?>>
      <?php  print render($content); ?>
    </div>
</div>
