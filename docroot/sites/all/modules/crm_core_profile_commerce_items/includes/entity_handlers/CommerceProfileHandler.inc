<?php

/**
 * @file
 * Commerce entity handler.
 */

/**
 * Profile entity handlers are used to add fields from different entities into
 * a single CRM Core Profile.
 */
class CommerceProfileHandler extends ProfileEntityHandler {

  /**
   * Returns entity info in array.
   */
  public function entityInfo() {
    return array(
      'entity' => 'commerce_product',
      'entity_label' => t('Commerce Profile'),
      'add_bundles' => FALSE,
      'edit_callback' => 'renderEditForm',
    );
  }

  /**
   * Assoc array of meta fields which should be handled.
   *
   * @return array
   *   Array of entity properties.
   */
  public function entityProperties() {
    return array(
      'commerce_items_amount_single' => t('Amount Field'),
      'commerce_items_cart' => t('Shopping Cart'),
      'commerce_items_customer_billing' => t('Billing Address'),
      'commerce_items_payment' => t('Payment Form'),
    );
  }

  /**
   * Creates a custom form on the edit page for managing commerce items.
   *
   * This entity does not use bundles, so we are not selecting fields from the
   * standard selector. Instead, users are selecting metafields that will be
   * used to render form elements within the profile.
   *
   * This handler will ask users to select a product and add a list of
   * metafields that are standard to any product type. This means it would not
   * be possible to display information about the selected product, but it would
   * be possible to add form elements that allow you to work with the product.
   * For instance, you could add a payment form, a cart form, an amount that
   * will be used in payment processing, etc.
   *
   * @param array $form
   *   Structured form array.
   * @param array $profile
   *   A CRM Core Profile.
   * @param string $entity_handler
   *   The entity handler class name being used.
   * @param array $entity_info
   *   Array of entity info.
   * @param string $entity_label
   *   A label.
   */
  public function renderEditForm(&$form, $profile, $entity_handler, $entity_info, $entity_label) {

    // Set default bundle for commerce_order and change description.
    $entity_type = $entity_info['entity'];
    // note: we are setting the value of the bundle to the name of the entity,
    // this should probably be used to connote that a bundle is not being
    // employed.
    $entity_bundle = $entity_info['entity'];

    if (isset($form['entities'][$entity_type])) {

      $form['entities'][$entity_type]['status']['#description'] = t('Check this box to add commerce items to your profile.');

      // @todo: this probably has some implications downstream, since we are not
      // actually using a bundle.
      // Make sure this is not breaking the form when it is being generated.
      $form['entities'][$entity_type]['bundle']['#value'] = $entity_bundle;
      $form['entities'][$entity_type]['bundle']['#type'] = 'hidden';

      // Add a field container for the selected entity.
      _crm_core_profile_add_field_container($form, $entity_type, TRUE);

      // Now we need to add in all the fields.
      $entity_config = isset($profile->entities[$entity_type]) ? $profile->entities[$entity_type] : array();
      $entity_config['always_display'] = TRUE;
      _crm_core_profile_fields_selector($form, $entity_type, $entity_config, $entity_handler, $entity_label);

      // Update tokens.
      // @todo: we are going to need to change the tokens that are added here.
      if (module_exists('token')) {
        $form['entities'][$entity_type]['token']['tokens'] = array(
          '#theme' => 'token_tree',
          '#token_types' => array(
            token_get_entity_mapping('entity', $entity_type),
            token_get_entity_mapping('entity', 'commerce_line_item'),
            token_get_entity_mapping('entity', 'commerce_customer_profile'),
          ),
          '#dialog' => FALSE,
          // @todo: Maybe enable token browser to not have this load so long.
        );
      }
    }
  }

  /**
   * Profile edit form validate handler.
   *
   * Ensures that payment form added and one of amount or cart field added,
   * also checks that there is at least one full address field.
   */
  public function editFormValidate(&$form, &$form_state, $profile) {
    $amount_valid = FALSE;
    $cart_valid = FALSE;
    $billing_address_valid = FALSE;
    if (!empty($profile['fields'])) {
      // Checking amount field.
      if (!empty($profile['fields']['commerce_product_commerce_product_commerce_items_amount_single'])
        && !empty($profile['fields']['commerce_product_commerce_product_commerce_items_amount_single']['status'])
      ) {
        $amount_valid = TRUE;
      }
      // Checking cart field.
      if (!empty($profile['fields']['commerce_product_commerce_product_commerce_items_cart'])
        && !empty($profile['fields']['commerce_product_commerce_product_commerce_items_cart']['status'])
      ) {
        $cart_valid = TRUE;
      }
      // Checking billing address.
      if (!empty($profile['fields']['commerce_product_commerce_product_commerce_items_customer_billing'])
        && !empty($profile['fields']['commerce_product_commerce_product_commerce_items_customer_billing']['status'])
      ) {
        $billing_address_valid = TRUE;
      }
      foreach ($profile['fields'] as $field_config) {
        if ($field_config['handler'] == 'addressfieldProfileFieldHandler'
          && $field_config['status']
          && empty($field_config['field_item'])
        ) {
          $billing_address_valid = TRUE;
        }
      }
    }

    if ($amount_valid && $cart_valid) {
      form_set_error('', t('You must add only one: amount or cart field to the form.'));
    }
    elseif (!$amount_valid && !$cart_valid) {
      form_set_error('', t('You must add at least one: amount or cart field to the form.'));
    }
    if (!$billing_address_valid) {
      form_set_error('', t('You must add billing address field to the form.'));
    }
  }

