<?php
/**
 * @file
 * Implements hook_preprocess_page().
 */

global $theme_key;
$theme_name = $theme_key;

// Hide title on frontpage.
if ($vars['is_front']) {
  $vars['title_attributes_array']['class'][] = 'element-invisible';
}

/*
 * Set up logo's and wordmark for this website.
 */

// Provide a variable with the type of branding (rijkshuisstijl|neutral|custom).
$vars['branding_type'] = theme_get_setting('branding_type', $theme_name);

/*
 * Add the sender of the website to place on the right of the logotype of the
 * Rijksoverheid logo. i.e. National Archives, secondary sender: Ministry of
 * Education. May contain linebreaks for long names, should get converted to
 * <br /> not sure if we for a11y should include the wordmark in a p, or
 * add headings etc.
 */
$sender = theme_get_setting('sender');
$vars['sender'] = '<span class="sender">' . nl2br(check_plain($sender)) . '</span>';
$vars['secondary_sender'] = '<span class="secondary-sender">' . nl2br(check_plain(theme_get_setting('secondary_sender'))) . '</span>';
$vars['wordmark'] = ($sender) ? '<span class="wordmark">' . $vars['sender'] . ' ' . $vars['secondary_sender'] . '</span>' : "";

// Create the alt text for the logo.
$logo_alt = check_plain(variable_get('site_name', t('Site logo')));

// Create the logotype as svg.
$rijkslogo_svg_path = check_url(theme_get_setting('logo_svg_url'));
// Try to 'cache' the externally hosted file locally.
_rijkshuisstijl_load_file_locally($rijkslogo_svg_path);
$rijkslogo_vars = array(
  'path' => $rijkslogo_svg_path,
  'alt' => $logo_alt,
  'attributes' => array('class' => 'logotype'),
);
$vars['rijkslogo_img_svg'] = theme('image', $rijkslogo_vars);

// Create the logotype as png.
$rijkslogo_png_path = check_url(theme_get_setting('logo_png_url'));
// Try to 'cache' the externally hosted file locally.
_rijkshuisstijl_load_file_locally($rijkslogo_png_path);
$rijkslogopng_vars = array(
  'path' => $rijkslogo_png_path,
  'alt' => $logo_alt,
  'attributes' => array('class' => 'rijkslogo'),
);
$vars['rijkslogo_img_png'] = theme('image', $rijkslogopng_vars);

// Include smart SVG fallback with PNG for browsers that don't support SVG
// (IE8 and Android 2).
$vars['rijkslogo_img_with_fallback'] = '<!--[if lte IE 8]>' . $vars['rijkslogo_img_png'] . '<![endif]-->' .
  '<!--[if gt IE 8]>' . $vars['rijkslogo_img_svg'] . '<![endif]-->' .
  '<!--[if !IE]> -->' . $vars['rijkslogo_img_svg'] . '<!-- <![endif]-->';

/*
 * Fallback mechanism described here: http://stackoverflow.com/questions/8946134/conditional-code-for-displaying-png-in-place-of-svg-for-older-browsers
 * Note there are 10+ fallback tricks for svg, none of them is perfect.
 * Above one should work for every browser, even without javascript,
 * and works for IE8 and Android 2. Not sure if it works for IE10
 * Drawback is performance as an extra file request is done
 * (PNG is loaded also by modern browsers).
 */

// Combining above to a styled and linked Rijkslogo.
$vars['rijkslogo'] = l($vars['rijkslogo_img_with_fallback'] . $vars['wordmark'], '<front>', array(
  'attributes' => array('title' => t('Home page')),
  'html' => TRUE,
));

// Generate projectlogo (wordmark, rendered as text with markup) used in
// the neutrale stijl.
$projecttext = theme_get_setting('project_name');
$vars['projectlogo_html'] = strtr(nl2br(check_plain($projecttext)), array('{' => '<span>', '}' => '</span>'));
$vars['projectlogo'] = $projecttext ? l($vars['projectlogo_html'], '<front>', array('attributes' => array('title' => t('Home page')), 'html' => TRUE)) : "";

/*
 * @todo: Generate a custom logo from a file upload, for special edge cases.
 * if ($branding_type == 'custom'){
 *    $logo_path = check_url($vars['logo']);
 * }
 * $vars['site_logo'] = drupal_static('rijkshuisstijl_preprocess_page_site_logo');
 */
