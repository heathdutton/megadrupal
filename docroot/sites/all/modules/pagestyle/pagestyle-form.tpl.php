<?php
// $Id: pagestyle-form.tpl.php,v 1.2 2009/11/17 23:15:33 christianzwahlen Exp $

/**
 * @file
 * Form theme implementation for rendering a block.
 *
 * Available variables:
 * - $list_inline: Display the ul list "inline" or not.
 * - $current_inline: Display the current page style "inline" or not.
 * - $current_pagestyle: The current page style.
 * - $display_current_pagestyle: Show or hide ("display_hidden") the text of the current page style.
 * - $display_current_pagestyle_text: Show or hide ("display_hidden").
 * - $pagestyle_form: The form.
 * - $current_pagestyle_text: The current page style text "Current Style" or "Current Theme".
 * - $pagestyle: The current page style (default Standard).
 *
 * @see template_preprocess_pagestyle_form()
 */
?>
<div id="pagestyle" class="pagestyle_<?php print $list_inline .' '. $current_inline; ?> container-inline">
  <?php print $pagestyle_form; ?>
  <?php  if ($current_pagestyle): ?>
  <p class="pagestyle_current <?php print $current_inline .' '. $display_current_pagestyle .' current_text_'. $display_current_pagestyle_text .' text_display'; ?>"><span class="<?php print $display_current_pagestyle_text; ?>"><?php print $current_pagestyle_text; ?>: </span><span id="pagestyle_current" title="<?php print $current_pagestyle_text .': '. $pagestyle; ?>"><?php print $pagestyle; ?></span></p>
  <?php endif; ?>
</div>
<div class="ps_clear"></div>