  /**
   * Renders property configuration form.
   *
   * Renders commerce items when they are being added to a profile on the edit
   * screen.
   */
  public function renderPropertyConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {

    // Call the parent function.
    $field_config = parent::renderPropertyConfig($form, $entity_type, $entity_label, $bundle, $field_name);
    $field_separator = _crm_core_profile_field_separator($field_config);
    $row =& $form['fields'][$field_separator];

    // Override the row for our properties.
    unset($row['hidden']);
    unset($row['default']);

    $description = '';
    $descriptions = array(
      'commerce_items_amount_single' => t('Includes a field for displaying an amount in the profile, which will be used to process payments. You can configure options for this field on the settings screen. Use token replacement values to store the amount in other fields.'),
      'commerce_items_cart' => t('Includes a shopping cart in the profile form, which can be used to allow users to select quantities of items at different prices. Items in the shopping cart are propulated programmatically.'),
      'commerce_items_payment' => t('Includes a form for processing online payments in the profile. You must include an address field as part of your profile in order for this to work, and can configure the address to use on the settings screen.'),
      'commerce_items_customer_billing' => t('Optional. Includes a billing address field to use for processing payments.'),
    );
    if (isset($descriptions[$field_name])) {
      $description = $descriptions[$field_name];
    }

    $row['description_wrapper'] = array(
      '#type' => 'container',
    );
    $row['description_wrapper']['description'] = array(
      '#markup' => $description,
    );
  }

  /**
   * Entity configuration form builder.
   */
  public function configEntityForm($profile, &$form) {

    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];

    // Check if we have commerce entities.
    if (isset($profile->entities[$entity_type])) {
      $entity_conf = $profile->entities[$entity_type];

      // Add a fieldset for commerce items.
      $form['commerce_items_wrapper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Commerce Items'),
        '#collapsible' => TRUE,
      );
      // Add a fieldset for commerce items.
      $form['commerce_items_wrapper'][$entity_type] = array(
        '#type' => 'container',
        '#tree' => TRUE,
      );

      $form['commerce_items_wrapper'][$entity_type]['description'] = array(
        '#markup' => t('Please select commerce options for this profile using the fields below.'),
      );

      // Allow users to select a product through an autocomplete.
      $default_product_type = empty($entity_conf['product_type']) ? '' : $entity_conf['product_type'];
      $options = commerce_product_type_options_list();
      $form['commerce_items_wrapper'][$entity_type]['product_type'] = array(
        '#type' => 'select',
        '#title' => t('Product type'),
        '#description' => t('Please select a Commerce Product type to be used when processing payments through this profile.'),
        '#default_value' => $default_product_type,
        '#options' => $options,
        '#empty_option' => t('--Please select--'),
        '#empty_value' => '',
        '#required' => TRUE,
      );

      // Order status.
      // Build an array of order status options grouped by order state.
      $options = array();
      foreach (commerce_order_state_get_title() as $name => $title) {
        foreach (commerce_order_statuses(array('state' => $name)) as $order_status) {
          $options[check_plain($title)][$order_status['name']] = check_plain($order_status['title']);
        }
      }
      $default = 'completed';
      if (!empty($entity_conf['order_status'])) {
        $default = $entity_conf['order_status'];
      }
      $form['commerce_items_wrapper'][$entity_type]['order_status'] = array(
        '#type' => 'select',
        '#title' => t('Order status'),
        '#description' => t('Please select the order status to assign to successful transactions.'),
        '#options' => $options,
        '#default_value' => $default,
      );

