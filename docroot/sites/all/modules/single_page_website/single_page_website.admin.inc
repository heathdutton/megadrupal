<?php

/**
 * @file
 * Admin Settings form for the Single Page Website module.
 */
function single_page_website_settings_form($form, &$form_state) {
  $themes = single_page_website_available_theme_list();
  if (!count($themes)) {
    $themes = single_page_website_supported_theme_list();
    drupal_set_message(t("You should install and enable at least one theme that supported Single Page Website module. See http://drupal.org/sandbox/vasilyyaremchuk/1131866 and README.txt"), "error");
    return array();
  }
  $form['theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#required' => TRUE,
    '#default_value' => variable_get('single_page_website_sub_theme', 'bartik') . " " . variable_get('single_page_website_theme', 'bartik'), // added default value.
    '#options' => $themes,
    '#description' => "Define theme for your Single Page. There are only supported themes in the list. <strong>Default theme will be switched to the selected one!</strong>",
  );
  $path = libraries_get_path('jquery.easing') . '/jquery.easing.js';
  if (file_exists($path)) {
    $form['easing'] = array(
      '#type' => 'select',
      '#title' => t('Easing'),
      '#required' => FALSE,
      '#default_value' => variable_get('single_page_website_easing', 'jswing'), // added default value.
      '#options' => array("jswing" => "jswing", "def" => "def",
        "easeInQuad" => "easeInQuad",
        "easeOutQuad" => "easeOutQuad",
        "easeInOutQuad" => "easeInOutQuad",
        "easeInCubic" => "easeInCubic",
        "easeOutCubic" => "easeOutCubic",
        "easeInOutCubic" => "easeInOutCubic",
        "easeInQuart" => "easeInQuart",
        "easeOutQuart" => "easeOutQuart",
        "easeInOutQuart" => "easeInOutQuart",
        "easeInQuint" => "easeInQuint",
        "easeOutQuint" => "easeOutQuint",
        "easeInOutQuint" => "easeInOutQuint",
        "easeInSine" => "easeInSine",
        "easeOutSine" => "easeOutSine",
        "easeInOutSine" => "easeInOutSine",
        "easeInExpo" => "easeInExpo",
        "easeOutExpo" => "easeOutExpo",
        "easeInOutExpo" => "easeInOutExpo",
        "easeInCirc" => "easeInCirc",
        "easeOutCirc" => "easeOutCirc",
        "easeInOutCirc" => "easeInOutCirc",
        "easeInOutBounce" => "easeInOutBounce",
        "easeInElastic" => "easeInElastic",
        "easeOutElastic" => "easeOutElastic",
        "easeInOutElastic" => "easeInOutElastic",
        "easeInBack" => "easeInBack",
        "easeOutBack" => "easeOutBack",
        "easeInOutBack" => "easeInOutBack",
        "easeInBounce" => "easeInBounce",
        "easeOutBounce" => "easeOutBounce"),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

function single_page_website_settings_form_submit($form, &$form_state) {
  $theme = explode(" ", $form_state['values']['theme']);
  variable_set('theme_default', $theme[1]);
  variable_set('single_page_website_theme', $theme[1]);
  variable_set('single_page_website_sub_theme', $theme[0]);
  $settings = parse_ini_file(drupal_get_path("module", "single_page_website") . '/js/' . variable_get('single_page_website_sub_theme', 'bartik') . '.ini');
  variable_set('single_page_website_header', $settings['header']);
  variable_set('single_page_website_footer', $settings['footer']);
  variable_set('single_page_website_menu', $settings['menu']);
  variable_set('single_page_website_content', $settings['content']);
  if (isset($form_state['values']['easing'])) {
    variable_set('single_page_website_easing', $form_state['values']['easing']);
  }
  drupal_set_message(t('The Single Page Module Settings has been saved.'));
}

function single_page_website_available_theme_list() {
  $themes = list_themes();
  $supported_themes = single_page_website_supported_theme_list();
  $available_themes = array();
  foreach ($themes as $theme) {
    foreach ($supported_themes as $supported_theme)
      if (($supported_theme == $theme->name && $theme->status) || (isset($theme->base_theme) && $supported_theme == $theme->base_theme && $theme->status))
        if (isset($theme->base_theme)) {
          $available_themes[$theme->base_theme . ' ' . $theme->name] = $theme->name;
        }
        else {
          $available_themes[$theme->name . ' ' . $theme->name] = $theme->name;
        }
  }
  return $available_themes;
}

function single_page_website_supported_theme_list() {
  $supported_themes = array();
  if ($handle = opendir(drupal_get_path("module", "single_page_website") . '/js')) {
    while (false !== ($file = readdir($handle))) {
      if ($file != "." && $file != "..") {
        if (stristr($file, ".ini"))
          $supported_themes[] = drupal_substr($file, 0, -4);
      }
    }
    closedir($handle);
  }
  return $supported_themes;
}