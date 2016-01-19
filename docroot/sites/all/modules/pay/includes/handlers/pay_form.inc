<?php

/**
 * @file
 * The base class for payment activities. All payment form classes should
 * extend this.
 */

class pay_form extends pay {

  var $pfid;
  var $title;
  var $status = 1;
  var $embeddable = FALSE;
  var $uid;
  var $min_amount = 1.00;
  var $max_amount = 1000.00;
  var $menu_path;
  var $total_goal;
  var $currency;
  var $pay_methods = array();
  var $notes_title = 'Comments';
  var $notes_format = NULL;
  var $notes_description = '';

  var $user_register = FALSE;

  var $table = 'pay_form';
  var $key = 'pfid';

  function title() {
    return check_plain($this->title);
  }

  function set_notes_format() {
    $this->notes_format = filter_fallback_format();
    return $this->notes_format;
  }

  function set_transaction($values) {
    module_load_include('inc', 'pay', 'includes/handlers/pay_transaction');
    $this->transaction = New pay_transaction($values);
    if (!$this->transaction->pfid) {
      $this->transaction->pfid = $this->pfid;
    }
    $this->transaction->save($values);
    return $this->transaction;
  }

  /**
   * Modify the list of payment actions that are valid for a given transaction.
   *
   * @param $pay_transaction
   *  An existing transaction that is using this pay_form.
   * @param $actions
   *  An array of default actions, passed by reference. Modify or augment this
   *  as needed.
   */
  function set_valid_actions($pay_transaction, &$actions) {
    foreach ($this->pay_methods() as $pay_method) {
      $pay_method->set_valid_actions($this, $actions);
    }
  }

  /**
   * List of all currencies available for this form.
   */
  function currency_list() {
    if (!isset($this->currency_list)) {

      // By default, return the one globally-selected currency.
      $global  = pay_currency_list();
      $this->currency_list = array(
        $this->currency() => $global[$this->currency()],
      );

      // Multiple currency support is handled by a separate module that must
      // set this variable and deal with the conversion fallout responsibly.
      if (variable_get('pay_currency_multiple', FALSE)) {
        foreach ($this->pay_methods() as $method) {
          foreach ($method->available_currencies() as $currency) {
            if (!isset($this->currency_list[$currency])) {
              $this->currency_list[$currency] = $currency;
            }
          }
        }
      }
      drupal_alter('pay_currency_list', $this->currency_list);
    }
    return $this->currency_list;
  }

  /**
   * Set the currency for this payment form.
   */
  function set_currency($value = NULL) {
    if (!$value) {
      $value = variable_get('pay_currency', NULL);
    }
    $this->currency = $value;
  }

  function currency() {
    if (!isset($this->currency)) {
      $this->set_currency();
    }
    return $this->currency;
  }

  function user_register() {
    global $user;
    if ($user->uid) {
      return FALSE;
    }
    if ($this->user_register == 'none') {
      return FALSE;
    }
    return $this->user_register;
  }

  function set_user_register($value = FALSE) {
    if (in_array($value, array(FALSE, 'optional', 'required'))) {
      $this->user_register = $value;
    }
    else {
      $this->user_register = FALSE;
    }
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_method_list() {
    $list = array();
    $result = db_query("SELECT * FROM {pay_method} WHERE STATUS = 1 ORDER BY title");
    while ($row = $result->fetchObject()) {
      $list[$row->pmid] = $row->title;
    }
    return $list;
  }

  function set_embeddable($value = FALSE) {
    $this->embeddable = (bool) $value;
  }

  function embeddable() {
    return (bool) $this->embeddable;
  }

  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);
    $group = $this->handler();

