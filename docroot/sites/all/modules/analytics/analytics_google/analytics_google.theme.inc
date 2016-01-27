<?php

function theme_analytics_google_universal_js(array &$variables) {
  $output = '';

  static $added = FALSE;
  if (!$added) {
    $url = analytics_url('https://www.google-analytics.com/analytics.js');
    // Ensure that the original script loading only gets added once.
    $output .= "(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
})(window,document,'script','" . $url . "','ga');\n";
    $added = TRUE;
  }

  foreach ($variables['actions'] as $action) {
    if (!is_array($action)) {
      $output .= $action . "\n";
    }
    else {
      $line = 'ga(';
      foreach ($action as $parameter) {
        $line .= drupal_json_encode($parameter) . ',';
      }
      $line = rtrim($line, ',');
      $line .= ');';
      $output .= $line . "\n";
    }
  }

  return theme('analytics_js', array('js' => $output));
}