      // Add settings for each field that is turned on.
      foreach ($profile->fields as $item) {
        if ($item['entity_type'] == $entity_type && !empty($item['status'])) {
          switch ($item['field_name']) {
            case 'commerce_items_amount_single':
              $this->configAmountSingle($profile, $form);
              break;

            case 'commerce_items_cart':
              $this->configCart($profile, $form);
              break;

            case 'commerce_items_payment':
              $this->configPaymentForm($profile, $form);
              break;
          }
        }
      }
    }
  }

  /**
   * Validate handler for configuration form.
   *
   * @see ProfileEntityHandler::configEntityFormValidate()
   */
  public function configEntityFormValidate(&$form, &$form_state) {
    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];
    $profile = $form_state['profile'];

    // Check if we have commerce entities.
    if (isset($profile->entities[$entity_type])) {
      // Checking valid product ID.
      $product_id = isset($form_state['values'][$entity_type]['product']) ? $form_state['values'][$entity_type]['product'] : NULL;
      if (isset($product_id)) {
        $product = commerce_product_load($product_id);
        if (!$product) {
          form_set_error('commerce_product][product', t('There is no product with such ID.'));
        }
      }
      // Checking line items entered.
      if (!empty($profile->entities[$entity_type]['fields_container']['fields']['commerce_items_cart'])
        && !empty($profile->fields['commerce_product_commerce_product_commerce_items_cart']['status'])
      ) {
        $valid = TRUE;
        $line_items = $this->parseLineItems($form_state['values'][$entity_type]['commerce_items_cart_settings']['line_items']);
        foreach ($line_items as $li) {
          if (!is_numeric($li['amount'])) {
            $valid = FALSE;
            break;
          }
        }
        if (!$valid) {
          $message = 'Please check line items entered. Looks like you entered '
            . 'wrong amount value for one of them.';
          form_set_error('commerce_product][commerce_items_cart_settings][line_items', t($message));
        }
      }
    }
    // @todo:
    // there cannot be both an amount field and a cart
    // someone must select name fields to use a checkout form
  }

  /**
   * Configuration options for amount fields.
   *
   * This allows users to configure amount fields to appear as:
   * - static text
   * - a form field
   * - a form field with buttons that populate the value
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param array $form
   *   Structured form array to update.
   */
  public function configAmountSingle($profile, &$form) {

    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_amount_single_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Amount Field'),
      '#collapsible' => TRUE,
    );

    // @todo: better labels
    $states = array(
      0 => t('Static Amount'),
      1 => t('Variable Amount'),
      2 => t('Variable Amount with Buttons'),
    );

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_amount_single_settings']['amount_config'] = array(
      '#type' => 'radios',
      '#title' => t('Display the amount field as:'),
      '#options' => $states,
      '#description' => t('Please select static to create a preset amount that is used each time this profile is displayed. Select variable to allow users to select their own values for this field. Add buttons to allow users to select from a number of suggested amounts.'),
      '#required' => TRUE,
      '#default_value' => isset($profile->entities[$entity_type]['commerce_items_amount_single_settings']['amount_config']) ? $profile->entities[$entity_type]['commerce_items_amount_single_settings']['amount_config'] : '0',
    );

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_amount_single_settings']['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => isset($profile->entities[$entity_type]['commerce_items_amount_single_settings']['label']) ? $profile->entities[$entity_type]['commerce_items_amount_single_settings']['label'] : 'Amount',
      '#description' => t('Enter a label to be displayed with the amount.'),
      '#maxlength' => 255,
    );

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_amount_single_settings']['preset_amounts'] = array(
      '#type' => 'textarea',
      '#title' => t('Amounts'),
      '#cols' => 60,
      '#rows' => 3,
      '#default_value' => isset($profile->entities[$entity_type]['commerce_items_amount_single_settings']['preset_amounts']) ? $profile->entities[$entity_type]['commerce_items_amount_single_settings']['preset_amounts'] : '0',
      '#description' => t('For variable amounts, enter them here. If you are adding buttons to the field, separate recommended values with a comma.'),
    );
  }

  /**
   * Configuration options for a cart.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param array $form
   *   Structured form array to update.
   */
  public function configCart($profile, &$form) {

    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];
    $cart_config = isset($profile->entities[$entity_type]['commerce_items_cart_settings']) ? $profile->entities[$entity_type]['commerce_items_cart_settings'] : array();

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_cart_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cart'),
      '#collapsible' => TRUE,
    );

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_cart_settings']['line_items'] = array(
      '#type' => 'textarea',
      '#title' => t('Line Items'),
      '#cols' => 60,
      '#rows' => 3,
      '#default_value' => isset($cart_config['line_items']) ? $cart_config['line_items'] : '',
      '#description' => t('Add line items to the cart in the format of "LABEL|AMOUNT". One item per line. Custom modules can also add values to carts programatically, which will override the values provided here.'),
    );

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_cart_settings']['quantities'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use quantities in cart?'),
      '#default_value' => isset($cart_config['quantities']) ? $cart_config['quantities'] : FALSE,
      '#description' => t('Check this box to allow users to select quantities for the items in the cart.'),
    );
  }

  /**
   * Configuration options for a payment form.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param array $form
   *   Structured form array to update.
   */
  public function configPaymentForm($profile, &$form) {

    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];

    $form['commerce_items_wrapper'][$entity_type]['commerce_items_payment_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payment Form and Billing Information'),
      '#collapsible' => TRUE,
    );
    $payment_form =& $form['commerce_items_wrapper'][$entity_type]['commerce_items_payment_settings'];
    // Get a list of available payment processors for the site.
    $order = new stdClass();
    rules_invoke_all('commerce_payment_methods', $order);

    // If no payment methods were found, return the empty form.
    if (empty($order->payment_methods)) {
      $payment_form['payment_processors'] = array(
        '#markup' => '<div>' . t('There are no payment processors configured for this site. Please enable one before attempting to use the payment forms with profile.') . '</div>',
      );
    }
    else {
      // Sort the payment processors by weight.
      uasort($order->payment_methods, 'drupal_sort_weight');

      // Create an array of enabled payment processors.
      $payment_processors = array();
      foreach ($order->payment_methods as $instance_id => $method_info) {
        // Ensure we've received a valid payment method.
        $payment_method = commerce_payment_method_load($method_info['method_id']);
        if ($payment_method) {
          $payment_processors[$instance_id] = $payment_method['display_title'];
        }
      }

      $item['all'] = '-- Select All --';
      $payment_processors = $item + $payment_processors;

      // Display the processors.
      $payment_form['payment_processors'] = array(
        '#type' => 'select',
        '#title' => t('Available Payment Processors'),
        '#options' => $payment_processors,
        '#description' => t('Select the payment processors that can be used with this profile.'),
        '#default_value' => empty($profile->entities[$entity_type]['commerce_items_payment_settings']['payment_processors']) ? 'all' : $profile->entities[$entity_type]['commerce_items_payment_settings']['payment_processors'],
        "#multiple" => TRUE,
      );
    }

    // Select name fields for use in processing an order.
    $name_fields = array();

    foreach ($profile->fields as $key => $field) {
      // Show only enabled fields.
      if (empty($field['status'])) {
        continue;
      }
      $field_info = field_info_instance($field['entity_type'], $field['field_name'], $field['bundle']);
      if (empty($field_info['label'])) {
        continue;
      }
      $entity_info = entity_get_info($field['entity_type']);
      $field_item = '';
      if (!empty($field['field_item'])) {
        // @todo Get proper label for subitem here.
        $field_item = ' > ' . $field['field_item'];
      }
      $name_fields[$entity_info['label']][$key] = $field_info['label'] . $field_item;
    }

    if (empty($name_fields)) {

      $payment_form['name_fields'] = array(
        '#markup' => '<div>' . t('There are no name fields associated with your profile. Please add some before attempting to process a payment.') . '</div>',
      );
    }
    else {
      // Select name fields to be used in processing a payment.
      $payment_form['name_fields'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Select Name Fields'),
        '#description' => t('Please select name fields from your profile that will be used to process a payment.'),
        '#options' => $name_fields,
        '#default_value' => empty($profile->entities[$entity_type]['commerce_items_payment_settings']['name_fields']) ? array() : $profile->entities[$entity_type]['commerce_items_payment_settings']['name_fields'],
      );
    }
    // So what we are going to do here allow people to select a primary address
    // used for payment processing if there is a billing address field, it
    // should default to that maybe something that lets people decide on the
    // order in which to process payment forms.
    $options = array();
    $description = t('Select the primary billing address to be used for this profile.');
    if (isset($profile->fields['commerce_product_commerce_product_commerce_items_customer_billing'])
      && !empty($profile->fields['commerce_product_commerce_product_commerce_items_customer_billing']['status'])
    ) {
      $options['commerce_items_customer_billing'] = t('Billing Address');
      $description .= '&nbsp;' . t('The billing address field will always be used first if it has been filled out.');
    }
    foreach ($profile->fields as $key => $field) {
      // Show only whole address handler fields.
      if ($field['handler'] == 'addressfieldProfileFieldHandler' && empty($field['field_item']) && !empty($field['status'])) {
        $field_info = field_info_instance($field['entity_type'], $field['field_name'], $field['bundle']);
        $entity_info = entity_get_info($field['entity_type']);
        $options[$entity_info['label']][$key] = $field_info['label'];
      }
    }
    $payment_form['commerce_billing_address_field'] = array(
      '#type' => 'select',
      '#title' => t('Select Address Field'),
      '#options' => $options,
      '#description' => $description,
      '#default_value' => isset($profile->entities[$entity_type]['commerce_items_payment_settings']['commerce_billing_address_field']) ? $profile->entities[$entity_type]['commerce_items_payment_settings']['commerce_billing_address_field'] : 'commerce_items_customer_billing',
    );
  }

  /**
   * Used to validate entity submissions from CRM Core Profile.
   */
  public function validateEntity($profile, $form, &$form_state) {
  }

  /**
   * Used to handle submissions from the given entity type.
   */
  public function submitEntity($profile, $form, &$form_state) {

    global $user;

    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];
    $entity_conf = $profile->entities[$entity_type];
    // Getting preconfigured product type.
    $product_type = commerce_product_type_load($entity_conf['product_type']);
    if ($product_type) {
      // Creating order.
      $order = commerce_cart_order_new($user->uid);
      commerce_order_save($order);
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      // Check if we have a cart with items.
      if (isset($form_state['cart_line_items'])) {
        $cart_line_items = $form_state['cart_line_items'];
        $use_quantities = isset($entity_conf['commerce_items_cart_settings']) ? $entity_conf['commerce_items_cart_settings']['quantities'] : 0;
        if ($use_quantities) {
          // We have to update them with submitted data.
          $values = $form_state['values']['cart'];
          foreach ($cart_line_items as $id => $item) {
            $item['quantity'] = $values[$id]['quantity'];
            if (!empty($item['quantity'])) {
              $product = commerce_product_load_by_sku($id);
              if (!$product) {
                $product = $this->initProduct($item, $product_type['type']);
              }
              $line_item = commerce_product_line_item_new($product, $item['quantity'], $order->order_id);
              $this->initLineItemPrice($line_item, $item['amount']);
              $line_item->line_item_label = check_plain($item['title']);
              // Save the line item to get it ID.
              commerce_line_item_save($line_item);
              // Add the item to the list of line items.
              $order_wrapper->commerce_line_items[] = $line_item;
            }
          }
        }
        else {
          $item_id = $form_state['values']['cart'];
          $item = $form_state['cart_line_items'][$item_id];
          $product = commerce_product_load_by_sku($item_id);
          if (!$product) {
            $product = $this->initProduct($item, $product_type['type']);
          }
          $line_item = commerce_product_line_item_new($product, $item['quantity'], $order->order_id);
          $this->initLineItemPrice($line_item, $item['amount']);
          $line_item->line_item_label = check_plain($item['title']);
          // Save the line item to get it ID.
          commerce_line_item_save($line_item);
          // Add the item to the list of line items.
          $order_wrapper->commerce_line_items[] = $line_item;
        }
        $order_wrapper->save();
      }
      // Lets check single amount field.
      if (isset($form_state['values']['commerce_amount_single'])) {
        $item_title = t('Amount');
        if (isset($form_state['crm_core_profile_data']['commerce_items'])
          && isset($form_state['crm_core_profile_data']['commerce_items']['commerce_items_amount_single_settings'])
          && !empty($form_state['crm_core_profile_data']['commerce_items']['commerce_items_amount_single_settings']['amount'])) {
          $item_title = $form_state['crm_core_profile_data']['commerce_items']['commerce_items_amount_single_settings']['amount'];
        }
        elseif (!empty($entity_conf['commerce_items_amount_single_settings']['label'])) {
          $item_title = $entity_conf['commerce_items_amount_single_settings']['label'];
        }
        $item = array(
          'title' => $item_title,
          'amount' => $form_state['values']['commerce_amount_single'],
        );
        $product = $this->initProduct($item, $product_type['type']);
        $line_item = commerce_product_line_item_new($product, 1, $order->order_id);
        $this->initLineItemPrice($line_item, $form_state['values']['commerce_amount_single']);
        $title = t('@product_title with @amount amount', array(
            '@product_title' => $product->title,
            '@amount' => _crm_core_profile_commerce_items_currency_format($form_state['values']['commerce_amount_single']),
          ));
        $line_item->line_item_label = $title;
        // Save the line item to get it ID.
        commerce_line_item_save($line_item);
        // Add the item to the list of line items.
        $order_wrapper->commerce_line_items[] = $line_item;
        $order_wrapper->save();
      }
      // Assuming that at this stage all line items created and added to order.
      if (!empty($entity_conf['commerce_items_payment_settings'])) {
        // Creating customer profile.
        $customer_profile = commerce_customer_profile_new('billing', $user->uid);
        $customer_profile->commerce_customer_address[LANGUAGE_NONE][0] = array();
        $billing_address =& $customer_profile->commerce_customer_address[LANGUAGE_NONE][0];
        $billing_field = $entity_conf['commerce_items_payment_settings']['commerce_billing_address_field'];
        // Check if our custom address field should be used.
        if ($billing_field == 'commerce_items_customer_billing' || !empty($form_state['values']['use_commerce_customer_address'])) {
          // @see $this::addBillingAddress.
          $billing_field = 'commerce_customer_address';
        }
        else {
          $billing_field = $profile->fields[$billing_field]['field_name'];
        }
        $billing_address = array_merge($billing_address, $form_state['values'][$billing_field][LANGUAGE_NONE][0]);
        // Ugly hack to avoid "Undefined index" PHP notices.
        // See https://drupal.org/node/1512130 for details.
        // @todo: Remove this when commerce_authnet.module would respect
        // commerce_customer_billing field settings.
        if (!isset($billing_address['organisation_name'])) {
          $billing_address['organisation_name'] = '';
        }

        // Filing in name data.
        $billing_name = array();
        foreach ($entity_conf['commerce_items_payment_settings']['name_fields'] as $field_name) {
          $field_config = $profile->fields[$field_name];
          // Do we dealing with full field?
          if (empty($field_config['field_item'])) {
            $field_name = $field_config['field_name'];
            if (isset($form_state['values'][$field_name][LANGUAGE_NONE])) {
              foreach ($form_state['values'][$field_name][LANGUAGE_NONE][0] as $field_item) {
                $billing_name[] = $field_item;
              }
            }
            else {
              if (is_array($form_state['values'][$field_name])) {
                foreach ($form_state['values'][$field_name] as $field_item) {
                  $billing_name[] = $field_item;
                }
              }
              else {
                $billing_name[] = $form_state['values'][$field_name];
              }
            }
          }
          // This is part of composite field.
          else {
            $billing_name[] = $form_state['values'][$field_name];
          }
        }
        $billing_address['name_line'] = implode(' ', $billing_name);
        commerce_customer_profile_save($customer_profile);
        $order_wrapper->commerce_customer_billing = $customer_profile;
        $order_wrapper->save();
      }
      commerce_order_calculate_total($order);
      $order_total = $order_wrapper->commerce_order_total->amount->value();
      // We don't need to process 0 amount payments.
      if (!empty($order_total)) {
        // Saving order in $form_state to use in token replacement.
        $form_state[$entity_type] = $order;
        if (empty($profile->fields['commerce_product_commerce_product_commerce_items_payment'])
          || empty($profile->fields['commerce_product_commerce_product_commerce_items_payment']['status'])) {
          // Payment form not added or disabled - redirecting to checkout page.
          $form_state['redirect'] = 'checkout/' . $order->order_id;
          return;
        }
        // Now we can process the payment at last.
        $payment_method = commerce_payment_method_instance_load($form_state['values']['payment_method']);
        // Delegate submit to the payment method callback.
        if ($callback = commerce_payment_method_callback($payment_method, 'submit_form_submit')) {
          $charge = array(
            'amount' => $order_wrapper->commerce_order_total->amount->value(),
            'currency_code' => commerce_default_currency(),
          );
          // Payment method form.
          $details_form = !empty($form['commerce_payment_form']['payment_details']) ? $form['commerce_payment_form']['payment_details'] : array();
          // Payment method values.
          $details_values = !empty($form_state['values']['payment_details']) ? $form_state['values']['payment_details'] : $form_state['input']['payment_details'];

          $result = $callback($payment_method, $details_form, $details_values, $order, $charge);

          if ($result === FALSE) {
            drupal_set_message(t('Your payment was not received. Please try again, or contact us.'), 'error');
          }
          else {
            commerce_order_status_update($order, $entity_conf['order_status'], FALSE, TRUE, t('Payment accepted successful.'));
          }
        }
      }
      else {
        drupal_set_message(t("Your order total is zero. If you think that this shouldn't happen, please contact us."));
      }
    }
    else {
      $message = "Can't load product type with ID: @product_id, that stored in"
        . " profile '@profile_name' configuration. !settings.";
      $settings_uri = 'admin/structure/crm-core/profile/manage/' . $profile->name . '/settings';
      watchdog('crm_core_profile_commerce_items', $message, array(
          '@product_id' => $product_type['type'],
          '@profile_name' => $profile->name,
          '!settings' => l(t('Profile settings'), $settings_uri),
        ), WATCHDOG_ERROR);
      drupal_set_message(t('Payment transaction was not created. Please contact us.'), 'error');
    }
  }

  /**
   * Adds commerce items to a profile.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param string $entity_type
   *   Entity type.
   * @param object $preload
   *   Preloaded user object.
   * @param array $form
   *   Structured form array to update.
   * @param array $form_state
   *   Form state.
   */
  public function execute($profile, $entity_type, $preload, &$form, &$form_state) {

    // Get the configuration for the profile.
    $entity_config = $profile->entities[$entity_type];

    // Check to see that commerce items are enabled.
    if ($entity_config['status'] === 1) {

      // Create something on the form that the after build handler can
      // recognize.
      $form_state[$entity_type] = commerce_order_new($GLOBALS['user']->uid);

      // Get the fields that are supposed to appear on the profile.
      $fields = $profile->fields;

      // Add an amount field.
      if (isset($fields['commerce_product_commerce_product_commerce_items_amount_single']['status']) && $fields['commerce_product_commerce_product_commerce_items_amount_single']['status'] === 1) {
        $this->addAmountField($profile, $entity_type, $form, $form_state);
      }

      // Add a cart.
      if (isset($fields['commerce_product_commerce_product_commerce_items_cart']['status']) && $fields['commerce_product_commerce_product_commerce_items_cart']['status'] === 1) {
        $this->addCart($profile, $entity_type, $form, $form_state);
      }

      // Add a billing address.
      if (isset($fields['commerce_product_commerce_product_commerce_items_customer_billing']['status']) && $fields['commerce_product_commerce_product_commerce_items_customer_billing']['status'] === 1) {
        $this->addBillingAddress($profile, $entity_type, $form, $form_state);
      }

      // Add a payment form.
      if (isset($fields['commerce_product_commerce_product_commerce_items_payment']['status']) && $fields['commerce_product_commerce_product_commerce_items_payment']['status'] === 1) {
        $this->addPaymentForm($profile, $entity_type, $form, $form_state);
      }
    }
  }

  /**
   * This will render a single amount field within a CRM Core Profile form.
   *
   * It operates several ways:
   *
   * - The amount field can be static, which will render some text and pass the
   * actual value to the form through other means.
   *
   * - The amount field can be variable, which will render a textfield users can
   * use to enter their own values.
   *
   * - The amount field can be variable with buttons, which will render a
   * textfield plus buttons that will populate the text field with recommended
   * amounts.
   *
   * The amount can be overridden through form_alters in the following manner:
   *
   * - $form_state['crm_core_profile_data']['commerce_items']['commerce_items_amount_single_settings']['amount'] = 0
   *
   * Prepopulating values should be done through hook_node_load().
   * See crm_core_profile_node_view for examples of how data is passed in
   * through other modules.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param string $entity_type
   *   Entity type.
   * @param array $form
   *   Structured form array to update.
   * @param array $form_state
   *   Form state.
   */
  public function addAmountField($profile, $entity_type, &$form, &$form_state) {

    $entity_config = $profile->entities[$entity_type];
    if (isset($entity_config['commerce_items_amount_single_settings'])) {
      $amount_config = $entity_config['commerce_items_amount_single_settings'];
    }
    else {
      $amount_config = array(
        'amount_config' => CCPCI_AMOUNT_STATIC,
        'preset_amounts' => 0,
        'label' => t('Default amount label'),
      );
    }

    $field_config = $profile->fields['commerce_product_commerce_product_commerce_items_amount_single'];

    // If we are allowing users to set their own values, offer a default
    // value (if there is one).
    $amounts = crm_core_profile_commerce_items_parse_amounts($amount_config['preset_amounts']);

    // Make sure there is a numeric value for the first amount.
    if (empty($amounts) || !is_numeric($amounts[0])) {
      $amounts[0] = 0;
    }
    // The default amount can be overridden programmatically
    // through form_state settings.
    if (isset($form_state['crm_core_profile_data']['commerce_items_amount_single'])
      && !empty($form_state['crm_core_profile_data']['commerce_items_amount_single']['amount'])) {
      $amounts[0] = $form_state['crm_core_profile_data']['commerce_items_amount_single']['amount'];
      // Allow to override title, if set.
      if (!empty($form_state['crm_core_profile_data']['commerce_items_amount_single']['title'])) {
        $amount_config['label'] = $form_state['crm_core_profile_data']['commerce_items_amount_single']['title'];
      }
    }

    // Getting default currency.
    $currency = commerce_currency_load(commerce_default_currency());
    $amount_element = array(
      '#type' => 'textfield',
      '#title' => check_plain($amount_config['label']),
      '#weight' => $field_config['weight'],
      '#field_prefix' => $currency['symbol'],
      '#size' => 10,
      '#default_value' => $amounts[0],
      '#element_validate' => array('crm_core_profile_commerce_items_commerce_amount_single_validate_field'),
    );
    switch ($amount_config['amount_config']) {

      case CCPCI_AMOUNT_STATIC:
        $form['commerce_amount_single_display'] = array(
          '#weight' => $field_config['weight'],
          '#type' => 'item',
          '#title' => check_plain($amount_config['label']),
          '#markup' => _crm_core_profile_commerce_items_currency_format($amounts[0]),
        );
        // Ensures that specified amount will be submitted.
        $form['commerce_amount_single'] = array(
          '#type' => 'value',
          '#value' => $amounts[0],
        );
        break;

      case CCPCI_AMOUNT_VARIABLE:
        $form['commerce_amount_single'] = $amount_element;
        break;

      case CCPCI_AMOUNT_VARIABLE_BUTTONS:
        $form['commerce_amount_single'] = $amount_element;
        $form['commerce_amount_single']['#prefix'] = theme('crm_core_profile_commerce_items_amount_buttons', array('amounts' => $amounts));
        $form['commerce_amount_single']['#attached'] = array(
          'js' => array(drupal_get_path('module', 'crm_core_profile_commerce_items') . '/js/amount_buttons.js'),
        );
        break;
    }
  }

  /**
   * Adds a shopping cart to a profile.
   *
   * The contents can be populated in the following manner:
   *
   * - $form_state['crm_core_profile_data']['commerce_items']['commerce_items_cart']['items'] = array()
   *
   * ... where array is a list of associative arrays:
   *
   * array('title'  => 'Platinum','amount' => 10000)
   *
   * Prepopulating values should be done through hook_node_load().
   * See crm_core_profile_node_view for examples of how data is passed in
   * through other modules.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param string $entity_type
   *   Entity type.
   * @param array $form
   *   Structured form array to update.
   * @param array $form_state
   *   Form state.
   */
  public function addCart($profile, $entity_type, &$form, &$form_state) {

    $entity_config = $profile->entities[$entity_type];

    $field_config = $profile->fields['commerce_product_commerce_product_commerce_items_cart'];
    $entity_properties = $this->entityProperties();
    if (isset($form_state['crm_core_profile_data']) &&
      !empty($form_state['crm_core_profile_data']['commerce_items_cart_items'])
    ) {
      $line_items = $form_state['crm_core_profile_data']['commerce_items_cart_items'];
      // Lets validate incoming data.
      foreach ($line_items as $id => &$li) {
        // Ignoring items with not numeric amounts or empty labels.
        $title = trim($li['title']);
        if (is_numeric($li['amount']) && !empty($title)) {
        }
        else {
          unset($line_items[$id]);
        }
      }
    }
    else {
      $line_items = array();
      if (!empty($entity_config['commerce_items_cart_settings'])
        && !empty($entity_config['commerce_items_cart_settings']['line_items'])) {
        $line_items = $this->parseLineItems($entity_config['commerce_items_cart_settings']['line_items']);
      }
    }

    if (!empty($line_items)) {
      $use_quantities = $entity_config['commerce_items_cart_settings']['quantities'];
      if ($use_quantities) {
        $form_state['cart_line_items'] = $line_items;

        $form['cart_wrapper'] = array(
          '#type' => 'container',
          '#attributes' => array(),
          '#weight' => $field_config['weight'],
        );
        $form['cart_wrapper']['cart'] = array(
          '#theme' => 'crm_core_profile_commerce_cart',
          '#title' => check_plain($entity_properties['commerce_items_cart']),
          '#weight' => $field_config['weight'] + 0.1,
          '#tree' => TRUE,
          '#prefix' => '<div id="crm-core-profile-commerce-cart-ajax">',
          '#suffix' => '</div>',
        );

        // Header form cart table.
        $form['cart_wrapper']['cart']['header'] = array(
          'title' => array(
            '#markup' => t('Title'),
          ),
          'amount' => array(
            '#markup' => t('Price'),
          ),
          'quantity' => array(
            '#markup' => t('Quantity'),
          ),
          'total' => array(
            '#markup' => t('Line total'),
          ),
        );

        if (isset($form_state['values']) && !empty($form_state['values']['update_cart'])) {
          $values = $form_state['input']['cart'];
        }
        $cart_total = 0;
        foreach ($line_items as $id => &$line_item) {

          $form['cart_wrapper']['cart'][$id]['title'] = array(
            '#type' => 'item',
            '#markup' => check_plain($line_item['title']),
          );

          $form['cart_wrapper']['cart'][$id]['amount'] = array(
            '#type' => 'item',
            '#markup' => _crm_core_profile_commerce_items_currency_format($line_item['amount']),
          );

          $line_item['quantity'] = isset($values[$id]['quantity']) ? $values[$id]['quantity'] : 0;
          $form['cart_wrapper']['cart'][$id]['quantity'] = array(
            '#type' => 'textfield',
            '#size' => 5,
            '#default_value' => $line_item['quantity'],
          );

          $line_item['total'] = $line_item['amount'] * $line_item['quantity'];
          $form['cart_wrapper']['cart'][$id]['total'] = array(
            '#type' => 'item',
            '#markup' => _crm_core_profile_commerce_items_currency_format($line_item['total']),
          );

          $cart_total += $line_item['total'];
        }

        $vars = array(
          'total_label' => t('Total:'),
          'total' => _crm_core_profile_commerce_items_currency_format($cart_total),
          'total_raw' => $cart_total,
        );
        $form['cart_wrapper']['cart']['summary']['#markup'] = theme('crm_core_profile_commerce_cart_summary', $vars);

        $form['cart_wrapper']['actions'] = array(
          '#type' => 'container',
          '#attributes' => array(),
          '#weight' => $field_config['weight'] + 0.2,
        );
        $form['cart_wrapper']['actions']['update_cart'] = array(
          '#type' => 'button',
          '#value' => t('Update'),
          '#ajax' => array(
            'callback' => 'crm_core_profile_commerce_items_cart_refresh',
            'wrapper' => 'crm-core-profile-commerce-cart-ajax',
          ),
          // We don't want to validate form when this button clicked.
          '#limit_validation_errors' => array(),
        );

        $form_state['cart_line_items'] = $line_items;
      }
      else {
        $form['cart_wrapper'] = array(
          '#type' => 'container',
          '#attributes' => array(),
          '#weight' => $field_config['weight'],
        );

        // Header form cart table.
        $header = array(
          'title' => t('Title'),
          'amount' => t('Price'),
        );
        $options = array();
        foreach ($line_items as $id => &$line_item) {
          $options[$id] = array(
            'title' => check_plain($line_item['title']),
            'amount' => _crm_core_profile_commerce_items_currency_format($line_item['amount']),
          );
          $line_item['quantity'] = 1;
          $line_item['total'] = $line_item['amount'];
        }

        $form['cart_wrapper']['cart'] = array(
          '#type' => 'tableselect',
          '#header' => $header,
          '#options' => $options,
          '#multiple' => FALSE,
          '#prefix' => '<div class="caption">' . check_plain($entity_properties['commerce_items_cart']) . '</div>',
        );

        $form_state['cart_line_items'] = $line_items;
      }
    }
  }

  /**
   * Adds a billing address to a profile.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param string $entity_type
   *   Entity type.
   * @param array $form
   *   Structured form array to update.
   * @param array $form_state
   *   Form state.
   */
  public function addBillingAddress($profile, $entity_type, &$form, &$form_state) {

    global $user;
    $checkbox = FALSE;
    $field_config = $profile->fields['commerce_product_commerce_product_commerce_items_customer_billing'];
    $weight = $field_config['weight'];

    // Collect address fields attached to profile.
    $addr_fields = array();
    foreach ($profile->fields as $field) {
      if ($field['handler'] == 'addressfieldProfileFieldHandler' && !empty($field['status'])) {
        $addr_fields[$field['field_name']] = $field['field_name'];
      }
    }
    $billing_addr_field = 'commerce_items_customer_billing';
    if (!empty($addr_fields) && $profile->entities[$entity_type]['commerce_items_payment_settings']['commerce_billing_address_field'] != $billing_addr_field) {
      $form['use_commerce_customer_address'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use separate billing address?'),
        '#default_value' => 0,
        '#weight' => $weight,
      );
      $weight += 0.1;
      $checkbox = TRUE;
    }

    // Ensure this include file is loaded when the form is rebuilt from
    // the cache.
    $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_customer') . '/includes/commerce_customer_profile.forms.inc';
    $billing = commerce_customer_profile_new('billing', $user->uid);
    $form_state['billing_address'] = $billing;
    field_attach_form('commerce_customer_profile', $billing, $form, $form_state);
    $form['commerce_customer_address'][LANGUAGE_NONE][0]['name_block']['#access'] = FALSE;
    $form['commerce_customer_address']['#weight'] = $weight;

    // We are using another field as billing address.
    if ($checkbox) {
      $form['commerce_customer_address']['#states'] = array(
        'visible' => array(
          ':input[name="use_commerce_customer_address"]' => array('checked' => TRUE),
        ),
      );
      unset($form['commerce_customer_address']['und'][0]['street_block']['thoroughfare']['#required']);
      unset($form['commerce_customer_address']['und'][0]['locality_block']['locality']['#required']);
      unset($form['commerce_customer_address']['und'][0]['locality_block']['administrative_area']['#required']);
      unset($form['commerce_customer_address']['und'][0]['locality_block']['postal_code']['#required']);
      unset($form['commerce_customer_address']['und'][0]['country']['#required']);
    }
  }

  /**
   * Add a payment form to a profile.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param string $entity_type
   *   Entity type.
   * @param array $form
   *   Structured form array to update.
   * @param array $form_state
   *   Form state.
   */
  public function addPaymentForm($profile, $entity_type, &$form, &$form_state) {
    $entity_config = $profile->entities[$entity_type];
    $field_config = $profile->fields['commerce_product_commerce_product_commerce_items_payment'];
    $entity_properties = $this->entityProperties();

    if (isset($entity_config['commerce_items_payment_settings'])) {
      $payment_processors = $entity_config['commerce_items_payment_settings']['payment_processors'];
      $payment_methods = _crm_core_profile_commerce_items_enabled_payment_methods();
      $options = array();
      foreach ($payment_processors as $instance_id) {
        if ($instance_id == 'all') {
          foreach ($payment_methods as $instance_id => $instance) {
            $options[$instance_id] = $instance['display_title'];
          }
          break;
        }
        if ($payment_processor = commerce_payment_method_instance_load($instance_id)) {
          $options[$instance_id] = $payment_processor['display_title'];
        }
      }
      // If no payment methods were found, return the empty form.
      if (empty($options)) {
        $pane_form['message'] = array(
          '#markup' => '<div>' . t('Unfortunately we could not find any suitable payment methods, and we require a payment method to complete checkout.') . '<br /><strong>' . t('Please contact us to resolve any issues with your order.') . '</strong></div>',
        );
      }
      else {
        // If there is only one payment option attach it as value to form.
        if (count($options) == 1) {
          $default_method = key($options);
          $pane_form['payment_method'] = array(
            '#type' => 'value',
            '#value' => $default_method,
          );
        }
        else {
          // Add a radio select widget to specify the payment method.
          $pane_form['payment_method'] = array(
            '#type' => 'radios',
            '#options' => $options,
            '#ajax' => array(
              'callback' => 'crm_core_profile_commerce_items_payment_form_refresh',
              'wrapper' => 'payment-details',
            ),
            '#limit_validation_errors' => array(),
          );

          if (isset($form_state['values'])) {
            $default_method = $form_state['values']['payment_method'];
          }
          else {
            reset($options);
            $default_method = key($options);
          }
        }

        // Set the default value for the payment method radios.
        $pane_form['payment_method']['#default_value'] = $default_method;

        $payment_method = $payment_methods[$default_method];

        $checkout_pane = array();
        $order = new stdClass();
        $pane_values = array();
        if ($callback = commerce_payment_method_callback($payment_method, 'submit_form')) {
          $pane_form['payment_details'] = $callback($payment_method, $pane_values, $checkout_pane, $order);
        }
        else {
          $pane_form['payment_details'] = array();
        }

        $pane_form['payment_details']['#tree'] = TRUE;
        $pane_form['payment_details']['#prefix'] = '<div id="payment-details">';
        $pane_form['payment_details']['#suffix'] = '</div>';
      }

      $form['commerce_payment_form'] = array(
        '#type' => 'fieldset',
        '#title' => $entity_properties['commerce_items_payment'],
        '#weight' => $field_config['weight'],
      );

      $form['commerce_payment_form'] = array_merge($form['commerce_payment_form'], $pane_form);
    }
  }

  /**
   * Helper to parse line items from saved string.
   *
   * @param string $line_items
   *   String of stored default line items.
   *
   * @return array
   *   Array of line items.
   */
  protected function parseLineItems($line_items) {

    $dli = crm_core_profile_commerce_items_parse_amounts($line_items);
    $line_items = array();
    foreach ($dli as $line_item) {
      list($label, $amount) = explode('|', $line_item);
      $line_items[] = array(
        'title' => check_plain(trim($label)),
        'amount' => trim($amount),
      );
    }

    return $line_items;
  }

  /**
   * Helper to init line item price.
   *
   * @param object $line_item
   *   Line item.
   * @param float $decimal_amount
   *   Price in decimal.
   * @param bool $product
   *   Indicates if line item is product.
   */
  protected function initLineItemPrice(&$line_item, $decimal_amount, $product = FALSE) {
    $currency_code = commerce_default_currency();
    $price = array(
      'amount' => commerce_currency_decimal_to_amount($decimal_amount, $currency_code),
      'currency_code' => $currency_code,
      'data' => array('components' => array()),
    );
    $data = commerce_price_component_add($price, 'base_price', $price, TRUE);
    if ($product) {
      $line_item->commerce_price[LANGUAGE_NONE][0] = $price;
      $line_item->commerce_price[LANGUAGE_NONE][0]['data'] = $data;
    }
    else {
      $line_item->commerce_unit_price[LANGUAGE_NONE][0] = $price;
      $line_item->commerce_unit_price[LANGUAGE_NONE][0]['data'] = $data;
    }
  }

  /**
   * Initialize product.
   *
   * @param array $item
   *   Item that must be created as product. Must contain 'amount' and 'title'
   * entries.
   * @param string $product_type
   *   Product type machine name.
   *
   * @return object
   *   Product object.
   */
  protected function initProduct($item, $product_type) {
    $product_human_name = $item['title'] . ' ' . $item['amount'];
    $product_sku = $this->human2Machine($product_human_name);
    $product = commerce_product_load_by_sku($product_sku);
    if (!$product) {
      $product = commerce_product_new($product_type);
      $product->sku = $product_sku;
      $product_type_array = commerce_product_type_load($product_type);
      $product->title = $product_type_array['name'];
    }
    $this->initLineItemPrice($product, $item['amount'], TRUE);
    commerce_product_save($product);

    return $product;
  }

  /**
   * Transliterate human readable name to machine.
   *
   * @param string $name
   *   Human readable name.
   *
   * @return string
   *   Machine readable name.
   */
  protected function human2Machine($name) {
    $replace_pattern = '/[^a-z0-9_]+/';
    $replace = '_';

    return drupal_substr(preg_replace($replace_pattern, $replace, strtolower($name)), 0, 32);
  }
}
