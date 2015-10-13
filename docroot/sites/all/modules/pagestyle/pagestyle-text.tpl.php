<?php
// $Id: pagestyle-text.tpl.php,v 1.2 2009/11/17 23:15:33 christianzwahlen Exp $

/**
 * @file
 * Default theme implementation for rendering a block.
 *
 * Available variables:
 * - $dest: The drupal function drupal_get_destination().
 * - $block_title: Block title: "Page Style" or "Theme".
 * - $list_inline: Display the ul list "inline" or not.
 * - $current_inline: Display the current page style "inline" or not.
 * - $subtitle: The subtitle.
 * - $subtitle_text: "Page Style"/"Theme" or inline "Page Style: "/"Theme: ".
 * - $display_links: Show or hide ("display_hidden") the text in the links.
 * - $black_white: The pagestyle Black/White "black_white".
 * - $white_black: The pagestyle White/Black "white_black".
 * - $yellow_blue: The pagestyle Yellow/Blue "yellow_blue".
 * - $standard: The pagestyle Standard "standard".
 * - $current_pagestyle: The current page style.
 * - $display_current_pagestyle: Show or hide ("display_hidden") the text of the current page style.
 * - $display_current_pagestyle_text: Show or hide ("display_hidden");
 * - $current_pagestyle_text: The current pagestyle text "Current Style" or "Current Theme".
 * - $pagestyle: The current page style (default Standard).
 *
 * @see template_preprocess_pagestyle_text()
 */
?>
<?php  if ($subtitle): ?>
<h3 class="<?php print $list_inline; ?>"><?php print $subtitle_text; ?></h3>
<?php endif; ?>
<ul class="pagestyle_<?php print $list_inline .'" id="'. $current_inline;  ?>">
  <?php  if ($black_white): ?>
  <li class="ps_black_white"><?php print l('<span class="'. $display_links .'">'. t('Black') .'/'. t('White') .'</span>', 'pagestyle/black_white', array('attributes' => array('title' => $block_title .': '. t('Black') .'/'. t('White'), 'class' => 'ps_icon ps_black_white pagestyle_black_white text_'. $display_links), 'query' => $dest, 'html' => TRUE)); ?></li>
  <?php endif; ?>
  <?php  if ($white_black): ?>
  <li class="ps_white_black"><?php print l('<span class="'. $display_links .'">'. t('White') .'/'. t('Black') .'</span>', 'pagestyle/white_black', array('attributes' => array('title' => $block_title .': '. t('White') .'/'. t('Black'), 'class' => 'ps_icon ps_white_black pagestyle_white_black text_'. $display_links), 'query' => $dest, 'html' => TRUE)); ?></li>
  <?php endif; ?>
  <?php  if ($yellow_blue): ?>
  <li class="ps_yellow_blue"><?php print l('<span class="'. $display_links .'">'. t('Yellow') .'/'. t('Blue') .'</span>', 'pagestyle/yellow_blue', array('attributes' => array('title' => $block_title .': '. t('Yellow') .'/'. t('Blue'), 'class' => 'ps_icon ps_yellow_blue pagestyle_yellow_blue text_'. $display_links), 'query' => $dest, 'html' => TRUE)); ?></li>
  <?php endif; ?>
  <?php  if ($standard): ?>
  <li><?php print l('<span class="'. $display_links .'">'. t('Standard') .'</span>', 'pagestyle/standard', array('attributes' => array('title' => $block_title .': '. t('Standard'), 'class' => 'ps_icon ps_standard pagestyle_standard text_'. $display_links), 'query' => $dest, 'html' => TRUE)); ?></li>
  <?php endif; ?>
</ul>
<?php  if ($current_pagestyle): ?>
<p class="pagestyle_current <?php print $current_inline .' '. $display_current_pagestyle .' current_text_'. $display_current_pagestyle_text .' text_'. $display_links; ?>"><span class="<?php print $display_current_pagestyle_text; ?>"><?php print $current_pagestyle_text; ?>: </span><span id="pagestyle_current" title="<?php print $current_pagestyle_text .': '. $pagestyle; ?>"><?php print $pagestyle; ?></span></p>
<?php endif; ?>
<div class="ps_clear"></div>