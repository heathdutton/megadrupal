<?php

/**
 * @file
 * provides all the basic functionality. However, in case you wish to customize
 * the output that Drupal generates through Alpha & Omega.
 * this file is a good place to do so.
 * Alpha comes with a neat solution for keeping this file as clean as possible
 * while the code for your subtheme grows.
 * Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */
?>

<?php $tag = $block->subject ? 'section' : 'div'; ?>
<<?php print $tag; ?><?php print $attributes; ?>>
  <div class="block-inner clearfix">  
    <div<?php print $content_attributes; ?>>
      <?php print $content ?>
    </div>
  </div>
</<?php print $tag; ?>>
