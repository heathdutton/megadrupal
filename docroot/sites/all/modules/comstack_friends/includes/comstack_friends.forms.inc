<?php
/**
 * @file
 * comstack_friends.forms.inc
 */

/**
 * Admin settings form callback.
 */
function comstack_friends_admin_settings_form() {
  $form = array();

  $form['killswitch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Read only mode'),
    '#collapsible' => TRUE,
  );

  $form['killswitch']['guidance_text'] = array(
    '#markup' => '<p class="help-text">' . t('Read only mode is intended as a temporary measure and allows you to prevent new releationships being created, approved or deleted.') . '</p>',
  );

  $form['killswitch']['comstack_friends_killswitch__enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force read only mode on'),
    '#default_value' => variable_get('comstack_friends_killswitch__enabled', FALSE),
    '#description' => t("Check the box if you want to prevent users from request, approving or deleting relationships."),
  );

  $form['killswitch']['comstack_friends_killswitch__text'] = array(
    '#type' => 'textarea',
    '#title' => t('Warning message text'),
    '#description' => t('The text to display to the user when read only is enabled.'),
    '#default_value' => filter_xss(variable_get('comstack_friends_killswitch__text', '')),
    '#states' => array(
      'visible' => array(
        ':input[id="edit-comstack-friends-killswitch-enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['comstack_friends_add_links_to_profiles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add friend &amp; block links to user profiles'),
    '#default_value' => variable_get('comstack_friends_add_links_to_profiles', TRUE),
  );

  $form['comstack_friends_opted_out_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('If a user has opted out, prevent them from being blocked'),
    '#description' => t('This will hide the block link on a users profile if they do not allow friend requests.'),
    '#default_value' => variable_get('comstack_friends_opted_out_hide', FALSE),
  );

  $form['comstack_friends_dissolve_on_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('When a user blocks another user, dissolve their friendship'),
    '#description' => t('When a user blocks another user, if they have a friendship should it be ended?'),
    '#default_value' => variable_get('comstack_friends_dissolve_on_block', TRUE),
  );

  $form['comstack_friends_already_friends_hide_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the "Add friend" button if the users are already friends'),
    '#description' => t('If the users are already friends then a button will be output, but disabled. Check this to hide the button entirely.'),
    '#default_value' => variable_get('comstack_friends_already_friends_hide_button', TRUE),
  );

  $form['comstack_friends_exclude_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles to exclude from the Friend &amp; Block functionality'),
    '#description' => t('You can exclude certain roles from being friended and blocked by selecting them here. This is in addition to whatever permissions have been defined.'),
    '#options' => drupal_map_assoc(user_roles(TRUE), 'check_plain'),
    '#default_value' => variable_get('comstack_friends_exclude_roles', array()),
  );

  return system_settings_form($form);
}
