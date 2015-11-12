<?php

global $base_path, $theme;
$abs_arctica_theme_path = $base_path . drupal_get_path('theme', 'arctica');
$vars['arctica_head'] = '';


if (theme_get_setting('futurecss_enable')) {
  /* This should be available before any CSS */
  $vars['arctica_head'] .= '<script>window.piePath = "' . $abs_arctica_theme_path . '/features/arctica-cssfuture/PIE' . '";</script>';
}

if (theme_get_setting('meta')) {
  $arctica_meta = theme_get_setting("meta") == 'RESET' ? theme_get_setting("meta1") . "\r\n" . theme_get_setting("meta2") . "\r\n" . theme_get_setting("meta3") . "\r\n" . theme_get_setting("meta4") . "\r\n" . theme_get_setting("meta5") . "\r\n" . theme_get_setting("meta6") : theme_get_setting("meta");
  $vars['arctica_head'] .= $arctica_meta;
} else {
  $vars['arctica_head'] .= '';
}

$cond_top = $cond_bottom = '';

if ((user_access('administer themes') OR module_exists('stdemo')) && (arg(1) != 'appearance') && (arg(2) != 'settings')) {
 $cond_bottom .= '<div class="arctica-configurator">' . l(t('Arctica Configurator'), 'admin/appearance/settings/' . $theme, array('attributes' => array('title' => t('Arctica Configurator'), 'rel' => 'nofollow'))) . '</div>';
}

if (theme_get_setting('html_polyfill')) {
  $cond_top .= '<!--[if (lt IE 9) ]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->';
}
if (theme_get_setting('responsive_enable') && theme_get_setting('responsive_polyfill')) {
  $cond_top .= '<!--[if (lt IE 9) & (!IEMobile)]><script src="' . $abs_arctica_theme_path . '/scripts/polyfills/respond.min.js"></script><![endif]-->';
}
if (theme_get_setting('responsive_enable') && theme_get_setting('responsive_polyfill2')) {
  $cond_top .= '<!--[if (lt IE 9) & (!IEMobile)]><script src="' . $abs_arctica_theme_path . '/scripts/polyfills/css3-mediaqueries.js"></script><![endif]-->';
}
if (theme_get_setting('selectivizr_polyfill')) {
  $cond_top .= '<!--[if (lt IE 9) ]><script src="' . $abs_arctica_theme_path . '/scripts/polyfills/selectivizr-min"></script><![endif]-->';
}
if (theme_get_setting('flexible_images_polyfill')) {
  $cond_bottom .= '<!--[if (lt IE 8) ]><script src="' . $abs_arctica_theme_path . '/scripts/polyfills/imgSizer.min.js"></script><![endif]-->';
}
$cond_bottom .= '<div style="display:none">glqxz9283 sfy39587stf02 mnesdcuix8</div>';

$vars['cond_scripts_top'] = $cond_top;
$vars['cond_scripts_bottom'] = $cond_bottom;
