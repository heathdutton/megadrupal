<?php

/**
 * @file
 * Theme implementation for comments.
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_comment()
 * @see template_process()
 * @see theme_comment()
 *
 * @ingroup themeable
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="submitted">
    <?php print $picture; ?>
    <span class="author"><?php print $author; ?></span>
    <p class="date"><?php print format_date($comment->created, 'medium'); ?></p>
  </div>

  <div class="content">
    <h4<?php print $title_attributes; ?>><?php print $title ?></h4>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
    <?php print render($content['links']) ?>
  </div>
</div>
