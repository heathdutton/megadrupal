<?php
/**
 * @file
 * A custom Panels renderer.
 */

$plugin = array(
  'renderer' => 'panels_perf_renderer_timer',
);

class panels_perf_renderer_timer extends panels_renderer_standard {
  function render_pane_content(&$pane) {
    // Do you want to bypass caching to learn the truth about your slow queries?
    if (variable_get('panels_why_so_slow_skip_cache', FALSE)) {
      $this->display->skip_cache = TRUE;
    }

    $start = microtime(true);
    $content = parent::render_pane_content($pane);
    // Bail if nothing rendered
    if (!$content) {
      return $content;
    }

    // Include any theme functions' timing too.
    $rendered = render($content->content);
    $duration = microtime(true) - $start;
    $render_time = intval(($duration) * 100000) / 100;

    // These times are totally arbitrary.
    if ($render_time > (int) variable_get('panels_why_so_slow_really_bad', 100)) {
      $class = "bad";
    }
    elseif ($render_time > (int) variable_get('panels_why_so_slow_not_good', 15)) {
      $class = "okay";
    }
    else {
      $class = "good";
    }

    if (!empty($pane->cache['method']) && empty($this->display->skip_cache)) {
      $message = t("Rendered in %time ms (may cache)", array('%time'=> $render_time));
    }
    else {
      $message = t("Rendered in %time ms", array('%time'=> $render_time));
    }

    drupal_add_css(drupal_get_path('module', 'panels_why_so_slow') . '/panels_why_so_slow.css');

    $content->content = <<<HTML
<div class='panels-why-so-slow panels-why-so-slow-$class'>
  <span class='panels-why-so-slow-message'>$message</span>
  $rendered
</div>
HTML;

    return $content;
  }
}
