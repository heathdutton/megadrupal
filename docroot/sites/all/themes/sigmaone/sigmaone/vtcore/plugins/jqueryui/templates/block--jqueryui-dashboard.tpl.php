<?php
/**
Available variables:

    * $block->subject: Block title.
    * $content: Block content.
    * $block->module: Module that generated the block.
    * $block->delta: An ID for the block, unique within each module.
    * $block->region: The block region embedding the current block.
    * $classes: String of classes that can be used to style contextually through CSS. It can be manipulated through the variable $classes_array from preprocess functions. The default values can be one or more of the following:
          o block: The current template type, i.e., "theming hook".
          o block-[module]: The module generating the block. For example, the user module is responsible for handling the default user navigation block. In that case the class would be "block-user".
    * $title_prefix (array): An array containing additional output populated by modules, intended to be displayed in front of the main title tag that appears in the template.
    * $title_suffix (array): An array containing additional output populated by modules, intended to be displayed after the main title tag that appears in the template.

Helper variables:

    * $classes_array: Array of html class attribute values. It is flattened into a string within the variable $classes.
    * $body_classes_array: Array of body class attribute values. It is flattened into a string within the variable $body_classes
    * $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
    * $zebra: Same output as $block_zebra but independent of any block region.
    * $block_id: Counter dependent on each block region.
    * $id: Same output as $block_id but independent of any block region.
    * $is_front: Flags true when presented in the front page.
    * $logged_in: Flags true when the current user is a logged-in member.
    * $is_admin: Flags true when the current user is an administrator.
    * $block_html_id: A valid HTML ID and guaranteed unique.
**/
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> <?php print $classes;?>">
  <?php // Print the suffix and prefix before accordion header to avoid js confusion ?>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix);?>

  <?php if (!empty($block->subject)): ?>
  <h3<?php print $title_attributes; ?>>
  <span class="ui-icon ui-icon-circle-arrow-s"></span>
  <a href="#" class="header"><?php print $block->subject; ?></a>
  </h3>
  <?php endif;?>

  <div class="block-content <?php print $body_classes;?>"><?php print $content; ?></div>
</div>
