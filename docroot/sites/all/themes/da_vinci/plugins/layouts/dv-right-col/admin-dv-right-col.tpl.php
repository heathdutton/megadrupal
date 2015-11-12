<?php
drupal_add_css(drupal_get_path('theme', 'da_vinci') . '/css/panel.css', array('group' => CSS_THEME, 'type' => 'file'));
/**
 * @file
 * Template for a custom layout: 3 columns and 9 columns in a 12 column susy framework
 */
?>
<div class="dv-right-col dv-pane">
  <div class="content">
    <?php print $content['content']; ?>
  </div>
  <div class="right-sidebar">
    <?php print $content['right']; ?>
  </div>
  <div class="footer">
    <?php print $content['footer']; ?>
  </div>
</div>
