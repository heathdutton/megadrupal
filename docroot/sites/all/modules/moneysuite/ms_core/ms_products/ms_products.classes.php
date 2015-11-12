<?php

// ======================================
// Classes
// ======================================

/**
 * MsProductsPlan class
 */
class MsProductsPlan
{
  // Declare properties.
  public $uid = 0;
  public $sku = '';
  public $name = '';
  public $description = '';
  public $recurring_schedule = array(
    'total_occurrences' => 0,
    'main_amount' => 0,
    'main_length' => 0,
    'main_unit' => 'M',
    'has_trial' => FALSE,
    'trial_amount' => 0,
    'trial_length' => 0,
    'trial_unit' => 'W',
    'fixed_date' => 0,
    'fixed_date_discount' => 1,
    'fixed_date_type' => 'M',
    'fixed_date_string' => '',
  );

  public $total_occurrences = 0;
  public $signup_mail_subject = '';
  public $signup_mail_body = '';
  public $eot_mail_subject = '';
  public $eot_mail_body = '';
  public $cancel_mail_subject = '';
  public $cancel_mail_body = '';
  public $modify_mail_subject = '';
  public $modify_mail_body = '';
  public $expiring_mail_subject = '';
  public $expiring_mail_body = '';
  public $expiring_mail_days = 3;
  public $weight = 0;
  public $shippable = FALSE;
  public $bundle = '';
  public $cart_type = 'cart';
  public $expire_when = 'subscr_eot';
  public $allow_roles = array();
  public $deny_roles = array();
  public $modify_options = array();
  public $data = array(
    'allow_multiple' => FALSE,
    'recurring_optional' => FALSE,
    'shippable' => FALSE,
    'grant_credit' => TRUE,
    'change_plan_options' => TRUE,
  );
  public $plan_options = array();

  /**
   * Constructor - set some additional defaults
   */
  function __construct($bundle = NULL) {
    if (!is_null($bundle)) {
      $this->bundle = $bundle;
    }
    $this->set_defaults($bundle);
  }

