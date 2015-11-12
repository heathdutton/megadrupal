<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--block--block.tpl.php/7
 */
?>

<section id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
  <?php endif;?>
  <?php print render($title_suffix); ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print $content ?>
  </div>

</section>