    // If the parent form has not supplied a title, add one.
    if (!isset($form['title'])) {
      $form[$group]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#default_value' => $this->title,
        '#required' => TRUE,
        '#description' => t('This title will appear as the title of the form page and in listings.'),
      );
    }

    $form[$group]['min_amount'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Minimum amount'),
      '#description' => t('The minimum allowed payment on this form. If there are service fees or other costs associated with receiving payments, you will want this to set this amount higher than those costs.'),
      '#default_value' => $this->min_amount,
    );

    $form[$group]['max_amount'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Maximum amount'),
      '#description' => t('The maximum allowed amount for this form. If your payment method has a limit, this setting should reflect it.'),
      '#default_value' => $this->max_amount,
    );

    $form[$group]['pay_methods'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Available payment methods'),
      '#description' => t('Payment methods available for this form.'),
      '#options' => $this->pay_method_list(),
      '#default_value' => $this->pay_methods,
    );

    /**
     * @todo Disable this until it's ready for prime time.
     *
     $form[$group]['user_register'] = array(
     '#type' => 'radios',
     '#title' => t('Provide a user registration form'),
     '#description' => t('Allow users to register or login while making a payment.'),
     '#options' => array(
     'none' => t('No: Transactions will be recorded anonymously.'),
     'optional' => t('Optional: If users do not register or login, their transaction will be recorded anonymously.'),
     'required' => t('Required: Users must register or login and payments will be associated with their account.'),
     ),
     '#default_value' => $this->user_register,
     );
     */
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_methods() {
    $methods = array();
    foreach ($this->pay_methods as $pmid => $status) {
      if ($status && $pay_method = pay_method_load($pmid)) {
        if ($pay_method->access('default')) {
          $pay_method->pay_form = $this;
          $methods[$pmid] = $pay_method;
        }
      }
    }
    return $methods;
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_method_form(&$form, &$form_state) {
    $group = $this->handler();
    $methods = $this->pay_methods();

    $form[$group]['pay_method']['selected'] = array();

    foreach ($methods as $pmid => $pay_method) {
      $options[$pmid] = $pay_method->title;
      $pay_method->form($form, $form_state);

      // Make sure each payment method has a total.
      if (!isset($form[$group]['pay_method'][$pmid]['total'])) {
        $form[$group]['pay_method'][$pmid]['total'] = array(
          '#type' => 'value',
          '#value' => 0.00,
        );
      }
    }

    $form[$group]['pay_method']['selected'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#required' => TRUE,
      '#access' => count($methods) > 1,
      '#default_value' => count($methods) == 1 ? $methods : NULL,
    );
  }

  function form(&$form, &$form_state) {
    parent::form($form, $form_state);

    $group = $this->handler();

    // If an amount-only form was submitted, get the amount from $_GET.
    if (isset($_GET[$group]['amount'])) {
      $this->set_amount($_GET[$group]['amount']);
    }

    $form[$group]['total'] = array(
      '#type' => 'textfield',
      '#size' => 6,
      '#title' => t('Amount'),
    );

    // Include a user registration form, if necessary.
    if ($this->user_register()) {
      $required = ($this->user_register() == 'required');
      $register = drupal_retrieve_form('user_register', $form_state);
      drupal_prepare_form('user_register', $register, $form_state);

      $exclude = array('form_id', '#build_id', '#id', '#method', '#action', 'submit');
      foreach ($register as $key => $value) {
        if (!in_array($key, $exclude)) {

          if ($key[0] != '#' && is_array($value)) {
            // Manage 'required' if the account registration isn't.
            if (!$required && isset($value['#required'])) {
              $value['#user_register_required'] = $value['#required'];
              unset($value['#required']);
            }

            // Set #parents so that the value can be located by pay.
            $value['#parents'] = array($group, 'register', $key);
          }

          // Add the registration form's field to a user registration element.
          $form[$group]['register'][$key] = $value;
        }
      }
    }

    // Add a payment method selection form.
    $this->pay_method_form($form, $form_state);

    $form[$group]['notes'] = array(
      '#type' => 'textarea',
      '#title' => $this->notes_title,
      '#description' => $this->notes_description,
    );
  }

  function form_alter(&$form, &$form_state) {
    $group = $this->handler();

    // Special handling for "amount-only" forms.
    if ($form_state['pay_form_type'] == 'amount') {
      $visible = array();

      foreach (element_children($form[$group]) as $key) {
        if (!in_array($key, array('amount', 'amount_other', 'total'))) {
          $form[$group][$key]['#access'] = FALSE;
        }

        // Keep track of any visible elements.
        if ($form[$group][$key]['#access'] !== FALSE) {
          $visible[] = $key;
        }

        // Change the 'action' to the ultimate destination for the form.
        $form['#action'] = url($this->menu_path());

        // Change the 'method' to GET ...?
        $form['#method'] = 'get';
      }
    }

    // If only one field is visible, hide its label.
    if (isset($visible) && count($visible) == 1) {
      unset($form[$group][$visible[0]]['#title']);
    }
  }

  function form_validate($form, &$form_state) {
    parent::form_validate($form, $form_state);
    $values = $this->form_values($form_state);
    $group  = $this->handler();

    if ($this->user_register()) {
      // Copy & overwrite form values so it looks like a user_registration form.
      $register_state = $form_state;
      $register_state['values'] = $values['register'];

      // Execute all of the user_registration form's submit handlers.
      $register = $form[$this->handler()]['register'];
      form_execute_handlers('validate', $register, $register_state);

      // Copy any new $register_state values into $form_state.
      $form_state = array_merge($register_state, $form_state);
    }

    // Remove non-numeric characters from the total.
    $total = (float) preg_replace('/[^\d\.]/', '', $values['total']);

    // Confirm that the amount falls within our min/max settings.
    if (($total < $this->min_amount) || ($total > $this->max_amount) ) {
      $error = t('Please enter an amount between %min and %max', array('%min' => $this->min_amount, '%max' => $this->max_amount));
      form_set_error('total', $error);
    }

    // If there is no total value per payment method, set the global total.
    foreach ($values['pay_method']['selected'] as $pmid => $status) {
      if (!$status) {
        continue;
      }
      $method_values = $values['pay_method'][$pmid];
      $method_values['pmid'] = $pmid;
      $method_element = $form[$group]['pay_method'][$pmid];
      if (!$method_values['total']) {
        form_set_value($method_element['total'], (float) $total, $form_state);
        $method_values['total'] = $total;
      }
      $pay_method = pay_method_load($method_values);
      $pay_method->pay_method_validate($form, $form_state, $method_element);
    }
  }

  function form_submit($form, &$form_state) {
    global $user;

    parent::form_submit($form, $form_state);
    $values = $this->form_values($form_state);

    if ($this->user_register()) {
      // Copy & overwrite form values so it looks like a user_registration form.
      $register_state = $form_state;
      $register_state['values'] = $values['register'];

      // Execute all of the user_registration form's submit handlers.
      $register = $form[$this->handler()]['register'];
      form_execute_handlers('submit', $register, $register_state);

      // Copy any new $register_state values into $form_state.
      $form_state = array_merge($register_state, $form_state);

      // Did the new user get logged in? If not, we need to temporarily log her
      // in so that the transaction/actions/etc are associated with the account.
      if (!$user->uid && isset($form_state['user'])) {
        $user_anonymous = $user;
        $user = $form_state['user'];
      }
    }

    // Add a new transaction for this form with status as default of 'pending'.
    $transaction = $this->set_transaction($values);

    // Run payment activities for the 1 or more selected payment methods.
    $selected = $values['pay_method']['selected'];
    if (!is_array($selected)) {
      $selected = array($selected => 1);
    }

    foreach ($selected as $pmid => $status) {
      if (!$status) {
        continue;
      }
      $method_values = $values['pay_method'][$pmid];
      $method_values['pmid'] = $pmid;
      $pay_method = pay_method_load($method_values);
      $transaction->save($method_values);

      $activity = $transaction->add_activity($pay_method);
      $activity->do_activity($pay_method->pay_form_action, $method_values);

      // Add this activity to form_state for other modules to use.
      $form_state['pay_activity'][] = $activity;
    }

    // Trigger a special hook if there's a goal amount and we have reached it.
    if ($this->total_goal() && ($this->total_paid() >= $this->total_goal())) {
      $this->drupal_invoke('pay_form_goal', $form_state);
    }

    // Leave the user object the way we found it.
    if (isset($user_anonymous)) {
      $user = $user_anonymous;
    }

    unset($form_state['rebuild'], $form_state['storage']);
  }

  function set_menu_path($val = NULL) {
    $this->menu_path = $val;
  }

  function menu_path() {
    return check_plain($this->menu_path);
  }

  function total() {
    return (float) db_query("SELECT SUM(total) as total FROM {pay_transaction} WHERE pfid = :pfid", array(':pfid' => $this->pfid))->fetchField();
  }

  function total_paid() {
    return (float) db_query("SELECT SUM(total_paid) as total FROM {pay_transaction} WHERE pfid = :pfid", array(':pfid' => $this->pfid))->fetchField();
  }

  function total_goal() {
    return (float) $this->total_goal;
  }

  function transaction_count() {
    return (int) db_query("SELECT COUNT(*) FROM {pay_transaction} WHERE pfid = :pfid", array(':pfid' => $this->pfid))->fetchField();
  }
}
