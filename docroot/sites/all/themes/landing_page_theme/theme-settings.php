<?php
require_once('landing_page_theme.inc');
/**
* Implementation of hook_form_system_theme_settings_alter
*/
function landing_page_theme_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  landing_page_theme_exist_libraries();

  $form['landing_page_theme'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Landing Page Theme Settings') . '</small></h2>',
    '#weight' => -10,
  );

  // Components.

  $form['background'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Background settings'),
    '#description'   => t('If toggled on, the following background will be displayed.'),
    '#group' => 'landing_page_theme',
  );
  $form['background']['default_background'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default background'),
    '#default_value' => theme_get_setting('default_background'),
    '#description' => t('Check here if you want the theme to use the background supplied with it.')
  );
  $form['background']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      // Hide the background settings when using the default background.
      'invisible' => array(
        'input[name="default_background"]' => array('checked' => TRUE),
      ),
    ),
  );
  // $form['background']['settings']['background_path'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Path to custom background'),
  //   '#description' => t('The path to the file you would like to use as your background file instead of the default background.'),
  //   '#default_value' => theme_get_setting('background_path'),
  // );
  $form['background']['settings']['background_upload'] = array(
    '#type' => 'managed_file',
    '#upload_location' => 'public://images/background/',
    '#upload_validators' => array('file_validate_extensions' => array('gif png apng jpg jpeg svg')),
    '#title' => t('Upload background image'),
    '#description' => t("If you don't have direct file access to the server, use this field to upload your background."),
    '#default_value' => theme_get_setting('background_upload'),
  );
  
  // Region settings.
  $form['regions'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Region settings'),
    '#description'   => t('Settings for specified regions.'),
    '#group' => 'landing_page_theme',
  );

  foreach (system_region_list('landing_page_theme') as $name => $title) {
    if ($name != 'dashboard_main' && $name != 'dashboard_sidebar' && $name != 'dashboard_inactive') {
      drupal_add_js(array('theme_regions' => array($name)), 'setting');

      $form['regions']['settings_' . $name] = array(
        '#type' => 'fieldset',
        '#title' => $title,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['regions']['settings_' . $name]['region_background_opacity-' . $name] = array(
        '#title' => t('Background opacity.'),
        '#type' => 'textfield',
        '#description' => t('Number between 0 and 1.'),
        '#default_value' => theme_get_setting('region_background_opacity-' . $name),
      );
      $form['regions']['settings_' . $name]['region_anchor-' . $name] = array(
        '#title' => t('Region anchor.'),
        '#type' => 'textfield',
        '#default_value' => theme_get_setting('region_anchor-' . $name),
      );
    } 
  }

  $form['padding_regions'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Padding regions'),
    '#description'   => t('Edit padding top and bottom for regions.'),
    '#group' => 'landing_page_theme',
  );
  $form['padding_regions']['region_padding_top'] = array(
      '#title' => t('Padding top'),
      '#type' => 'textfield',
      '#description' => t('Padding top'),
      '#default_value' => theme_get_setting('region_padding_top'),
  );
  $form['padding_regions']['region_padding_bottom'] = array(
      '#title' => t('Padding bottom'),
      '#type' => 'textfield',
      '#description' => t('Padding bottom'),
      '#default_value' => theme_get_setting('region_padding_bottom'),
  );


  $form['#submit'][] = 'landing_page_theme_settings_form_submit';

  // Get all themes.
  $themes = list_themes();
  // Get the current theme
  $active_theme = $GLOBALS['theme_key'];
  $form_state['build_info']['files'][] = str_replace("/$active_theme.info", '', $themes[$active_theme]->filename) . '/theme-settings.php';
}

function landing_page_theme_settings_form_submit(&$form, $form_state) {
  if (isset($form_state['values']['background_upload'])) {
    $image_fid = $form_state['values']['background_upload'];
    $image = file_load($image_fid);
    if (is_object($image)) {
      if ($image->status == 0) {
        $image->status = FILE_STATUS_PERMANENT;
        file_save($image);
        file_usage_add($image, 'landing_page_theme', 'theme', 1);
      }
    }
  }
 /*CREATE CSS FILE*/ 
  $output = "";
  
  if (!empty($form_state['input']['region_padding_top'])) {
    $output .= "section, header, footer{padding-top : " . $form_state['input']['region_padding_top'] . "px}";
  }
  if (!empty($form_state['input']['region_padding_bottom'])) {
    $output .= "section, header, footer{padding-bottom : " . $form_state['input']['region_padding_bottom'] . "px}";
  }

  foreach (system_region_list('landing_page_theme') as $name => $title) { 
    
    if ($name != 'dashboard_main' && $name != 'dashboard_sidebar' && $name != 'dashboard_inactive') {
      if ($form_state['input']['region_background_opacity-' . $name] < 0 || $form_state['input']['region_background_opacity-' . $name] > 1 || empty($form_state['input']['region_background_opacity-' . $name]) || is_string($form_state['input']['region_background_opacity-' . $name])) 
        $region_opacity = 1;
      else
        $region_opacity = $form_state['input']['region_background_opacity-' . $name];

      $color = hextorgb($form_state['input']['palette'][$name . 'bg']);
      $colortxt = $form_state['input']['palette'][$name . 'txt'];
      $colorlinks = $form_state['input']['palette'][$name . 'links'];
      $colorhover = $form_state['input']['palette'][$name . 'hover'];
      $output .= ".region-" . $name . "-bg{background-color:rgba(" . $color . "," . $region_opacity . "); color:" . $colortxt . ";}.region-" . $name . "-bg a{color:" . $colorlinks . ";}.region-" . $name . "-bg a:hover{color:" . $colorhover . ";}";
    }
  }


  $theme_path = drupal_get_path('theme', 'landing_page_theme');

  $my_file = $theme_path . '/css/custom_style/custom_style.css';
  $handle = fopen($my_file, 'w') or die('Cannot open file:  ' . $my_file);
  fwrite($handle, $output);
  fclose($handle);

  
}

function hextorgb($color) {
    if ($color[0] == '#') {
        $color = substr($color, 1);
    }
    if (strlen($color) == 6) {
        list($red, $green, $blue) = array(
            $color[0] . $color[1],
            $color[2] . $color[3],
            $color[4] . $color[5]
        );
    } 
    elseif (strlen($color) == 3) {
        list($red, $green, $blue) = array(
            $color[0] . $color[0],
            $color[1] . $color[1],
            $color[2] . $color[2]
        );
    }
    else{
        return FALSE; 
    }
 
    $red = hexdec($red); 
    $green = hexdec($green);
    $blue = hexdec($blue);

  return $red . ',' . $green . ',' . $blue;
}