<?php
// $Id: block--sidebar_first.tpl.php,v 1.1 2010/10/23 05:44:17 chevy Exp $

/**
 * @file block--sidebar_first.tpl.php
 * Block tpl override for the sidebar first region
 */
?>

<div id="<?php print $block_html_id; ?>" class=" <?php print $classes; ?>"<?php print $attributes; ?>>

<div class="postit-t"><div class="postit-b"><div class="postit-l"><div class="postit-r">
<div class="postit-bl"><div class="postit-br"><div class="postit-tl"><div class="postit-tr">
<div class="postit-inner">

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
