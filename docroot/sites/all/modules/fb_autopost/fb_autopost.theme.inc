<?php

/**
 * @file
 * Theme implementation file for Facebook Autopost
 */

/**
 * Theme implementation for the fb_autopost_facebook_profile render element.
 */
function theme_fb_autopost_facebook_profile($variables) {
  $output = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('fb-profile')),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'fb_autopost') . '/fb_autopost.css',
      ),
      'js' => array(
        drupal_get_path('module', 'fb_autopost') . '/fb_autopost.js',
      ),
    ),
    'image' => array(
      '#theme' => 'image',
      '#path' => $variables['fbprofile']['#fbprofile_picture']['data']['url'],
      '#title' => t('Facbeook picture'),
      '#alt' => t('Facbeook picture'),
      '#width' => 50,
      '#height' => 50,
    ),
    'data' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('fb-data')),
      'link' => array(
        '#type' => 'link',
        '#title' => $variables['fbprofile']['#fbprofile_name'],
        '#href' => $variables['fbprofile']['#fbprofile_link'],
      ),
      'id' => array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => t('Profile ID: @profile', array('@profile' => $variables['fbprofile']['#fbprofile_id'])),
      ),
      'logout' => array(
        '#type' => 'link',
        '#title' => t('Logout'),
        '#href' => $variables['fbprofile']['#fbprofile_logout_url'],
        '#id' => 'fb-logout-link',
      ),
    ),
  );
  return drupal_render($output);
}

/**
 * Default preprocess implementation.
 */
function template_preprocess_fb_autopost_facebook_profile(&$variables) {
  try {
    $fb = facebook_autopost();
    $fbuser_profile = $fb->api('/' . $variables['fbprofile']['#fbprofile_id'] . '?fields=picture,id,name,link');
    $variables['fbprofile']['#fbprofile_name'] = empty($fbuser_profile['name']) ? $variables['fbprofile']['#fbprofile_id'] : $fbuser_profile['name'];
    $variables['fbprofile']['#fbprofile_link'] = empty($fbuser_profile['link']) ? 'http://facebook.com/' . $variables['fbprofile']['#fbprofile_id'] : $fbuser_profile['link'];
    $variables['fbprofile']['#fbprofile_picture'] = $fbuser_profile['picture'];
    $variables['fbprofile']['#fbprofile_logout_url'] = $fb->getLogoutUrl(array(
      'next' => url('admin/config/services/fbautopost/logout', array('absolute' => TRUE)),
    ));
  }
  catch (FacebookApiException $e) {
    // Do nothing with the exception.
    watchdog('fb_autopost', $e->getMessage(), array(), WATCHDOG_ERROR);
  }
}

/**
 * Theme implementation for the fb_autopost_facebook_profile render element.
 */
function theme_fb_autopost_facebook_page($variables) {
  drupal_add_css(drupal_get_path('module', 'fb_autopost') . '/fb_autopost.css');
  return theme('image', array(
    'path' => "https://graph.facebook.com/" . $variables['id'] . "/picture?type=square",
    'title' => t('Facebook picture'),
    'alt' => t('Facebook picture'),
    'width' => 30, 'height' => 30)
  ) . ' ' . $variables['name'] . ' - ' . $variables['category'] . ' (Id: ' . $variables['id'] . ')';
}
