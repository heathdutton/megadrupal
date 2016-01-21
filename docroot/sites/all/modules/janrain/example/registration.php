<?php
/**
 * @file
 * Login example configuration.
 */

/**
 * Implements hook_menu().
 *
 * Add verify email page to site and edit profile tab to user pages.
 */
function janrain_example_menu() {
  $menu = array();
  $menu['user/%user/edit_profile'] = array(
    'title' => 'Edit Profile',
    'access callback' => 'user_is_logged_in',
    'type' => MENU_LOCAL_TASK,
  );
  return $menu;
}

/**
 * Implements hook_block_info_alter().
 *
 * Add the profile block to pages for logged in users who have capture uuids and
 * are viewing their own profiles.
 */
function janrain_example_block_info_alter(&$blocks, $theme, $code_blocks) {

  if (!empty($blocks['user']['login'])) {
    $blocks['user']['login']['visibility'] = BLOCK_VISIBILITY_NOTLISTED;
    $blocks['user']['login']['pages'] = "janrain/verify_email\njanrain/password_recover";
  }

  $not_ajax = FALSE === strpos(current_path(), 'ajax');

  if (!isset($blocks['janrain_widgets'])) {
    // Fail fast if no janrain widgets are uploaded.
    if (user_access('administer configuration') && $not_ajax) {
      // Show admins.
      drupal_set_message(t('Janrain registration package not found.'), 'error', FALSE);
    }
    watchdog('janrain_example', 'Example enabled but no Registration widgets!');
    return;
  }
  elseif ($not_ajax) {
    // It's possible this code runs more than once on a module submit.
    // Remove the previous warnings if, on this run, there IS a widget.
    $errors = drupal_get_messages('error');
    if (!empty($errors['error'])) {
      foreach ($errors['error'] as $e) {
        // Remove error messages that aren't about missing packages.
        if ($e == t('Janrain registration package not found.')) {
          continue;
        }
        // Re-add other errors.
        drupal_set_message($e, 'error');
      }
    }
  }

  // Registrations.
  foreach ($blocks['janrain_widgets'] as &$widget) {
    if (FALSE !== strpos($widget['delta'], 'signIn_')) {
      // Add to all pages except verify_email and password_recover.
      // Clean up block list in list alter.
      $widget['status'] = 1;
      $widget['region'] = 'content';
      $widget['weight'] = 1000;
      $widget['visibility'] = BLOCK_VISIBILITY_NOTLISTED;
      $widget['pages'] = "janrain/verify_email\njanrain/password_recover";
      // Only the first one.
      break;
    }
  }
  unset($widget);

  // Profiles.
  foreach ($blocks['janrain_widgets'] as &$widget) {
    // Add profile widget to profile page.
    if (FALSE !== strpos($widget['delta'], 'editProfile_')) {
      $widget['status'] = 1;
      $widget['region'] = 'content';
      $widget['visibility'] = BLOCK_VISIBILITY_LISTED;
      $widget['pages'] = 'user/*/edit_profile';
      // Only the first one.
      break;
    }
  }
  unset($widget);

  // Verify Email.
  foreach ($blocks['janrain_widgets'] as &$widget) {
    // Add email verification widget to verify_email page.
    if (FALSE !== strpos($widget['delta'], 'verifyEmail_')) {
      $widget['status'] = 1;
      $widget['region'] = 'content';
      $widget['visibility'] = BLOCK_VISIBILITY_LISTED;
      $widget['pages'] = 'janrain/verify_email';
      // Only the first one.
      break;
    }
  }
  unset($widget);

  // Recover password.
  foreach ($blocks['janrain_widgets'] as &$widget) {
    // Add password recover widget to password_recover page.
    if (FALSE !== strpos($widget['delta'], 'resetPasswordRequestCode_')) {
      $widget['status'] = 1;
      $widget['region'] = 'content';
      $widget['visibility'] = BLOCK_VISIBILITY_LISTED;
      $widget['pages'] = 'janrain/password_recover';
      // Only the first one.
      break;
    }
  }
  unset($widget);
}

/**
 * Implements hook_block_view_alter().
 *
 * Hides the signIn widget block content when user logged in.
 */
function janrain_example_block_view_alter(&$data, $block) {
  if ('janrain_widgets' != $block->module) {
    // Skip non-janrain widgets.
    return;
  }
  if (FALSE === strpos($block->delta, 'signIn_')) {
    // Skip non-sign-in blocks.
    return;
  }
  if (user_is_logged_in()) {
    $data['content'] = FALSE;
  }
}

/**
 * Implements hook_block_view_MODULE_DELTA_alter() for user_login().
 */
function janrain_example_block_view_user_login_alter(&$data, $block) {
  $packages = _janrain_widgets_list_pkgs();
  if (!$packages) {
    // Fail fast if no widgets are configured.
    return;
  }
  if (!$data) {
    // Not sure why but this runs on pages where the login is not.
    return;
  }
  $data['content'] = array('#markup' => "<a href='javascript:;' class='capture_modal_open'>Sign In / Sign Up</a>");
}

/**
 * Implements hook_form_FORM_ID_alter() for user_profile_form().
 *
 * Remove the account and picture sections of the form provided by Drupal.
 * This information can only be edited on the Edit Profile tab.
 */
function janrain_example_form_user_profile_form_alter(&$form, &$form_state) {
  // Allow admins to still see Drupal data.
  if (!user_access('administer users')) {
    $form['account']['#prefix'] = '<div style="display:none">';
    $form['account']['#suffix'] = '</div>';
    if (isset($form['picture'])) {
      $form['picture']['#prefix'] = '<div style="display:none">';
      $form['picture']['#suffix'] = '</div>';
    }
  }
}

/**
 * Implements hook_form_user_login_alter().
 */
function janrain_example_form_user_login_alter(&$form, &$form_state) {
  array_unshift($form['#validate'], '_janrain_example_login_validate');
}

/**
 * Helper to validate login attempts using Janrain.
 */
function _janrain_example_login_validate(&$form, &$form_state) {
  $identifiers = _janrain_get_identifiers();
  foreach ($identifiers as $external_id) {
    $account = user_external_load($external_id);
    if ($account) {
      user_save($account, array('status' => 1));
    }
  }
}
