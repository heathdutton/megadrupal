<?php
drupal_add_js(drupal_get_path('module', 'print_anything') . '/inc/print_anything.js', array('type' => 'file', 'preprocess' => FALSE));
drupal_add_js(print_anything_print_getcssids(), array('type' => 'inline', 'preprocess' => FALSE));
?>
<div id="print-friendly-head" class="clearfix">
  <div id="print-friendly-close"><a href="javascript: self.close()">Close Window</a> | <span id="printLink"><a href="#">Print this page</a></span></div>
  <div id="print-friendly-logo"><?php print print_anything_header('logo'); ?></div>
  <div id="print-friendly-header"><?php print print_anything_header('header'); ?></div>
</div>
<div id="print-friendly-doc-body" class="clearfix">
</div>
<div id="print-friendly-footer">
<?php print print_anything_footer(); ?>
</div>