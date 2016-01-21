<?php

/**
 * @file
 * theme-settings.php provides the custom theme settings
 *
 * Provides the checkboxes for the CSS overrides functionality
 * as well as the serif/sans-serif style option.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function gratis_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {

  //$form['theme_settings']['#group'] = 'gratis_settings';

  // Set the vertical tabs up.
  $form['gratis_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Gratis color settings tab area.
  $form['gratis_color_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis color settings'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
    '#description' => t("Set the theme color palette for gratis from the list below."),
  );

  $form['gratis_color_settings']['theme_color_palette'] = array(
    '#type' => 'select',
    '#title' => t('Choose a color palette'),
    '#default_value' => theme_get_setting('theme_color_palette'),
    '#options' => array(
      'turquoise' => t('Turquoise Blue'),
      'purple' => t('Cool Purple'),
      'orange' => t('Pumpkin Orange'),
      'green' => t('Olive Green'),
      'pomegranate' => t('Pomegranate Red'),
      'seafoam' => t('Seafoam Green'),
      'greengray' => t('Green Gray'),
      'pink' => t('Pink'),
      'mustard' => t('Mustard'),
      'surf-green' => t('Surf Green'),
      'maillot-jaune' => t('Maillot Jaune (Dark background)'),
      'caribe' => t('Caribe (Dark background)'),
      'chartreuse' => t('Chartreuse (Dark background)'),
      'mediterranean-red' => t('Mystic Blue (Dark background)'),
    ),
  );

  $form['gratis_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis css settings'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
  );

  $form['gratis_css']['gratis_localcss'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use local.css?'),
    '#default_value' => theme_get_setting('gratis_localcss'),
    '#description' => t("This setting allows you to use your own custom css file within the gratis
theme folder. Only check this box if you have renamed local.sample.css to local.css.
You must clear the Drupal cache after doing this."),
  );

  $form['gratis_css']['gratis_hacks_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable gratis hacks css?'),
    '#default_value' => theme_get_setting('gratis_hacks_settings'),
    '#description' => t('Check this if you want these hacks. <ul><li>no admin menu  /
    toolbar at narrow viewports. (this might be good while in development.) </li></ul>'),
  );

  $form['gratis_css']['gratis_header_layout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Gratis Header Layout'),
    '#default_value' => theme_get_setting('gratis_header_layout'),
    '#description' => t("Check this option to have the header logo left and the site slogan right. (default is both centered"),
  );

  $form['gratis_css']['custom_css_path_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom css path settings'),
    '#collapsible' => FALSE,
  );

  $form['gratis_css']['custom_css_path_settings']['custom_css_path']['gratis_custom_css_location'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only check this box if you want to specify a custom path below to your local css file.'),
    '#default_value' => theme_get_setting('gratis_custom_css_location'),
  );

  $form['gratis_css']['custom_css_path_settings']['custom_css_path']['gratis_custom_css_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Custom Stylesheet'),
    '#description' => t('Specify a custom path to the local.css file without the leading slash:
e.g.: sites/default/files/custom-css/local.css you must check the box above for this to work.'),
    '#default_value' => theme_get_setting('gratis_custom_css_path'),
  );

  $form['gratis_gridwidth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis page width and style'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
  );

  $form['gratis_gridwidth']['gratis_grid_container_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Optional grid width value. e.g 1020px, 100% etc...'),
    '#default_value' => theme_get_setting('gratis_grid_container_width'),
    '#description' => t('This setting allows you to set the width of the entire gird container.
Leave blank for the default max width of 1200px.  All inner grids are percentage based
so this should work with most any value you set within reason.'),
  );

  $form['gratis_gridwidth']['gratis_page_style'] = array(
    '#type' => 'select',
    '#title' => t('Choose a page style'),
    '#default_value' => theme_get_setting('gratis_page_style'),
    '#options' => array(
      'default' => t('Default'),
      'boxed' => t('Boxed'),
    ),
    '#description' => t("Deafult appears as a fuil width layout with a container within. Boxed
    appears more as a fixed width container, yet is still responsive. With boxed, you can
    utilize background tints and patterns for wider screen layouts."),
  );

  // Gratis bg patterns settings.
  $form['gratis_bg_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis boxed layout background settings'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
    '#description' => t("Choose a background pattern. These will only show if you choose 'Boxed Layout.'"),
  );

  $form['gratis_bg_settings']['theme_bg_pattern'] = array(
    '#type' => 'select',
    '#title' => t('Choose a background pattern'),
    '#default_value' => theme_get_setting('theme_bg_pattern'),
    '#description' => t("See the theme page for patterns key"),
    '#options' => array(
      'bg_pattern_01' => t('Background pattern 1'),
      'bg_pattern_02' => t('Background pattern 2'),
      'bg_pattern_03' => t('Background pattern 3'),
      'bg_pattern_04' => t('Background pattern 4'),
      'bg_pattern_05' => t('Background pattern 5'),
      'bg_pattern_06' => t('Background pattern 6'),
      'bg_pattern_07' => t('Background pattern 7'),
      'bg_pattern_08' => t('Background pattern 8'),
      'bg_pattern_09' => t('Background pattern 9'),
      'bg_pattern_10' => t('Background pattern 10'),
      'bg_pattern_11' => t('Background pattern 11'),
      'bg_pattern_12' => t('Background pattern 12'),
      'bg_pattern_13' => t('Background pattern 13'),
      'bg_pattern_14' => t('Background pattern 14'),
      'bg_pattern_15' => t('Background pattern 15'),
    )
  );

  $form['gratis_bg_settings']['theme_bg_tint'] = array(
    '#type' => 'select',
    '#title' => t('Background Tint'),
    '#default_value' => theme_get_setting('theme_bg_tint'),
    '#description' => t("Choose a tint hue for your background"),
    '#options' => array(
      'no_tint' => t('None'),
      'bg-tint-turquoise' => t('Tint Turquoise Blue'),
      'bg-tint-purple' => t('Tint Cool Purple'),
      'bg-tint-orange' => t('Tint Pumpkin Orange'),
      'bg-tint-green' => t('Tint Olive Green'),
      'bg-tint-pomegranate' => t('Tint Pomegranate Red'),
      'bg-tint-seafoam' => t('Tint Seafoam Green'),
      'bg-tint-greengray' => t('Tint Green Gray'),
      'bg-tint-pink' => t('Tint Pink'),
      'bg-tint-mustard' => t('Tint Mustard'),
      'bg-tint-surf-green' => t('Tint Surf Green'),
      'bg-tint-maillot-jaune' => t('Tint Maillot Jaune'),
      'bg-tint-caribe' => t('Tint Caribe'),
      'bg-tint-chartreuse' => t('Tint Chartreuse'),
      'bg-tint-mediterranean-red' => t('Tint Mystic Blue'),
    )
  );

  $form['gratis_breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis breadcrumbs'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
  );

  $form['gratis_breadcrumb']['breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show page breadcrumbs'),
    '#default_value' => theme_get_setting('breadcrumb'),
    '#description' => t("Check this option to show page breadcrumbs. Uncheck to hide."),
  );

  $form['gratis_touch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis touch device'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
  );

  $form['gratis_touch']['gratis_viewport'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Touch device pinch and zoom?'),
    '#default_value' => theme_get_setting('gratis_viewport'),
    '#description' => t('Check this box ONLY if you want to enable touch device users to be able to pinch and zoom.'),
  );

  // gratis additional settings.
  $form['gratis_js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis JS settings'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
  );

  $form['gratis_js']['gratis_livereload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Live Reload?'),
    '#default_value' => theme_get_setting('gratis_livereload'),
    '#description' => t('Check this box to use livereload. (Recommneded for local development only.)'),
  );

  $form['gratis_js']['gratis_minifiedjs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use minified js scripts file?'),
    '#default_value' => theme_get_setting('gratis_minifiedjs'),
    '#description' => t('Check this box and the site will use the generated minified custom JS file within the theme.'),
  );

  // Gratis typography.
  $form['gratis_typography'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gratis typography'),
    '#collapsible' => TRUE,
    '#group' => 'gratis_settings',
  );

  $form['gratis_typography']['gratis_setfonts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Would you like to use Gratis\' default typography? '),
    '#default_value' => theme_get_setting('gratis_setfonts'),
    '#description' => t('Check this box to use gratis\' built-in fonts, leave unchecked to use the @font-your-face module or other font providers.'),
  );

  $form['gratis_typography']['gratis_typography_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Font choices'),
    '#collapsible' => TRUE,
    '#description' => t('Choose your fonts.'),
    '#states' => array(
      'visible' => array(
        ':input[name="gratis_setfonts"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['gratis_typography']['gratis_typography_settings']['gratis_heading_typeface'] = array(
    '#type' => 'select',
    '#title' => t('Choose a heading typeface'),
    '#default_value' => theme_get_setting('gratis_heading_typeface'),
    '#options' => array(
      'opensans' => t('Open Sans (modern clean sans-serif)'),
      'garamond' => t('EB Garamond (tradtional serif)'),
      'imfell' => t('IM Fell (antique style)'),
    ),
  );

  $form['gratis_typography']['gratis_typography_settings']['gratis_body_typeface'] = array(
    '#type' => 'select',
    '#title' => t('Choose a body typeface'),
    '#default_value' => theme_get_setting('gratis_body_typeface'),
    '#options' => array(
      'opensans' => t('Open Sans (modern clean sans-serif)'),
      'garamond' => t('EB Garamond (tradtional serif)'),
    ),
  );

}
