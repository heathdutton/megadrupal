<?php
/*
 * This template overrides the output of the main content block,
 * The main content block is the content of the $page['content']
 * in the page.tpl.php template.
 * This is used to lighten up the markup, but it is not mandatory
 * and can be removed. If you remove this template, the main content 
 * will heritate the markup in block.tpl.php
 *
 */
?>
<?php if (!empty($block->subject)): ?>
  <h3 class="title block-title main"><?php print $block->subject; ?></h3>
<?php endif; ?>

<?php print $content; ?>