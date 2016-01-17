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

<?php if ($wrapper): ?><div<?php print $attributes; ?>><?php endif; ?>
  <div<?php print $content_attributes; ?>>
    <?php if ($messages): ?>
      <div id="messages" class="grid-<?php print $columns; ?>"><?php print $messages; ?></div>
    <?php endif; ?>
    <?php print $content; ?>
  </div>
<?php if ($wrapper): ?></div><?php endif; ?>
