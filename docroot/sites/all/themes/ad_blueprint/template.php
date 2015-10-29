<?php
/**
* Override or insert PHPTemplate variables into the templates.
*/
function phptemplate_preprocess_page(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}
