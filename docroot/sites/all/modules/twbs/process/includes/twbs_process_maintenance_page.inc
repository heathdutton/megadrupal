<?php

/**
 * Theme process function for theme_maintenance_field().
 *
 * The variables array generated here is a mirror of twbs_process_html().
 * This processor will run its course when theme_maintenance_page() is invoked.
 *
 * @see maintenance-page.tpl.php
 * @see twbs_process_html()
 */
function twbs_process_maintenance_page(&$variables) {
  $variables['head']    = drupal_get_html_head();
  $variables['css']     = drupal_add_css();
  $variables['styles']  = drupal_get_css();
  $variables['scripts'] = drupal_get_js();
}
