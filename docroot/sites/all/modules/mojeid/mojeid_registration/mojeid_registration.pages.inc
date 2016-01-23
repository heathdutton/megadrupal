<?php


/**
 * Benefits for users page.
 *
 * This page is activated when motivation program is enabled.
 */
function mojeid_registration_benefits_page() {
  module_load_include('inc', 'mojeid');

  $title = _mojeid_registration_page_get('title');
  $text = _mojeid_registration_page_get('body');
  $format = _mojeid_registration_page_get('body_format');

  drupal_set_title($title);

  $build = array();
  $build['text'] = array(
    '#markup' => check_markup($text, $format),
  );

  if (_mojeid_promo_icon_display('mojeid_benefits_page')) {
    $size = variable_get('mojeid_banner_size', 'big');
    $type = variable_get('mojeid_banner_type', 'A1');
    $banner = mojeid_file_language_variant(drupal_get_path('module', 'mojeid') . "/img/powered_$size/$type.png");
    if ($banner) {
      $build['promo_icon'] = array(
        '#theme' =>  'image',
        '#path' => $banner,
        '#alt' => t('mojeID.cz'),
        '#title' => t('Supports mojeID authentication'),
        '#prefix' => '<p>',
        '#suffix' => '</p>',
      );
    }
  }

  if (user_is_anonymous()) {
    $build['mojeid_registration'] = drupal_get_form('mojeid_registration_form');
  }

  return $build;
}

/**
 * mojeID registration button.
 */
function mojeid_registration_form($form, &$form_values) {
  $module_path = drupal_get_path('module', 'mojeid');

  // Load additional CSS and JS.
  $form['#attached']['css'][] = $module_path . '/mojeid.css';

  // Get image buttons settings.
  $theme = variable_get('mojeid_theme', 'silver_155x24');
  list($color, $size) = explode('_', $theme);

  $form['mojeid_login'] = array(
    '#type' => 'image_button',
    '#src' => mojeid_file_language_variant("$module_path/img/buttons_$color/log_$size.png"),
    '#attributes' => array(
      'class' => array('image-button'),
      'title' => t('Log-in to mojeID'),
    ),
    '#access' => user_is_anonymous(),
    '#submit' => array('_mojeid_direct_login_submit'),
  );
  $form['mojeid_registration'] = array(
    '#type' => 'image_button',
    '#src' => mojeid_file_language_variant("$module_path/img/buttons_$color/reg_$size.png"),
    '#attributes' => array(
      'class' => array('image-button'),
      'title' => t('Register to mojeID'),
    ),
    '#access' => user_is_anonymous(),
    '#submit' => array('mojeid_registration_button_submit'),
  );
  return $form;
}

/**
 * mojeID registration button submit handler.
 */
function mojeid_registration_form_submit($form, &$form_values) {
  mojeid_registration_button_submit($form, $form_values);
}

/**
 * Page callback - client endpoint.
 *
 * Invokes hook_mojeid_registration() after valid reponse come in. Note that
 * endpoint returns HTTP code 200 when the response is valid and code 500
 * otherwise.
 */
function mojeid_registration_endpoint() {
  $status = FALSE;

  // Check that request came via HTTPS.
  $https = ((!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off') || $_SERVER['SERVER_PORT'] == 443);

  if (variable_get('mojeid_logging', FALSE)) {
    // Log all attempts.
    watchdog('mojeid',
      'Registration endpoint: HTTPS: @https, GET: @get, POST: @post',
      array('@https' => (bool) $https, '@get' => print_r($_GET, TRUE), '@post' => print_r($_POST, TRUE)),
      WATCHDOG_DEBUG);
  }

  // Allow only HTTPS, valid nonce and valid certificate.
  // @todo validate SSL PEM certificate? Currently webserver must check it.
  $response = $_POST;
  if ($https && isset($response['registration_nonce'], $response['claimed_id'])) {
    if (mojeid_registration_verify_nonce($response['registration_nonce'])) {
      $status = TRUE;
      // Invoke hook_mojeid_registration().
      module_invoke_all('mojeid_registration_response', $response);
    }
  }

  // Notify remote server that everything is OK or that something wrong happened.
  if ($status === TRUE) {
    header('HTTP/1.1 200 OK');
  }
  else {
    header('HTTP/1.1 500 Internal Server Error');
  }
  exit();
}
