<?php
// $Id: block--sidebar_second.tpl.php,v 1.1 2010/11/16 05:25:24 chevy Exp $

/**
 * @file block--sidebar_first.tpl.php
 * Block tpl override for the sidebar first region
 */

global $base_url;
?>

<div id="<?php print $block_html_id; ?>" class=" <?php print $classes; ?>"<?php print $attributes; ?>>

<div class="postit-t"><div class="postit-b"><div class="postit-l"><div class="postit-r">
<div class="postit-bl"><div class="postit-br"><div class="postit-tl"><div class="postit-tr">
<div class="postit-inner">
  <div style="background: url(<?php print $base_url . '/' . path_to_theme() . '/images/pushpin' . rand(1, 4) . '.png'; ?>) no-repeat; width: 32px; height: 32px; margin-left: <?php print rand(10, 100); ?>px;"></div>
  <?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
  <?php print render($title_suffix); ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print $content ?>
  </div>

</div></div></div></div></div></div></div></div></div>

</div>
