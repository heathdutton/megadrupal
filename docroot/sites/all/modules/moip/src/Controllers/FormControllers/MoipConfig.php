<?php

namespace Drupal\moip\Controllers\FormControllers;

class MoipConfig extends \Drupal\cool\BaseSettingsForm {

  static public function getId() {
    return 'moip_settings_form';
  }

  static public function build() {
    $form = parent::build();

    $form['basic_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Moip integration basic settings')
    );
    $form['basic_settings']['moip_server'] = array(
      '#type' => 'radios',
      '#title' => t('MoIP server'),
      '#options' => array(
        1 => t('Sandbox - use for testing, requires a MoIP Sandbox account'),
        0 => t('Production - use for processing real transactions'),
      ),
      '#default_value' => variable_get('moip_server', ''),
      '#required' => TRUE,
    );
    $form['basic_settings']['moip_token'] = array(
      '#type' => 'textfield',
      '#title' => t('MoIP token'),
      '#default_value' => variable_get('moip_token', ''),
      '#required' => TRUE,
    );
    $form['basic_settings']['moip_key'] = array(
      '#type' => 'textfield',
      '#title' => t('MoIP key'),
      '#default_value' => variable_get('moip_key', ''),
      '#required' => TRUE,
    );
    $form['basic_settings']['moip_display_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Display title'),
      '#description' => t('Text used on the checkout page description of the payment method.'),
      '#default_value' => variable_get('moip_display_title', 'MoIP'),
    );
    $form['basic_settings']['moip_detail_messages'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to specify each line item on the order info at MoIP?'),
      '#description' => t('If not, the info sent to MoIP will only contain the reason(title)'),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#default_value' => variable_get('moip_detail_messages', 1),
      '#required' => TRUE,
    );
    $form['basic_settings']['moip_debug'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to debug every action of this module?'),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#default_value' => variable_get('moip_debug', FALSE),
      '#required' => TRUE,
    );
    $form['advanced_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced settings')
    );
    $classes_available = \Drupal\cool\Loader::mapSubclassesAvailable('Moip', 'Drupal\moip\MoipDrupal', TRUE);
    $classes_options = array();
    foreach ($classes_available as $class_name) {
      $classes_options[$class_name] = $class_name;
    }
    $form['advanced_settings']['moip_class'] = array(
      '#type' => 'select',
      '#title' => t('MoIP Class'),
      '#options' => $classes_options,
      '#description' => t('MoIP PHP Class to use in this site. It is useful if you have some special use case to deal with.'),
      '#default_value' => variable_get('moip_class'),
    );
    if (module_exists('token')) {
      $form['advanced_settings']['display'] = array(
        '#type' => 'fieldset',
        '#title' => t('Order display'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE
      );
      $form['advanced_settings']['display']['moip_order_reason_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Order Reason Token'),
        '#description' => t('What title do you want to use for the orders sent to MoIP. The default ist "Order @order_number at @store". <strong>You can use tokens</strong> here(see the tokens browser above).'),
        '#default_value' => variable_get('moip_order_reason_token'),
      );
      $form['advanced_settings']['display']['moip_phone_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone Token'),
        '#description' => t('From where do you want to get the "Phone" field data to send to MoIP. <strong>You can use tokens</strong> here(see the tokens browser above).'),
        '#default_value' => variable_get('moip_phone_token'),
      );
      $form['advanced_settings']['display']['moip_name_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Name Token'),
        '#description' => t('From where do you want to get the "Name" field data to send to MoIP. <strong>You can use tokens</strong> here(see the tokens browser above).'),
        '#default_value' => variable_get('moip_name_token'),
      );
      $form['advanced_settings']['display']['moip_birthday_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Birthday Token'),
        '#description' => t('From where do you want to get the "Birthday" field data to send to MoIP. <strong>You can use tokens</strong> here(see the tokens browser above).'),
        '#default_value' => variable_get('moip_birthday_token'),
      );
      $form['advanced_settings']['display']['moip_cpf_token'] = array(
        '#type' => 'textfield',
        '#title' => t('CPF Token'),
        '#description' => t('From where do you want to get the "CPF" field data to send to MoIP. <strong>You can use tokens</strong> here(see the tokens browser above).'),
        '#default_value' => variable_get('moip_cpf_token'),
      );
      $form['advanced_settings']['display']['tokens'] = array(
        '#theme' => 'token_tree',
        '#token_types' => array('commerce-order'),
      );
    }
    else {
      $form['advanced_settings']['display'] = array(
        '#type' => 'fieldset',
        '#title' => t('Customize with @tokens'),
        '#markup' => t('Customize with @tokens'),
      );
      $form['advanced_settings']['display']['help'] = array(
        '#markup' => t('You will be able to customize some texts if you enable the <a href="http://drupal.org/project/token">Token module</a>.'),
      );
    }
    $form['advanced_settings']['comission'] = array(
      '#type' => 'fieldset',
      '#title' => t('Comission settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['advanced_settings']['comission']['moip_comission_reason_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Reason'),
      '#description' => t('Specify the reason for this comission.'),
      '#default_value' => variable_get('moip_comission_reason_1'),
    );
    $form['advanced_settings']['comission']['moip_comission_user_receiver_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Moip User'),
      '#description' => t('Moip account(email) to receive the comission.'),
      '#default_value' => variable_get('moip_comission_user_receiver_1'),
    );
    $form['advanced_settings']['comission']['moip_comission_type_1'] = array(
      '#type' => 'radios',
      '#title' => t('Comission type'),
      '#description' => t('You need to specify if this comission is a fixed amount or a percentual value from order total.'),
      '#options' => array(
        0 => t('Fixed amount'),
        1 => t('Percentual value'),
      ),
      '#default_value' => variable_get('moip_comission_type_1'),
    );
    $form['advanced_settings']['comission']['moip_comission_value_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#description' => t('Format: "10.0" (for both fixed amount and percentual value)'),
      '#default_value' => variable_get('moip_comission_value_1'),
    );
    return $form;
  }

  static public function validate($form, &$form_state) {

    // If something was informed about comissions, validate it
    if (!empty($form_state['values']['moip_comission_reason_1']) || !empty($form_state['values']['moip_comission_user_receiver_1'])) {

      if (empty($form_state['values']['moip_comission_reason_1']) || empty($form_state['values']['moip_comission_user_receiver_1']) || !isset($form_state['values']['moip_comission_type_1']) || empty($form_state['values']['moip_comission_value_1'])
      ) {
        form_set_error('advanced_settings', t('The comissions seem to be incorrectly configured. Please review.'));
      }
    }
  }

}
