<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function social_network_user_detection_rules_event_info() {
  return array(
    'social_network_user_detection_rules_condition_fb' => array(
      'group' => t('Social network user detection'),
      'label' => t('Social network user detection - FB'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_twitter' => array(
      'group' => t('Social network user detection'),
      'label' => t('Social network user detection - Twitter'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_gmail' => array(
      'group' => t('Social network user detection'),
      'label' => t('Social network user detection - Gmail'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_gplus' => array(
      'group' => t('Social network user detection'),
      'label' => t('Social network user detection - Google Plus'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_hyves' => array(
      'group' => t('Social network user detection'),
      'label' => t('Social network user detection - Hyves'),
      'module' => 'social_network_user_detection',
    ),
  );
}

/**
 * Controller function to call the corresponding function.
 *
 * String @param $network the network.
 */
function _social_network_user_detection_rules_controller($network) {
  switch ($network) {
    case 'Facebook':
      _social_network_user_detection_rules_detect_fb();
      break;

    case 'Twitter':
      _social_network_user_detection_rules_detect_twitter();
      break;

    case 'GooglePlus':
      _social_network_user_detection_rules_detect_gplus();
      break;

    case 'Google':
      _social_network_user_detection_rules_detect_gmail();
      break;

    case 'Hyves':
      _social_network_user_detection_rules_detect_hyves();
      break;

    default:
      // Nothing.
  }
}

/**
 * Function to detect hyves.
 */
function _social_network_user_detection_rules_detect_hyves($rules = TRUE) {
  if (isset($_SESSION['Hyves']) && $_SESSION['Hyves'] == TRUE) {
    ($rules) ? rules_invoke_event('social_network_user_detection_rules_condition_hyves') : '';
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function to detect Google Plus.
 */
function _social_network_user_detection_rules_detect_gplus($rules = TRUE) {
  if (isset($_SESSION['GooglePlus']) && $_SESSION['GooglePlus'] == TRUE) {
    ($rules) ? rules_invoke_event('social_network_user_detection_rules_condition_gplus') : '';
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function to detect Gmail.
 */
function _social_network_user_detection_rules_detect_gmail($rules = TRUE) {
  if (isset($_SESSION['Google']) && $_SESSION['Google'] == TRUE) {
    ($rules) ? rules_invoke_event('social_network_user_detection_rules_condition_gmail') : '';
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function to detect Twitter.
 */
function _social_network_user_detection_rules_detect_twitter($rules = TRUE) {
  if (isset($_SESSION['Twitter']) && $_SESSION['Twitter'] == TRUE) {
    ($rules) ? rules_invoke_event('social_network_user_detection_rules_condition_twitter') : '';
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function to detect FB.
 */
function _social_network_user_detection_rules_detect_fb($rules = TRUE) {
  if (isset($_SESSION['Facebook']) && $_SESSION['Facebook'] == TRUE) {
    ($rules) ? rules_invoke_event('social_network_user_detection_rules_condition_fb') : '';
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Implements hook_rules_condition_info().
 */
function social_network_user_detection_rules_condition_info() {
  return array(
    'social_network_user_detection_rules_condition_fb' => array(
      'group' => 'Social network user detection',
      'label' => t('Social network user detection - FB'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_twitter' => array(
      'group' => 'Social network user detection',
      'label' => t('Social network user detection - Twitter'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_gmail' => array(
      'group' => 'Social network user detection',
      'label' => t('Social network user detection - Gmail'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_gplus' => array(
      'group' => 'Social network user detection',
      'label' => t('Social network user detection - Google Plus'),
      'module' => 'social_network_user_detection',
    ),
    'social_network_user_detection_rules_condition_hyves' => array(
      'group' => 'Social network user detection',
      'label' => t('Social network user detection - Hyves'),
      'module' => 'social_network_user_detection',
    ),
  );
}

/**
 * Call back function for event FB.
 */
function social_network_user_detection_rules_condition_fb($settings) {
  return _social_network_user_detection_rules_detect_fb(FALSE);
}

/**
 * Call back function for event Twitter.
 */
function social_network_user_detection_rules_condition_twitter($settings) {
  return _social_network_user_detection_rules_detect_twitter(FALSE);
}

/**
 * Call back function for event Gmail.
 */
function social_network_user_detection_rules_condition_gmail($settings) {
  return _social_network_user_detection_rules_detect_gmail(FALSE);
}

/**
 * Call back function for event Google Plus..
 */
function social_network_user_detection_rules_condition_gplus($settings) {
  return _social_network_user_detection_rules_detect_gplus(FALSE);
}

/**
 * Call back function for event Hyves.
 */
function social_network_user_detection_rules_condition_hyves($settings) {
  return _social_network_user_detection_rules_detect_hyves(FALSE);
}
