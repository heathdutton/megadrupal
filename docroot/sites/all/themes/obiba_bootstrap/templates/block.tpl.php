<?php
/**
 * @file
 * Code for block template in micado theme.
 */

?>

<section id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h4<?php print $title_attributes; ?>><?php print $title; ?></h4>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <div class="block-content">
    <?php print $content ?>
  </div>

</section> <!-- /.block -->
