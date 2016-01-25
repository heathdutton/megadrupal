<?php

class authorizenetLogin {
  var $id;
  var $name;
  var $transaction_key;
  var $developer;

  function __construct($values = NULL) {
    $current = (array) $this;
    if ($values) {
      foreach ((array) $values as $name => $val) {
        if (array_key_exists($name, $current) && !is_null($val)) {
          $func = 'set' . str_replace('_', '', $name);
          if (method_exists($this, $func)) {
            $this->$func($val);
          }
        }
      }
    }
  }

  function setId($value = NULL) {
    $this->id = (int) ($value);
  }

  function setName($value = NULL) {
    // The name field cannot be modified once set.
    if (!isset($this->name)) {
      $this->name = check_plain($value);
    }
  }

  function setTransactionKey($value = NULL) {
    $this->transaction_key = check_plain($value);
  }

  function setDeveloper($value = NULL) {
    // The developer field cannot be modified once set.
    if (!isset($this->developer)) {
      $this->developer = (int) (bool) $value;
    }
  }

  function settingsForm(&$form, &$form_state) {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Authorize.Net login'),
      '#default_value' => $this->name,
      '#pre_render' => array('pay_element_set_required'),
      '#disabled' => isset($this->name),
    );
    $form['transaction_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Authorize.Net transaction key'),
      '#default_value' => $this->transaction_key,
      '#pre_render' => array('pay_element_set_required'),
    );
    $form['developer'] = array(
      '#type' => 'checkbox',
      '#title' => t('This is a developer test account'),
      '#description' => t('You can obtain a developer test account from http://developer.authorize.net/ and use those credentials for development and testing. If the above values belong to a developer test account and not a real merchant ac
count, check this box.'),
      '#default_value' => $this->developer,
      '#disabled' => isset($this->developer),
    );
  }

  function validate($values = array()) {
    // Reset this object with new values, if specified.
    $this->__construct($values);

    if (strlen($this->name) < 8) {
      $error = t('An Authorize.Net login must be at least 8 characters');
      form_set_error('name', $error);
    }

    if (strlen($this->transaction_key) != 16) {
      $error = t('An Authorize.Net transaction key must be 16 characters');
      form_set_error('transaction_key', $error);
    }
  }

  function save($values = array()) {
    $this->new = isset($this->{$this->id}) ? FALSE : TRUE;

    // Reset this object with new values, if specified.
    $this->__construct($values);

    // Save this item.
    if ($this->new) {
      drupal_write_record('authorizenet_login', $this);
    }
    else {
      drupal_write_record('authorizenet_login', $this, $this->id);
    }
  }
}
