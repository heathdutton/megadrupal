<?php

function theme_analytics_js(array $variables) {
  if (variable_get('analytics_privacy_dnt', TRUE)) {
    $output = 'if (!Drupal.settings.dnt) {' . "\n";
    //$output .= "alert('TRACKING!');";
    $output .= trim($variables['js']) . "\n}";
    //$output .= "else { alert('NOT TRACKING!'); }";
    //$output .= "console.log(Drupal.settings.dnt);";
    return $output;
  }
  else {
    return $variables['js'];
  }
}
