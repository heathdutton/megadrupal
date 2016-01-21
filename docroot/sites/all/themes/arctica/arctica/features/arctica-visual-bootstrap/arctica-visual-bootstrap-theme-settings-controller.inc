<?php

if (theme_get_setting('aavb_forms')) {
  drupal_add_css($arctica_theme_path . '/features/arctica-visual-bootstrap/css/forms.css');
}

if (theme_get_setting('aavb_navigation')) {
  drupal_add_css($arctica_theme_path . '/features/arctica-visual-bootstrap/css/navigation.css');
}

if (theme_get_setting('aavb_messages')) {
  drupal_add_css($arctica_theme_path . '/features/arctica-visual-bootstrap/css/messages.css');
  if (count(drupal_get_messages(NULL, FALSE)) > 0) {
    drupal_add_js('jQuery(document).ready(function() { jQuery(".message").append("<div class=\"close\">x</div>").find(".close").click(function() {jQuery(this).parent(".message").fadeOut("fast")});});', 'inline');
  }
}

if (theme_get_setting('aavb_print')) {
  drupal_add_css($arctica_theme_path . '/features/arctica-visual-bootstrap/css/print.css');
}