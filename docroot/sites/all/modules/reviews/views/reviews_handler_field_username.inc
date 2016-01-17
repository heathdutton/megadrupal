<?php

/**
 * Custom handler for reviews.
 */
class reviews_handler_field_username extends views_handler_field {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $uid = $values->reviews_uid;

    $result = db_select('users', 'u')
      ->fields('u', array('name'))
      ->condition('uid', $uid, '=')
      ->execute()
      ->fetchAll();

    if (count($result) != 0) {
      return $result[0]->name;
    }
    else {
      return t('Anonymous');
    }
  }

}
