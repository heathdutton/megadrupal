<?php
$form['arctica_settings']['fontkit'] = array(
  '#title' => t('Fonts'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
   '#weight' => 4,
);

$font_list = tundra_font_list();
$font_count = count($font_list, COUNT_RECURSIVE);

$form['arctica_settings']['fontkit']['font_choose'] = array(
  '#title' => "Choose from {$font_count} fonts",
  '#type' => 'fieldset',
  '#suffix' => _tundra_font_previews(),
);

// Regular Fonts Settings
$form['arctica_settings']['fontkit']['font_choose']['body_font_face'] = array(
  '#type' => 'select',
  '#title' => t('Base font'),
  '#default_value' =>  theme_get_setting('body_font_face'),
  '#options' => $font_list,
  '#prefix'        => '<p>' . t('You can select web fonts, local fonts or google fonts. All fonts are functional in all commonly used browsers. Local fonts are included in the theme and loaded with @font-face CSS. Google fonts are hosted and delivered by Google using @font-face CSS via google\'s cdn.') . '</p>',
);

$form['arctica_settings']['fontkit']['font_choose']['body_font_face_selector'] = array(
  '#type' => 'textfield',
  '#title' => t('Enter a CSS Selector for base font'),
  '#default_value' => theme_get_setting('body_font_face_selector'),
  '#required' => TRUE,
);

$form['arctica_settings']['fontkit']['font_choose']['headings_font_face'] = array(
  '#type' => 'select',
  '#title' => t('Headings font'),
  '#default_value' =>  theme_get_setting('headings_font_face'),
  '#options' => $font_list,
);

$form['arctica_settings']['fontkit']['font_choose']['headings_font_face_selector'] = array(
  '#type' => 'textfield',
  '#title' => t('Enter a CSS Selector for headings font'),
  '#default_value' => theme_get_setting('headings_font_face_selector'),
  '#required' => TRUE,
);

$form['arctica_settings']['fontkit']['font_choose']['nav_font_face'] = array(
  '#type' => 'select',
  '#title' => t('Navigation font'),
  '#default_value' =>  theme_get_setting('nav_font_face'),
  '#options' => $font_list,
);

$form['arctica_settings']['fontkit']['font_choose']['nav_font_face_selector'] = array(
  '#type' => 'textfield',
  '#title' => t('Enter a CSS Selector for navigation font'),
  '#default_value' => theme_get_setting('nav_font_face_selector'),
  '#required' => TRUE,
);

$form['arctica_settings']['fontkit']['font_choose']['sitename_font_face'] = array(
  '#type' => 'select',
  '#title' => t('Site name font'),
  '#default_value' =>  theme_get_setting('sitename_font_face'),
  '#options' => $font_list,
);

$form['arctica_settings']['fontkit']['font_choose']['sitename_font_face_selector'] = array(
  '#type' => 'textfield',
  '#title' => t('Enter a CSS Selector for site name font'),
  '#default_value' => theme_get_setting('sitename_font_face_selector'),
  '#required' => TRUE,
);


/**
 * Helper function to provide a list of fonts for select list in theme settings.
 */
function tundra_font_list() {
  $fonts = array(
    'Sans-serif web-safe' => array(
      'helvetica' => 'Helvetica/Arial',
      'myriad' => 'Myriad/Calibri',
      'verdana' => 'Verdana',
      'lucida' => 'Lucida Sans',
      'geneva' => 'Geneva',
      'tahoma' => 'Tahoma',
      'trebuchet' => 'Trebuchet',
      'century' => 'Century Gothic',
    ),
    'Serif web-safe' => array(
      'garamond' => 'Garamond',
      'georgia' => 'Georgia',
      'palatino' => 'Palatino Linotype',
      'times' => 'Times New Roman',
    ),
  );
  $fonts['Local webfonts'] = tundra_local_webfonts();
  $fonts['Google webfonts'] = tundra_google_webfonts();

  return $fonts;
}

/**
 * Helper function to get list of google web fonts
 */
function tundra_google_webfonts() {
  $json_webfonts = file_get_contents(dirname(__FILE__) . '/google-webfonts.json');
  $webfonts = json_decode($json_webfonts);
  $fonts = array();
  foreach ($webfonts->items as $font_family) {
    $fam_name = $font_family->family;
    foreach ($font_family->variants as $font) {
      if ($font == 'regular') $font = '';
      $font_name = $fam_name . ' ' . $font;
      $font_name_safe = str_replace(' ', '+', $fam_name) . ':' . $font;
      $font_name_key = '0' . $font_name_safe;
      $fonts[$font_name_key] = $font_name;
    }
  }
  return $fonts;
}
/**
 * Helper function to get list of locally hosted fonts
 */
function tundra_local_webfonts($loadem = FALSE) {
  global $base_path, $base_url, $theme_chain;
  $fonts = array();
  foreach ($theme_chain as $theme) {
    foreach (file_scan_directory(drupal_get_path('theme', $theme) . '/fonts', '/.css/i') as $file) {
      $font_path = str_replace(drupal_get_path('theme', $theme), '', $file->uri);
      $css = file_get_contents($file->uri);
      if ($css) {
        preg_match_all('/\'(.*?).svg#(.*?)\'/i', $css, $matches);
      }
      else {
        drupal_set_message(t('Cannot read font files, please check permissions on the fonts folder in the theme.'), 'error');
      }
      foreach ($matches[2] as $font) {
        $key = '1' . $theme . '|' . $font_path . '|' . $font;
        $fonts[$key] = preg_replace('/(?<!\ ) {A-Z]/', ' $0', $font);
      }
      // @code for theme-settings.php load all the fonts
      if ($loadem) {
        $element = array(
          '#tag' => 'link',
          '#attributes' => array(
            'href' => $base_path . $file->uri,
            'rel' => 'stylesheet',
            'type' => 'text/css',
          ),
        );
        drupal_add_html_head($element, $file->uri);
      }
    }
  }

  asort($fonts, SORT_STRING);
  return $fonts;
}

/**
 * Helper function to display font previews in the settings form
 */
function _tundra_font_previews() {
  $output = '<div class="fontkit-previews">';
  $output .= '  <h3>Google fonts preview</h3>';
  $output .= '  <p>Check out <a href="http://www.google.com/webfonts">google.com/webfonts</a> for previews of google web fonts.</p>';
  $output .= '  <h3>Local fonts preview</h3>';
  $output .= '  <p>Sooper Fontkit will automatically detect and parse any Fontsquirrel-generated @font-face package in the tundra fonts folder. See <a href="http://www.fontsquirrel.com/fontface">fontsquirrel.com/fontface</a></p>';
  foreach (tundra_local_webfonts(TRUE) as $key => $font_name) {
    $font = explode('|', $key);
    $local_name = $font[2];
    $output .= '  <div class="font-preview font-local" style="font-family:' . $local_name . '">';
    $output .= '  <p class="font-name">' . $font_name . '</p><p class="font-test">The quick brown fox jumps over the lazy dog.</p>';
    $output .= '  <p class="font-test-small">The quick brown fox jumps over the lazy dog.</p>';
    $output .= '  <p class="font-test-accents">&#36; &euro; &pound; &yen; &agrave; &egrave; &igrave; &ograve; &ugrave; &Agrave; &Egrave; &Igrave; &Ograve; &Ugrave; &aacute; &eacute; &iacute; &oacute; &uacute; &yacute; &Aacute; &Eacute; &Iacute; &Oacute; &Uacute; &Yacute;</p>';
    $output .= '  </div>';
  }
  $output .= '</div>';
  return $output;
}
