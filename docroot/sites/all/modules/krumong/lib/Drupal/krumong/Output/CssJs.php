<?php

namespace Drupal\krumong\Output;


class CssJs {

  protected $cssJsPrinted = FALSE;
  protected $cssJsAdded = FALSE;

  /**
   * Print CSS an JS to stdOut.
   */
  function printCssJsOnce() {

    if ($this->cssJsPrinted) {
      return;
    }
    $this->cssJsPrinted = TRUE;
    $this->cssJsAdded = TRUE;

    // TODO: How can we know this is the correct jQuery to include?
    $jq = check_plain(file_create_url('misc/jquery.js'));

    $path = drupal_get_path('module', 'krumong');
    $kjs = check_plain(file_create_url($path . '/js/krumong.js'));
    $kcss = check_plain(file_create_url($path . '/css/krumong.css'));

    print <<<EOT
<script type="text/javascript" src="$jq"></script>
<script type="text/javascript" src="$kjs"></script>
<link type="text/css" rel="stylesheet" media="all" href="$kcss"></style>
EOT;
  }

  /**
   * Add CSS an JS for the current request, the usual Drupal way.
   */
  function addCssJsOnce() {

    if ($this->cssJsAdded) {
      return;
    }
    $this->cssJsAdded = TRUE;

    $path = drupal_get_path('module', 'krumong');
    drupal_add_js($path . '/js/krumong.js');
    drupal_add_css($path . '/css/krumong.css');
  }
}
