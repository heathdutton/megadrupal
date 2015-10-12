<?php
/**
 * @file
 * Pages and forms constructors and handlers.
 */

/**
 * Output AJAX data for the picture changer modal dialog.
 */
function profile_pic_changer_ajax($uid) {
  global $user;

  if ($uid != $user->uid) {
    print t("There was an error creating the page.");
    drupal_exit();
  }

  ctools_include('modal');
  ctools_include('ajax');

  $form_state = array(
    'uid'   => $user->uid,
    'title' => t("Update your profile image"),
    'ajax'  => TRUE, // This should never get called from outside of ajax.
  );

  $output = ctools_modal_form_wrapper('profile_pic_changer_form', $form_state);
  if ($form_state['executed']) {

    // Remove the confirmation messages that will have been set by
    // user.module, or they will be displayed on the next page load,
    // confusing the user.
    drupal_get_messages('status');

    $commands = array();
    $commands[] = ctools_modal_command_dismiss(t('Picture changed'));
    $commands[] = array(
      'command' => 'pic_updated',
      'argument' => profile_pic_changer_user_picture_src($user->uid),
    );

    print ajax_render($commands);
  }
  else {
    print ajax_render($output);
  }
}

/**
 * Get user picture image final URL.
 */
function profile_pic_changer_user_picture_src($uid) {
  $account = user_load($uid);
  if (!$account) {
    return '1';
  }

  $filepath = $account->picture->uri;
  if (!$filepath) {
    return '2';
  }

  $image_html = '';
  if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
    $image_html = theme('image_style',
                        array(
                          'style_name' => $style,
                          'path' => $filepath,
                        ));
  }
  else {
    $image_html = theme('image',
                        array(
                          'path' => $filepath,
                        ));
  }

  $match = array();
  if (preg_match("/src=[\"'](.*?)[\"']/", $image_html, $match)) {
    return $match[1];
  }

  return "";
}

/**
 * Form constructor for the picture changer form.
 */
function profile_pic_changer_form($form, &$form_state) {
  global $user;
  if (!$form_state['uid'] || $user->uid != $form_state['uid']) {
    return array();
  }

  // Reload $user so that we pull in fields. Even though we will chop
  // off all controls from the form but the user picture upload
  // field, if we do not do this, profile fields are left empty after
  // saving.
  $user = user_load($user->uid);

  module_load_include('inc', 'user', 'user.pages');

  // Get the default user form so we can reuse the validation and submit
  // stuff, then use hook_form_FORM_ID_alter to remove everything
  // but the picture field.
  $form = user_profile_form($form, $form_state, $user);

  // Grab the picture field (which may have been nested if field_group is
  // installed).
  $fpic = profile_pic_changer_picture_field($form);
  if (!$fpic) {
    // No picture field here.
    watchdog('profile_pic_changer', 'Error finding picture field in the profile form.', WATCHDOG_ERROR);
    return array();
  }

  // Unset anything that isn't the Save button.
  foreach ($form as $k => $v) {
    // Internal keys.
    if ($k[0] == '#') {
      continue;
    }

    // The save button.
    if ($k == 'actions') {
      continue;
    }

    // Other important fields.
    if (substr($k, 0, 4) == 'form') {
      continue;
    }

    unset($form[$k]);
  }

  // Add the pic field back.
  $form['picture'] = $fpic;

  // Pull validation on everything except our picture field.
  $form['#validate'] = array('user_validate_picture');

  return $form;
}

/**
 * Retrieve the picture field.
 *
 * @param array $element
 *   The form array to retrieve the picture field from
 *
 * @return array
 *   An array describing the picture field
 */
function profile_pic_changer_picture_field($element) {
  foreach ($element as $k => $v) {
    if ($k == 'picture' && is_array($v)) {
      return $element[$k];
    }

    if (is_array($element[$k])) {
      $test = profile_pic_changer_picture_field($element[$k]);
      if ($test) {
        return $test;
      }
    }
  }

  return NULL;
}
