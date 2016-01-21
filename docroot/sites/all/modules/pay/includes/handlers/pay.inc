<?php

/**
 * @file
 * The base class for the Payment API.
 *
 * All functionality for pay_form, pay_transaction and pay_activity handlers
 * extend this class, which provides basic accessors and data storage.
 */

class pay {
  var $table = '';
  var $key = '';
  var $permissions = array(
    'default' => array('all' => 1),
  );

  function __construct($values = NULL) {
    $current = (array) $this;
    if ($values) {
      foreach ((array) $values as $name => $val) {
        if ($name == 'settings' && !empty($val) && is_scalar($val)) {
          $this->__construct(unserialize($val));
        }
        elseif (array_key_exists($name, $current) && !is_null($val)) {
          $func = 'set_' . $name;
          if (method_exists($this, $func)) {
            $this->$func($val);
          }
          else {
            $this->$name = $val;
          }
          unset($current[$name]);
        }
      }
    }
    // Set defaults if possible.
    foreach ($current as $name => $val) {
      $func = 'set_' . $name;
      if (method_exists($this, $func)) {
        $this->$func($this->$name);
      }
    }
  }

  /**
   * Do not allow this value to be automatically set.
   */
  function set_key() {
    return;
  }

  /**
   * Do not allow this value to be automatically set.
   */
  function set_table() {
    return;
  }

  function set_hostname($val = NULL) {
    if (!$val) {
      $val = ip_address();
    }
    $this->hostname = check_plain($val);
  }

  function set_notes($val = NULL) {
    $this->notes = filter_xss($val);
  }

  function set_identifer($val = NULL) {
    $this->identifer = filter_xss($val);
  }

  function set_status($status = 1) {
    $this->status = (int) (bool) $status;
  }

  function set_total($val = 0) {
    $this->total = round((float) $val, 2);
  }

  function set_total_paid($val = 0) {
    $this->total_paid = round((float) $val, 2);
  }

  function timestamp_value($val = NULL) {
    if (!$val) {
      $val = REQUEST_TIME;
    }
    if (!is_numeric($val)) {
      $val = strtotime($val);
    }
    return (int) $val;
  }

  function set_created($val = NULL) {
    $this->created = $this->timestamp_value($val);
  }

  function set_timestamp($val = NULL) {
    $this->timestamp = $this->timestamp_value($val);
  }

  function set_completed($val = NULL) {
    if ($val) {
      $this->completed = $this->timestamp_value($val);
    }
  }

  function set_uid($val = NULL) {
    if (!$val) {
      global $user;
      $val = $user->uid;
    }
    $this->uid = (int) $val;
  }

  function set_mail($val = NULL) {
    if (valid_email_address($val)) {
      $this->mail = $val;
    }
  }

  function set_handler() {
    $this->handler = get_class($this);
  }

  function handler() {
    $this->set_handler();
    return $this->handler;
  }

  function handler_title() {
    if ($info = pay_handlers($this->table, $this->handler())) {
      return filter_xss($info['title']);
    }
  }

  function disable() {
    $this->set_status(FALSE);
    $this->save();
  }

  function enable() {
    $this->set_status(TRUE);
    $this->save();
  }

  function menu_path() {
    if ($this->menu_path) {
      return check_plain($this->menu_path);
    }
  }

  function title() {
    return filter_xss($this->title);
  }

  function uid() {
    return (int) $this->uid;
  }

  function user() {
    return user_load($this->uid());
  }

  function total() {
    $total = (float) $this->total;
    if (strpos($total, '.') === FALSE) {
      $total .= '.00';
    }
    if (strpos($total, '.') === 0) {
      $total = '0.' . $total;
    }
    return $total;
  }

