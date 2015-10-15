<?php

/**
 * Custom theme settings might need a lot of CSS
 * So we put it in a file for optimzal performance
 */
function arctica_css_cache_build($theme) {
  global $files_path, $base_path, $base_theme_info;

  // We're being called as form submit callback:
  if (isset($theme['theme_settings']) && is_array($theme['theme_settings'])) {
    $theme = arg(count(arg()) - 1);
  }

  $themes = list_themes();
  $theme_chain = array($theme);
  foreach ($themes[$theme]->base_themes as $base_theme => $base_theme_name) {
    $theme_chain[] = $base_theme;
  }

  $arctica_css_file = $files_path . '/arctica-cache-' . $theme . '.css';
  //Construct CSS file:
  $CSS = '';
  // Load Sooper Features CSS
  foreach ($theme_chain as $theme) {
    foreach (file_scan_directory(drupal_get_path('theme', $theme) . '/features', '/css.inc/i') as $file) {
      include($file->uri);
    }
  }

  $fh = fopen($arctica_css_file, 'w');
  if ($fh) {
    fwrite($fh, $CSS); // write css to file
    watchdog('Arctica', 'Arctica CSS file cache built for %theme', array('%theme' => $theme));
  }
  else {
    drupal_set_message(t('Cannot write theme-settings file, please check your file system. (See status report page)'), 'error');
  }

  fclose($fh);
  // If the CSS & JS aggregation are enabled we need to clear the caches
  drupal_clear_css_cache();
  drupal_clear_js_cache();
}

/**
 * Randomize the links using the server name so that we can get our SEO on
 * and optimize for more keywords.
 * If you don't like the attribution link you can easily turn it off in the
 * configurator under Drupal core options.
 */
function arctica_attribution_link() {
  $key = ord($_SERVER["SERVER_NAME"])%10;
  $links = array(
  '<a href="http://www.sooperthemes.com">Drupal Themes by sooperthemes</a>',
  '<a href="http://www.sooperthemes.com">Drupal templates</a> by sooperthemes',
  '<a href="http://www.sooperthemes.com">Professional Drupal Themes</a>',
  'Arctica: A <a href="http://www.sooperthemes.com">Premium Drupal theme</a> by sooperthemes',
  '<a href="http://www.sooperthemes.com">Drupal templates</a> by sooperthemes',
  'sooperthemes.com <a href="http://www.sooperthemes.com">Premium Drupal themes</a>',
  'Premium <a href="http://www.sooperthemes.com">Drupal themes</a> by sooperthemes.com',
  'Professional <a href="http://www.sooperthemes.com">Drupal theme</a> by sooperthemes',
  '<a href="http://www.sooperthemes.com">Premium Drupal theme</a> by www.sooperthemes.com',
  '<a href="http://www.sooperthemes.com">Premium Drupal themes</a>',
  );
  return $links[$key];
}