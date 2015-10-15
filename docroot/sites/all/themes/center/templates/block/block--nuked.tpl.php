<?php

/**
 * @file
 * Custom theme implementation to display a block with no markup except for
 *   the title.
 *
 * @see template_preprocess()
 * @see template_preprocess_block()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<?php print $dev_deets; ?>
<?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
<?php print render($title_suffix); ?>
<?php print $content ?>
