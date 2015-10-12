<?php
/**
 * @file
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      hide($content['sharethis']);
      print render($content);
    ?>
  </div>
</div>
