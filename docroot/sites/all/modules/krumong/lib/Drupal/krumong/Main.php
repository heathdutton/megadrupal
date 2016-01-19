<?php

namespace Drupal\krumong;


class Main {

  protected $requestCache;
  protected $stdOut;
  protected $cssJs;

  /**
   * @param object $request_cache
   *   Service for drupal_set_message() and delayed drupal_add_js() / _css().
   */
  function __construct($request_cache, $stdOut, $cssJs) {
    $this->requestCache = $request_cache;
    $this->stdOut = $stdOut;
    $this->cssJs = $cssJs;
  }

  /**
   * Print data and js/css to stdOut.
   * This can be used on requests that don't have a regular header etc.
   *
   * @param mixed $data
   *   The data to print.
   * @param string $name
   *   Label for the data
   */
  function kPrint($data, $name = NULL) {
    $html = $this->dump($data, $name, FALSE);
    if (isset($html)) {
      $this->stdOut->html($html);
    }
  }

  /**
   * Dump a variable and make it show up in Drupal's message area.
   *
   * @param mixed $data
   *   any variable to dump.
   */
  function kMessage($data, $name = NULL, $type = 'status') {
    $html = $this->dump($data, $name, FALSE);
    $this->requestCache->message($html, $type);
  }

  /**
   * Make the data show up with console.log(), if available in the browser.
   * The script tag will be printed directly to stdOut.
   */
  function jPrint($data, $name = NULL) {
    $js = $this->jsPseudoTyped($data, $name);
    $html = <<<EOT
<script type="text/javascript"> // <!--
console && console.log && console.log($js);
// --></script>
EOT;
    $this->stdOut->html($html);
  }

  /**
   * Make the data show up with console.log(), if available in the browser.
   * The js will be added as inline js with drupal_add_js().
   * It will be postponed to the next request, if necessary.
   */
  function jMessage($data, $name = NULL) {
    $js = $this->jsPseudoTyped($data, $name);
    $js = "console && console.log && console.log($js);";
    $this->requestCache->jsInline($js);
  }

  /**
   * Dump a variable and return it as html.
   *
   * @param mixed $data
   *   The data to dump.
   * @param string $name
   *   Label for the data.
   * @param boolean $add_css_js
   *   If TRUE, this will trigger drupal_add_js() / _css().
   *
   * @return string
   *   tree-rendered html
   */
  function dump($data, $name = NULL, $add_css_js = TRUE) {
    if ($add_css_js) {
      $this->cssJs->addCssJsOnce();
    }
    if (is_string($data)) {
      $html = check_plain($data);
      $html = <<<EOT
<pre>$html</pre>
EOT;
    }
    else {
      $js = $this->jsPrefixed($data, $name);
      $randomstring = Util::randomstring(30);
      $html = <<<EOT
<script type="text/javascript"> // <!--
jQuery(function(){
  var data = $js;
  krumong.krumong('#_$randomstring', data.data, data.calledFrom);
});
// --></script>
<div id="_$randomstring"></div>
EOT;
    }
    if (isset($name)) {
      $name = check_plain($name);
      $html = <<<EOT
$name:
$html
EOT;
    }
    return $html;
  }

  /**
   * Generate a piece of javascript that evaluates as a nested object.
   * This is not json, it can only be processed as real js.
   *
   * The data is converted to js that looks nice with console.log().
   *
   * @param mixed $data
   *   The data to dump.
   * @param string $name
   *   Label for the data.
   *
   * @return string
   *   tree-rendered js.
   */
  function jsPseudoTyped(&$data, $name = NULL) {
    $theme = new TreeTheme_JsDataPseudoTyped();
    return $this->renderData($data, $theme);
  }

  /**
   * Generate a piece of javascript that evaluates as a nested object.
   * This is not json, it can only be processed as real js.
   *
   * The returned js has the format that is needed for client-side krumong print.
   *
   * @param mixed $data
   *   The data to dump.
   * @param string $name
   *   Label for the data.
   *
   * @return string
   *   tree-rendered js.
   */
  function jsPrefixed(&$data, $name = NULL) {
    $theme = new TreeTheme_JsDataPrefixed();
    return $this->renderData($data, $theme);
  }

  /**
   * Let the tree renderer render a nested data using a TreeTheme.
   *
   * @param mixed $data
   *   The data to dump.
   * @param TreeTheme $theme
   *   The theme to use.
   * @param string $name
   *   Label for the data.
   *
   * @return string
   *   tree-rendered js.
   */
  protected function renderData(&$data, $theme, $name = NULL) {
    $renderer = new TreeRenderer_BreadthFirst($theme);
    return $renderer->render($data, $this->calledFrom(), $name);
  }

  /**
   * Determine where dpm() and friends are called from.
   *
   * @return array
   *   Associative array with 'file' and 'line' information.
   */
  function calledFrom() {
    $call = $this->calledFromCall(0);
    return array(
      'file' => $call['file'],
      'line' => $call['line'],
    );
  }

  function calledFromCall($k = 1) {
    $trace = debug_backtrace();
    foreach ($trace as $i => $call) {
      if (!isset($call['file']) || !isset($call['line'])) {
        continue;
      }
      $filename = basename($call['file']);
      if ($filename === 'krumong.namespaced.inc' || $filename === 'devel.module') {
        continue;
      }
      if (isset($trace[$i + 1]['class']) && preg_match('#^Drupal\\\\krumong\\\\#', $trace[$i + 1]['class'])) {
        continue;
      }
      break;
    }

    return isset($trace[$i + $k]) ? $trace[$i + $k] : NULL;
  }
}
