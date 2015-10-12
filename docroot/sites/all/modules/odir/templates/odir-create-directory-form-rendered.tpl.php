<?php 
/**
 * @file
 * Prints a form for adding new directories
 * and a button for showing/hiding the block.
 */
?>

<div style="display:none;" id="odir_inline_directory_placeholder" ></div>

<div style="display:none;" id="odir_inline_directory_mask">
  <?php print $form ?>
</div>

<div style="display:none;" id="odir_inline_directory_mask_selector_div">
  <a id="odir_inline_directory_mask_selector_btn" href="#">[<?php print t("Add new directory")?>]</a>
</div>
