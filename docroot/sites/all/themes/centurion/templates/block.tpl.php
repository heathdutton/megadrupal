<?php
/**
 * @file
 * This is custom block.tpl.php for Centurion Framework
 */
?>

<section id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>> <?php print render($title_prefix); ?>
	<header>
		<?php if ($block->subject): ?>
		<h3<?php print $title_attributes; ?>><?php print $block->subject ?></h3>
    <?php endif;?>
  </header>
  <?php print render($title_suffix); ?>
  <div class="content"<?php print $content_attributes; ?>> <?php print $content ?> </div>
</section>
<div class="clear"></div>
