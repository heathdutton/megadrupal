<?php
// $Id: block.tpl.php,v 1.6 2010/11/16 15:36:36 shannonlucas Exp $
/**
 * @file
 * Block rendering for Nitobe.
 */
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
  <?php endif;?>
  <?php print render($title_suffix); ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print $content ?>
  </div>
</div>
<hr class="break"/>
