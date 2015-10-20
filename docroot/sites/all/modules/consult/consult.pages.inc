<?php
/**
 * @file
 * Page display callback for Consult module.
 */

/**
 * Page callback for consult interviews.
 *
 * @param ConsultInterview $interview
 *   The interview to render.
 *
 * @return array
 *   The render array.
 */
function consult_interview_page(ConsultInterview $interview) {
  $render = entity_view(CONSULT_INTERVIEW_ENTITY_NAME, array($interview));
  $output = render($render);

  return $output;
}

/**
 * Javascript settings injection callback for consult interviews.
 *
 * @param ConsultInterview $interview
 *   The interview to get the js settings for.
 */
function consult_interview_js_settings(ConsultInterview $interview) {
  $encoded_settings = &drupal_static(__FUNCTION__);

  if (!isset($encoded_settings)) {
    $cache_key = 'js_' . $interview->identifier() . '_' . (user_is_logged_in() ? '_auth' : '_anon');
    $cache = cache_get($cache_key);
    if ($cache) {
      $encoded_settings = $cache->data;
    }
    else {
      $interview_settings = $interview->getJsSettings();
      $rendered_interview_settings = drupal_json_encode($interview_settings[$interview->identifier()]);
      $encoded_settings = 'Drupal.settings.consult.' . $interview->identifier() . '=' . $rendered_interview_settings;
      cache_set($cache_key, $encoded_settings, 'cache');
    }
  }

  drupal_add_http_header('Cache-Control', 'public');
  drupal_add_http_header('Content-Type', 'application/javascript');
  print $encoded_settings;
  drupal_exit();
}
