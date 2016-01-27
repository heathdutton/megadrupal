<?php
/**
 * @file
 * Provides classes for UI actions on the user data set.
 */

/**
 * The 'attach' action: attach an existing entity.
 */
class PartyUserDataSetUIAttach extends PartyDefaultDataSetUIAttach {

  /**
   * Provides the action form for attaching an existing user.
   *
   * @param $party
   *   A loaded party object.
   * @param $data_set
   *   A loaded data set.
   *   We don't actually need this loaded, but we need a menu loader to convert
   *   the path-style string to the machine name, and a menu loader that doesn't
   *   load would be weird too.
   * @param $eid
   *   The id of the entity, if relevant to this action.
   */
  function action_form($form, &$form_state, $party, $data_set, $eid = NULL) {
    $form = array();

    $form['user'] = array(
      '#type' => 'textfield',
      '#title' => t('User'),
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#description' => t('Enter the username of the account to attach.'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Attach user'),
      '#weight' => 99,
    );

    return $form;
  }

  function action_form_validate($form, &$form_state) {
    // Validate the user name.
    $account = user_load_by_name($form_state['values']['user']);
    if (!$account) {
      form_set_error('user', t('The username %name does not exist.', array('%name' => $form_state['values']['user'])));
    }
  }

  function action_form_submit($form, &$form_state) {
    // Get the original form parameters.
    list ($party, $data_set, $action, $eid) = $form_state['build_info']['args'];

    // Attempt to load a user from the given input.
    $account = user_load_by_name($form_state['values']['user']);
    if ($account->uid != 0) {
      // Attach the user entity to the party.
      $party->getDataSetController($data_set['set_name'])
        ->attachEntity($account)
        ->save();
    }
  }
}

/**
 * The 'add' action: add a new user.
 */
class PartyUserDataSetUIAdd extends PartyDefaultDataSetUIAdd {

  /**
   * Provides the action form for attaching an existing user.
   *
   * @param $party
   *   A loaded party object.
   * @param $data_set
   *   A loaded data set.
   *   We don't actually need this loaded, but we need a menu loader to convert
   *   the path-style string to the machine name, and a menu loader that doesn't
   *   load would be weird too.
   * @param $eid
   *   The id of the entity, if relevant to this action.
   */
  function action_form($form, &$form_state, $party, $data_set, $eid = NULL) {
    module_load_include('inc', 'user', 'user.pages');

    $form = user_register_form($form, $form_state);

    return $form;
  }

  function action_form_validate($form, &$form_state) {

  }

  function action_form_submit($form, &$form_state) {
    // Get the original form parameters.
    list ($party, $data_set, $action, $eid) = $form_state['build_info']['args'];

    // Attempt to load a user from the given input.
    $account = $form_state['user'];
    if ($account->uid != 0) {
      // Attach the user entity to the party.
      $party->getDataSetController($data_set['set_name'])
        ->attachEntity($account)
        ->save();
    }
  }
}
