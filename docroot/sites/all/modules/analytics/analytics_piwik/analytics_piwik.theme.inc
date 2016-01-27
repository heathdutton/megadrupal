<?php

function theme_analytics_piwik_js(array &$variables) {
  $output = 'var _paq = _paq || [];' . "\n";

  $url = dirname(analytics_url($variables['url'] . 'piwik.js'));
  $output .= '(function(){ var u="' . $url . '/";' . "\n";

  foreach ($variables['actions'] as $action) {
    if (!is_array($action)) {
      $output .= $action . "\n";
    }
    else {
      $line = '_paq.push([';
      foreach ($action as $parameter) {
        $line .= drupal_json_encode($parameter) . ',';
      }
      $line = rtrim($line, ',');
      $line .= ']);';
      $output .= $line . "\n";
    }
  }

  $output .= "var d=document, g=d.createElement('script'), s=d.getElementsByTagName('script')[0]; g.type='text/javascript'; g.defer=true; g.async=true; g.src=u+'piwik.js';
    s.parentNode.insertBefore(g,s); })();" . "\n";

  return theme('analytics_js', array('js' => $output));
}