  function notes() {
    if (isset($this->notes)) {
      $notes_format = $this->set_notes_format();
      return check_markup($this->notes, $notes_format);
    }
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_form() {
    if ($this->pfid) {
      return pay_form_load($this->pfid);
    }
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_transaction() {
    if ($this->pxid) {
      return pay_transaction_load($this->pxid);
    }
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_activity() {
    if ($this->paid) {
      return pay_activity_load($this->paid);
    }
  }

  function form_setup(&$form, &$form_state, $form_type = 'form') {
    // By default, the settings form simply adds its identifiers to the form.
    $handler = $this->handler();
    $key = $this->key;

    // If the form has not been officially submitted, but the handler
    // is present in $_REQUEST, it may be from a sub-form (e.g. an amount-only
    // form). Set the POSTed values.
    if (!$form_state['submitted'] && isset($_REQUEST[$handler])) {
      foreach ($_REQUEST[$handler] as $posted_name => $posted_val) {
        $func = 'set_' . $posted_name;

        // It is up to the handler to sanitize the value in its set_* functions.
        if (method_exists($this, $func)) {
          $this->$func($posted_val);
        }
      }
    }

    if (!isset($form['#pay'])) {
      $form['#pay'] = array();
      $form['#after_build'][] = 'pay_after_build';
    }

    // TODO: check this, variable not set on 2nd step of
    // admin/config/pay/pay/add so we're adding in a default setting.
    if (empty($form_state['pay_form_type'])) $form_state['pay_form_type'] = $form_type;

    $values = (array) $this;
    $values['type'] = $this->table;
    $values['form'] = $form_type;
    $values['form_type'] = $form_state['pay_form_type'];

    $class = array(
      'pay',
      'pay-' . $form_type,
      'pay-' . $handler,
      'pay-' . $form_type . '-' . $form_state['pay_form_type'],
    );
    $form['#attributes']['class'] = empty($form['#attributes']['class']) ? $class : $form['#attributes']['class'] + $class;

    $form['#pay'][] = $values;

    $form[$handler]['#tree'] = TRUE;
    $form[$handler]['#group'] = $handler;
  }

  function form_values($form_state) {
    // Create a handy array of form_values for subforms.
    $values = $form_state['values'][$this->handler()];
    return $this->form_values = array_merge($form_state['values'], $values);
  }

  function settings_form(&$form, &$form_state) {
    $this->form_setup($form, $form_state, 'settings_form');

    if (!isset($form['submit']) && !isset($form['buttons'])) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save form'),
        '#weight' => 100,
      );
    }
  }

  function settings_form_validate($form, &$form_state) {
  }

  function settings_form_submit($form, &$form_state) {
    $this->form_values($form_state);
    $this->save($this->form_values);
  }

  function form(&$form, &$form_state) {
    // Call form_setup to include the the form's id and other settings
    $this->form_setup($form, $form_state);
  }

  function form_validate($form, &$form_state) {
  }

  function form_submit($form, &$form_state) {
  }

  function save($values = array()) {
    $this->__construct($values);
    $settings = (array) $this;
    $exclude = array('key', 'table', 'form_values');
    if (isset($this->table)) {
      $schema = drupal_get_schema($this->table);
      $settings = (array) $this;
      foreach ($this as $key => $val) {
        if (isset($schema['fields'][$key]) || in_array($key, $exclude)) {
          unset($settings[$key]);
        }
      }
      $this->settings = $settings;
      $this->handler = get_class($this);

      $this->new = isset($this->{$this->key}) ? FALSE : TRUE;
      $update = $this->new ? array() : $this->key;

      // Save this item.
      drupal_write_record($this->table, $this, $update);

      // Invoke a Drupal hook based on the table name (e.g. pay_form_create).
      $hook = $this->new ? '_create' : '_update';
      $this->drupal_invoke($this->table . $hook, $values);
    }
  }

  function permissions_settings($activity = 'view', $title = NULL) {
    $perm = $this->permissions[$activity];
    if (!$title) {
      $title = t('Permissions');
    }

    $element = array(
      '#type' => 'checkboxes',
      '#title' => $title ? $title : t('Permissions'),
      '#parents' => array($this->handler(), 'permissions', $activity),
      '#required' => TRUE,
      '#tree' => TRUE,
    );

    $all = (!empty($perm['all']) ? $perm['all'] : 0);
    $element['all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Everyone'),
      '#default_value' => $all,
    );
    $options['all'] = 'all';

    // The owner of this pay item.
    $owner = (!empty($perm['owner']) ? $perm['owner'] : 0);
    $element['owner'] = array(
      '#type' => 'checkbox',
      '#title' => t('Owner'),
      '#default_value' => $owner,
    );
    $options['owner'] = 'owner';

    $permissions = array(
      'administer pay',
      'administer payments for any form',
      'make payments on any form',
    );
    foreach (pay_handlers('pay_form') as $name => $info) {
      $permissions[] = 'administer payments for ' . $name . ' forms';
      $permissions[] = 'make payments on ' . $name . ' forms';
    }
    foreach ($permissions as $name) {
      $default = (!empty($perm['permission:' . $name]) ? $perm['permission:' . $name] : 0);
      $element['permission:' . $name] = array(
        '#type' => 'checkbox',
        '#title' => t('Users of any role with the %perm permission', array('%perm' => $name)),
        '#default_value' => $default,
      );
      $options['permission:' . $name] = 'permission:' . $name;
    }
    foreach (user_roles() as $rid => $role) {
      $default = (!empty($perm['role:' . $rid]) ? $perm['role:' . $rid] : 0);
      $element['role:' . $rid] = array(
        '#type' => 'checkbox',
        '#title' => t('Members of the %role role', array('%role' => $role)),
        '#default_value' => $default,
      );
      $options['role:' . $rid] = 'role:' . $rid;
    }
    $element['#options'] = $options;

    return $element;
  }

  function access($activity, $account = NULL) {
    if (!$account) {
      global $user;
      $account = $user;
    }

    // In the absence of any permissions, default access to TRUE.
    if (!is_array($this->permissions)) {
      return TRUE;
    }

    if (is_array($this->permissions) && $perm = $this->permissions[$activity]) {
      foreach ($perm as $key => $status) {
        if (!$status) {
          continue;
        }

        // The owner of this item.
        if ($key == 'owner' && $this->uid) {
          if ($account->uid == $this->uid) {
            return TRUE;
          }
        }

        // The 'Everyone' permission.
        if ($key == 'all') {
          return TRUE;
        }

        // Any user with a named permission.
        if (substr($key, 0, 11) == 'permission:') {
          if (user_access(substr($key, 11), $account)) {
            return TRUE;
          }
        }

        // Any user with the defined role.
        if (substr($key, 0, 5) == 'role:') {
          if (isset($account->roles[substr($key, 5)])) {
            return TRUE;
          }
        }
      }
    }

    // If none of the above code returned TRUE, return false by default.
    return FALSE;
  }

  /**
   * Execute an named Drupal hook function, passing $this as the first
   * parameter.
   */
  function drupal_invoke($hook, $values = array()) {
    foreach (module_implements($hook) as $name) {
      $func = $name . '_' . $hook;
      $func($this, $values);
    }
  }
}
