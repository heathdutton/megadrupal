<?php

namespace Drupal\smartling\Forms;

class UserSettingsForm extends GenericEntitySettingsForm {

  public function __construct() {
    $this->entity_name_translated = t('User');
    $this->entity_key = '#user';
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_get_user_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $account = $form_state['user'];
    $category = $form['#user_category'];
    // Remove unneeded values.
    form_state_values_clean($form_state);

    // Before updating the account entity, keep an unchanged copy for use with
    // user_save() later. This is necessary for modules implementing the user
    // hooks to be able to react on changes by comparing the values of $account
    // and $edit.
    $account_unchanged = clone $account;

    entity_form_submit_build_entity('user', $account, $form, $form_state);

    // Populate $edit with the properties of $account, which have been edited on
    // this form by taking over all values, which appear in the form values too.
    $edit = array_intersect_key((array) $account, $form_state['values']);


    //todo: Check what's going on above and below this line. Might we want to delete them?
    parent::submitForm($form, $form_state);



    $form_state['values']['uid'] = $account->uid;
    if ($category == 'account' && !empty($edit['pass'])) {
      // Remove the password reset tag since a new password was saved.
      unset($_SESSION['pass_reset_' . $account->uid]);
    }
    // Clear the page cache because pages can contain
    // usernames and/or profile information.
    cache_clear_all();
  }
}