<?php

/**
 * @file
 * Devicons integration to display demo content.
 */

/**
 * CSS parser to get class names of css file.
 */
function _devicons_parser($path) {

  $css = drupal_load_stylesheet($path, TRUE);

  preg_match_all( '/(?ims)([a-z0-9\s\.\:#_\-@,]+)\{([^\}]*)\}/', $css, $arr);
  $result = array();
  foreach ($arr[0] as $i => $x) {
    $selector = trim($arr[1][$i]);
    $rules = explode(';', trim($arr[2][$i]));
    $rules_arr = array();
    foreach ($rules as $strRule) {
      if (!empty($strRule)) {
        $rule = explode(":", $strRule);
        $rules_arr[trim($rule[0])] = trim($rule[1]);
      }
    }

    $selectors = explode(',', trim($selector));
    foreach ($selectors as $strSel) {
      if(array_key_exists('content', $rules_arr)) {
        $strSel = substr($strSel, 1, -7);
        $result[$strSel] = $strSel;
      }
    }
  }
  return $result;
}


/**
 * Prepares markup array Devicons demo page.
 */
function _devicons_create_markup(array $classnames) {

  $deviconmarkup = array();

  foreach ($classnames as $item) {
      $devicon_icon = "<span class=\"devicons " . $item . "\" style=\"font-size:45px;\">";
      $devicon_class = "devicons ".$item;
      $devicon_markup = "&lt;span class=\"devicons " . $item . "\"&gt;&lt;/span&gt;";
      $deviconmarkup[] = array($devicon_icon, $devicon_class, $devicon_markup);
  }
  return $deviconmarkup;
}


/**
 * Constructs a simple Devicons demo page.
 *
 * The simple page callback, mapped to the path '/admin/config/devicons/demo'.
 *
 * Page callbacks return a renderable array with the content area of the page.
 */
function devicons_demo_page() {

  global $base_url;

  $deviconvar = variable_get('devicons_global');

  if ($deviconvar == 'TRUE' && ($library = libraries_detect('devicons')) && !empty($library['installed'])) {
    $header = array(t('Icon'), t("CSS Class name"),t('Example Markup'));
    $rows = _devicons_parser(libraries_get_path('devicons') . '/css/devicons.css');
    $rows = _devicons_create_markup($rows);

    $build['intro'] = array(
      '#markup' => t("<p>A detailed documentation of the Devicons you can find here: !projectpage</p>", array(
        '!projectpage' => l(t('Visit projectpage'), 'http://vorillaz.github.io/devicons/'),
        )
      ),
    );

    $build['devicons_demo'] = array(
      '#theme' => 'table__devicons',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  else {
    $build['intro'] = array(
      '#markup' => t("<p>Please check Devicon !statuspage and/or enable global integration first: !settingspage</p>", array(
          '!settingspage' => l(t('Visit Devicon Settings'), $base_url . '/admin/config/devicons/settings'),
          '!statuspage' => l(t('installation status'), $base_url . '/admin/reports/status'),
        )
      ),
    );
  }
  return $build;
}
