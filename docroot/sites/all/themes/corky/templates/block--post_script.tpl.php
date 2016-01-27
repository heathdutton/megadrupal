<?php
// $Id: block--post_script.tpl.php,v 1.1 2010/11/16 05:25:24 chevy Exp $

/**
 * @file block--sidebar_first.tpl.php
 * Block tpl override for the sidebar first region
 */
?>
<div id="<?php print $block_html_id; ?>" style="padding: 0px; margin: 0px;" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
  <?php print render($title_suffix); ?>

  <div style="padding: 0px 5px; margin: 0px;" class="content"<?php print $content_attributes; ?>>
    <?php print $content ?>
  </div>
</div>