  public function set_defaults($bundle = NULL) {
    $this->allow_roles = array();
    $this->deny_roles = array();
    $this->modify_options = array(
      'upgrade' => array(),
      'downgrade' => array(),
    );
    $this->data = array(
      'allow_multiple' => FALSE,
      'recurring_optional' => FALSE,
      'shippable' => FALSE,
      'grant_credit' => TRUE,
      'change_plan_options' => TRUE,
    );
    if (!is_null($bundle) && ($bundle_info = ms_products_get_bundle($bundle))) {
      // Add translatable string defaults
      $this->signup_mail_subject = t("Thank you for Signing Up!");
      $this->signup_mail_body = t("Dear [ms_core_order:customerName],
You have purchased the [ms_products_plan:name] @plan_name for [ms_core_payment:paymentAmountPlain] on [current-date:long],
and your account access has been upgraded. Thank you!

Sincerely,
[site:name]
[site:url]", array('@plan_name' => $bundle_info['plan_name']));
      $this->eot_mail_subject = t("Your @purchase_name has expired",
        array('@purchase_name' => $bundle_info['purchase_name']));
      $this->eot_mail_body = t("Dear [ms_core_order:customerName],
Your [ms_products_plan:name] @purchase_name has expired or been cancelled.

To renew your @purchase_name, please click the following link: [ms_products_purchase:renew-uri]

Sincerely,
[site:name]
[site:url]", array('@purchase_name' => $bundle_info['purchase_name']));
      $this->cancel_mail_subject = t('@purchase_name Cancelled',
        array('@purchase_name' => $bundle_info['purchase_name']));
      $this->cancel_mail_body = t("Dear [ms_core_order:customerName],
Your [ms_products_plan:name] @purchase_name has been cancelled and will not automatically renew anymore.
Access will be removed at the end of the term.

Sincerely,
[site:name]
[site:url]", array('@purchase_name' => $bundle_info['purchase_name']));
      $this->modify_mail_subject = t('@purchase_name changed',
        array('@purchase_name' => $bundle_info['purchase_name']));
      $this->modify_mail_body = t("Dear [ms_core_order:customerName],
Your @purchase_name has been successfully changed.

Sincerely,
[site:name]
[site:url]", array('@purchase_name' => $bundle_info['purchase_name']));
      $this->expiring_mail_subject = t("Your @purchase_name is Expiring Soon!", array('@purchase_name' => $bundle_info['purchase_name']));
      $this->expiring_mail_body = t("Dear [ms_core_order:customerName],
Your [ms_products_plan:name] @purchase_name will expire on [ms_products_purchase:expiration:short]. To renew your @purchase_name,
click the following link: [ms_products_purchase:renew-uri]

Sincerely,
[site:name]
[site:url]", array('@purchase_name' => $bundle_info['purchase_name']));

      // Add and Override email templates if needed.
      foreach ($bundle_info['emails'] as $email_name => $email_info) {
        if (!empty($email_info['extra'])) {
          $this->data['emails'][$email_name] = $email_info;
        } else {
          if (isset($email_info['subject'])) {
            $this->{$email_name . '_mail_subject'} = $email_info['subject'];
          }
          if (isset($email_info['body'])) {
            $this->{$email_name . '_mail_body'} = $email_info['body'];
          }
        }
      }
    }
  }

  public function initialize() {
    // Add the plan fields.
    if (isset($this->pid)) {
      field_attach_load('ms_products_plan', array($this->pid => $this));
    }
    $this->primeRecurringSchedule();
    $this->decodeEmails();
    $this->addCustomFields();

    // Pass everything through the translation function.
    $this->name = ms_core_translate('plan:' . $this->sku . ':' . 'name', $this->name, 'ms_products_plan');
    $this->description = ms_core_translate('plan:' . $this->sku . ':' . 'description', $this->description, 'ms_products_plan');
    $this->recurring_schedule['main_amount'] = ms_core_translate('plan:' . $this->sku . ':' . 'main_amount', $this->recurring_schedule['main_amount'], 'ms_products_plan');
    $this->recurring_schedule['trial_amount'] = ms_core_translate('plan:' . $this->sku . ':' . 'trial_amount', $this->recurring_schedule['trial_amount'], 'ms_products_plan');
    $this->signup_mail_subject = ms_core_translate('plan:' . $this->sku . ':' . 'signup_mail_subject', $this->signup_mail_subject, 'ms_products_plan');
    $this->signup_mail_body = ms_core_translate('plan:' . $this->sku . ':' . 'signup_mail_body', $this->signup_mail_body, 'ms_products_plan');
    $this->expiring_mail_subject = ms_core_translate('plan:' . $this->sku . ':' . 'expiring_mail_subject', $this->expiring_mail_subject, 'ms_products_plan');
    $this->expiring_mail_body = ms_core_translate('plan:' . $this->sku . ':' . 'expiring_mail_body', $this->expiring_mail_body, 'ms_products_plan');
    $this->eot_mail_subject = ms_core_translate('plan:' . $this->sku . ':' . 'eot_mail_subject', $this->eot_mail_subject, 'ms_products_plan');
    $this->eot_mail_body = ms_core_translate('plan:' . $this->sku . ':' . 'eot_mail_body', $this->eot_mail_body, 'ms_products_plan');
    $this->cancel_mail_subject = ms_core_translate('plan:' . $this->sku . ':' . 'cancel_mail_subject', $this->cancel_mail_subject, 'ms_products_plan');
    $this->cancel_mail_body = ms_core_translate('plan:' . $this->sku . ':' . 'cancel_mail_body', $this->cancel_mail_body, 'ms_products_plan');
    $this->modify_mail_subject = ms_core_translate('plan:' . $this->sku . ':' . 'modify_mail_subject', $this->modify_mail_subject, 'ms_products_plan');
    $this->modify_mail_body = ms_core_translate('plan:' . $this->sku . ':' . 'modify_mail_body', $this->modify_mail_body, 'ms_products_plan');

    if (!empty($this->data['emails'])) {
      foreach ($this->data['emails'] as $email_name => &$email_info) {
        $email_info['subject'] = ms_core_translate('plan:' . $this->sku . ':' . $email_name . '_mail_subject', $email_info['subject'], 'ms_products_plan');
        $email_info['body'] = ms_core_translate('plan:' . $this->sku . ':' . $email_name . '_mail_body', $email_info['body'], 'ms_products_plan');
      }
    }

    // Give other modules a chance to add data to the product plan.
    drupal_alter('ms_products_plan_load', $this);
  }

  private function decodeEmails() {
    // Decode the emails.
    $this->signup_mail_subject = htmlspecialchars_decode($this->signup_mail_subject);
    $this->signup_mail_body = htmlspecialchars_decode($this->signup_mail_body);
    $this->cancel_mail_subject = htmlspecialchars_decode($this->cancel_mail_subject);
    $this->cancel_mail_body = htmlspecialchars_decode($this->cancel_mail_body);
    $this->expiring_mail_subject = htmlspecialchars_decode($this->expiring_mail_subject);
    $this->expiring_mail_body = htmlspecialchars_decode($this->expiring_mail_body);
    $this->modify_mail_subject = htmlspecialchars_decode($this->modify_mail_subject);
    $this->modify_mail_body = htmlspecialchars_decode($this->modify_mail_body);
    $this->eot_mail_subject = htmlspecialchars_decode($this->eot_mail_subject);
    $this->eot_mail_body = htmlspecialchars_decode($this->eot_mail_body);

    // Add and Override email templates if needed.
    if (!empty($this->data['emails'])) {
      foreach ($this->data['emails'] as $email_name => $email_info) {
        if (isset($email_info['subject'])) {
          $this->{$email_name . '_mail_subject'} = htmlspecialchars_decode($email_info['subject']);
        }
        if (isset($email_info['body'])) {
          $this->{$email_name . '_mail_body'} = htmlspecialchars_decode($email_info['body']);
        }
      }
    }
  }

  private function addCustomFields() {
    // Add custom elements.
    if (!empty($this->bundle) && ($bundle_info = ms_products_get_bundle($this->bundle))) {
      foreach ($bundle_info['plan_fields'] as $custom_field => $custom_field_info) {
        $this->$custom_field = isset($this->data[$custom_field]) ? $this->data[$custom_field] : $custom_field_info['#default_value'];
      }
    }

    // Add the plan options as well, from the database.
    $result = db_select('ms_products_plan_options', 'po')
      ->fields('po')
      ->condition('po.sku', $this->sku)
      ->execute();
    foreach ($result as $record) {
      $this->plan_options[$record->name] = $record;
    }
  }

  public function primeRecurringSchedule() {
    // If the plan is a fixed date one, we need to change the
    // recurring schedule.
    if (!empty($this->recurring_schedule['fixed_date'])) {
      if ($this->cart_type == 'recurring') {
        $this->recurring_schedule['main_length'] = 1;
        $this->recurring_schedule['main_unit'] = $this->recurring_schedule['fixed_date_type'];
        $this->recurring_schedule['has_trial'] = TRUE;
        $this->recurring_schedule['trial_amount'] = ($this->recurring_schedule['fixed_date_discount']) ? ms_products_calculate_prorated_amount($this->recurring_schedule['fixed_date_type'], $this->recurring_schedule['fixed_date_string'], $this->recurring_schedule['main_amount']) : $this->recurring_schedule['main_amount'];
        $this->recurring_schedule['trial_length'] = ms_products_calculate_days_left($this->recurring_schedule['fixed_date_type'], $this->recurring_schedule['fixed_date_string']);
        $this->recurring_schedule['trial_unit'] = 'D';
      } else {
        $this->recurring_schedule['total_occurrences'] = 1;
        $this->recurring_schedule['main_amount'] = ($this->recurring_schedule['fixed_date_discount']) ? ms_products_calculate_prorated_amount($this->recurring_schedule['fixed_date_type'], $this->recurring_schedule['fixed_date_string'], $this->recurring_schedule['main_amount']) : $this->recurring_schedule['main_amount'];
        $this->recurring_schedule['main_length'] = ms_products_calculate_days_left($this->recurring_schedule['fixed_date_type'], $this->recurring_schedule['fixed_date_string']);
        $this->recurring_schedule['main_unit'] = 'D';
        $this->recurring_schedule['has_trial'] = FALSE;
        $this->recurring_schedule['trial_amount'] = 0;
        $this->recurring_schedule['trial_length'] = 0;
        $this->recurring_schedule['trial_unit'] = 'D';
      }
    }
  }

  public function save() {
    // Update or insert a new plan.
    if (isset($this->pid) && !is_null($this->pid)) {
      $result = drupal_write_record('ms_products_plans', $this, 'pid');
    } else {
      $result = drupal_write_record('ms_products_plans', $this);
    }

    // Also save the plan options.
    // First delete any existing plan options.
    db_delete('ms_products_plan_options')
      ->condition('sku', $this->sku)
      ->execute();
    foreach ($this->plan_options as $option_info) {
      // Create a new record.
      $option_info = (object)$option_info;
      $option_info->sku = $this->sku;
      drupal_write_record('ms_products_plan_options', $option_info);
    }

    return $result;
  }
}

/**
 * MsProductsPurchase class
 */
class MsProductsPurchase
{
  // Declare properties.
  public $id = NULL;
  public $oid = 0;
  public $uid = 0;
  public $bundle = '';
  public $sku = '';
  public $pid = 0;
  public $status = '';
  public $expiration = 0;
  public $start_date = 0;
  public $current_payments = 0;
  public $max_payments = 0;
  public $data = array();
  public $fields = array();
  public $options = array();

  /**
   * Constructor - set some additional defaults.
   */
  function __construct($bundle = NULL) {
    $this->start_date = REQUEST_TIME;
    if (!is_null($bundle)) {
      $this->bundle = $bundle;
    }
  }

  public function initialize() {
    // Add the purchase fields.
    field_attach_load('ms_products_purchase', array($this->id => $this));

    if (!empty($this->bundle) && ($bundle_info = ms_products_get_bundle($this->bundle))) {
      foreach ($bundle_info['purchase_fields'] as $field_name => $field_info) {
        $this->fields[$field_name] = ms_products_get_purchase_field($this->bundle, $this->id, $field_name);
      }
    }

    // Add the purchase options.
    $result = db_select('ms_products_purchase_options', 'po')
      ->fields('po')
      ->condition('po.pid', $this->id)
      ->execute();

    foreach ($result as $option) {
      $this->options[$option->name] = $option;
    }
  }

  public function save() {
    field_attach_presave('ms_products_purchase', $this);
    // Update or insert a new purchase.
    if (!is_null($this->id)) {
      drupal_write_record('ms_products_purchases', $this, 'id');
      field_attach_update('ms_products_purchase', $this);
    } else {
      drupal_write_record('ms_products_purchases', $this);
      field_attach_insert('ms_products_purchase', $this);
    }

    // Next, save the options.
    if (!empty($this->options)) {
      foreach ($this->options as $option_info) {
        if (isset($option_info->id)) {
          if ($option_info == 'inactive') {
            db_delete('ms_products_purchase_options')
              ->condition('id', $option_info->id)
              ->execute();
          } else {
            // Update the existing record.
            drupal_write_record('ms_products_purchase_options', $option_info, 'id');
          }
        } elseif ($option_info->status != 'inactive') {
          // Create a new record.
          $option_info->pid = $this->id;
          drupal_write_record('ms_products_purchase_options', $option_info);
        }
      }
    }
  }
}
