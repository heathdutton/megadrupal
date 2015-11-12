<?php
/**
 * @file
 * Panel region template
 *
 * Variables available:
 * - $vars: Array of all available pane variables
 * - $pane: String of rendered panes
 * - $wrapper_classes: Array of wrapper classes
 * - $wrapper_class: String with wrapper class
 */
?>
<div class="<?php print $wrapper_class; ?>">
  <?php print $pane; ?>
</div>
