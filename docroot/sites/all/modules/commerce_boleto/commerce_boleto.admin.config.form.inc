<?php
/**
 * @file
 * Commerce Boleto Admin Settings.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Updates the widget form on each Ajax callback.
 */
function commerce_boleto_form_rules_ui_edit_element_alter(&$form, &$form_state, $form_id) {
  if (isset($form_state['element_settings']['payment_method']) &&
      $form_state['element_settings']['payment_method'] == 'boleto') {
    if (isset($form_state['values'])) {
      $settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
      $bank_element = $form_state['triggering_element']['#parents'][5];

      $show_hide = 'hidden';
      $required_field = FALSE;

      if ($settings[$bank_element]['enabled']) {
        $show_hide = 'markup';
        $required_field = TRUE;
      }
      else {
        // The user disabled the bank plugin.
        $bank_code = $settings[$bank_element]['bank_code'];
        // Reset the form values.
        $settings = _commerce_boleto_default_settings($bank_code);
        $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'][$bank_element] = $settings[$bank_element];
      }

      $form_temp = $form['parameter']['payment_method']['settings']['payment_method']['settings'][$bank_element]['settings'];

      $form_temp['#type'] = $show_hide;

      foreach ($form_temp as $field_name => $element) {
        if ((is_array($element)) && (isset($element['#required']))) {
          $form_temp[$field_name]['#required'] = $required_field;
        }
      }

      $form['parameter']['payment_method']['settings']['payment_method']['settings'][$bank_element]['settings'] = $form_temp;

    }
  }
}

/**
 * Construct / Reset the default values for a bank settings.
 *
 * @param String $bank_code
 *   The bank code.
 * @param Array $settings
 *   The existing array of settings.
 *
 * @return Array
 *   The constructed or reset settings array.
 */
function _commerce_boleto_default_settings($bank_code, $settings = array()) {
  $settings += array(
    "bank_$bank_code" => array(
      'enabled' => 0,
      'bank_code' => '',
      'bank_name' => '',
      'settings' => array(
        'agencia' => '',
        'agencia_dv' => '',
        'conta' => '',
        'conta_dv' => '',
        'carteira' => '',
        'carteira_nosso_numero' => '',
        'aceite' => '',
        'data_vencimento' => 5,
        'demonstrativo1' => '',
        'demonstrativo2' => '',
        'demonstrativo3' => '',
        'instrucoes1' => '',
        'instrucoes2' => '',
        'instrucoes3' => '',
        'instrucoes4' => '',
      ),
    ),
  );

  return $settings;
}

/**
 * Payment method callback: settings form.
 */
function commerce_boleto_settings_form($settings = NULL) {

  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + array(
    'cedente' => '',
    'cpf_cnpj' => '',
    'merchant_logo' => '',
    'endereco' => '',
    'cidade_uf' => '',
  );

  $form['cedente'] = array(
    '#type' => 'textfield',
    '#title' => t("Merchant's Name (Cedente)"),
    '#description' => t("Enter the merchant's name."),
    '#default_value' => $settings['cedente'],
    '#required' => TRUE,
  );

  $form['cpf_cnpj'] = array(
    '#type' => 'number_cnpj_cpf',
    '#title' => t("Merchant's CPF or CNPJ"),
    '#description' => t("Enter either the CPF or CNPJ number."),
    '#default_value' => $settings['cpf_cnpj'],
    '#required' => TRUE,
  );

  $form['merchant_logo'] = array(
    '#type' => 'textfield',
    '#title' => t("Merchant's Logo"),
    '#description' => t("Enter the merchant's logo relative path or full url."),
    '#default_value' => $settings['merchant_logo'],
    '#required' => TRUE,
  );

  $form['endereco'] = array(
    '#type' => 'textfield',
    '#title' => t("Merchant's Address"),
    '#description' => t("Enter the merchant's address."),
    '#default_value' => $settings['endereco'],
    '#required' => TRUE,
  );

  $form['cidade_uf'] = array(
    '#type' => 'textfield',
    '#title' => t('UF'),
    '#description' => t('Enter the 2 letters state code.'),
    '#default_value' => $settings['cidade_uf'],
    '#size' => 2,
    '#required' => TRUE,
  );

  foreach (CommerceBoletoLib::installedPlugins() as $bank_code) {
    $boleto = CommerceBoletoLib::load_boleto(array('bank_code' => $bank_code));

    $bank_name = $boleto->bankNamePropertyGetter();

    $settings = _commerce_boleto_default_settings($bank_code, $settings);

    $form["bank_$bank_code"] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($bank_name),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form["bank_$bank_code"]['enabled'] = array(
      '#type' => 'radios',
      '#title' => t('Enable Carteira'),
      '#description' => t('Define whether or not this issuer bank will be available at the payment check out pane.'),
      '#options' => array(
        1 => t('Enable'),
        0 => t('Disable'),
      ),

      '#ajax' => array(
        'callback' => 'commerce_boleto_settings_ajax_callback',
        'wrapper' => 'commerce_boleto_settings_' . $bank_code . '_wrapper',
      ),
      '#default_value' => $settings["bank_$bank_code"]['enabled'],
    );

    $form["bank_$bank_code"]['bank_code'] = array(
      '#type' => 'hidden',
      '#default_value' => $bank_code,
    );

    $form["bank_$bank_code"]['bank_name'] = array(
      '#type' => 'hidden',
      '#default_value' => $bank_name,
    );

    $form["bank_$bank_code"]['settings'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="commerce_boleto_settings_' . $bank_code . '_wrapper">',
      '#suffix' => '</div>',
    );

    _commerce_boleto_build_settings_form($form, $settings, $bank_code, $bank_name);

  }

  // Make the settings available to the ajax callback.
  $form['#admin_settings'] = $settings;

  return $form;
}

/**
 * AJAX callback for Showing/Hidding the bank issuer settings.
 */
function commerce_boleto_settings_ajax_callback($form, $form_state) {
  $bank_element = $form_state['triggering_element']['#parents'][5];
  $settings = $form['parameter']['payment_method']['settings']['payment_method']['settings']['#admin_settings'];
  $bank_code = $settings[$bank_element]['bank_code'];
  $bank_name = $settings[$bank_element]['bank_name'];
  
  $return_form = array();
  _commerce_boleto_build_settings_form($return_form, $settings, $bank_code, $bank_name);

  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('#commerce_boleto_settings_' . $bank_code . '_wrapper', render($return_form)),
    ),
  );
}

/**
 * Builds the settings form for an enabled Issuer Bank.
 *
 * @param Array $form
 *   All the modification / addition on the array are passed by reference.
 * @param Array $settings
 *   The settings default values.
 * @param String $bank_code
 *   The issuer bank code at the FEBRABAN.
 * @param Array $bank_name
 *   The name of the issuer bank.
 */
function _commerce_boleto_build_settings_form(&$form, $settings, $bank_code, $bank_name) {

  $bank_path = libraries_get_path('boleto-lib') . "/bancos/$bank_code";
  
  $show_hide = 'hidden';

  if ($settings["bank_$bank_code"]['enabled']) {
    $show_hide = 'markup';
  }

  $form["bank_$bank_code"]['settings'] = array(
    '#type' => $show_hide,
    '#title' => t('Settings for @bank_name.', array('@bank_name' => $bank_name)),
    '#description' => t('You are strongly adviced to read the') . ' ' . l('README.txt', "$bank_path/README.txt", array('attributes' => array('target' => '_blank'))) . ' ' . t('file from @bank_name plugin.', array('@bank_name' => $bank_name)), '#default_value' => $settings["bank_$bank_code"]['settings']['conta'],
    '#prefix' => '<div id="commerce_boleto_settings_' . $bank_code . '_wrapper">',
    '#suffix' => '</div>',
  );

  $form["bank_$bank_code"]['settings']['agencia'] = array(
    '#type' => 'textfield',
    '#title' => t('Agencia'),
    '#description' => t('Enter the branch code.'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['agencia'],
    '#size' => 4,
    '#required' => FALSE,
  );

  $form["bank_$bank_code"]['settings']['agencia_dv'] = array(
    '#type' => 'textfield',
    '#title' => t('Agencia Digito'),
    '#description' => t('Optional. Enter the check digit for the branch code.'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['agencia_dv'],
    '#size' => 1,
  );

  $form["bank_$bank_code"]['settings']['conta'] = array(
    '#type' => 'textfield',
    '#title' => t('Conta'),
    '#description' => t('You are strongly adviced to read the') . ' ' . l('README.txt', "$bank_path/README.txt", array('attributes' => array('target' => '_blank'))) . ' ' . t('file from @bank_name plugin.', array('@bank_name' => $bank_name)), '#default_value' => $settings["bank_$bank_code"]['settings']['conta'],
    '#size' => 15,
    '#required' => FALSE,
  );

  $form["bank_$bank_code"]['settings']['conta_dv'] = array(
    '#type' => 'textfield',
    '#title' => t('Conta Digito'),
    '#description' => t('Enter the check digit for the account number.'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['conta_dv'],
    '#size' => 1,
    '#required' => FALSE,
  );

  $form["bank_$bank_code"]['settings']['carteira'] = array(
    '#type' => 'textfield',
    '#title' => t('Carteira'),
    '#description' => t('Enter the value for Carteira.'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['carteira'],
    '#size' => 10,
    '#required' => FALSE,
  );

  $form["bank_$bank_code"]['settings']['carteira_nosso_numero'] = array(
    '#type' => 'textfield',
    '#title' => t('Carteira Nosso Numero'),
    '#description' => t('If required, enter the value for Carteira Nosso Numero.'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['carteira_nosso_numero'],
  );

  $form["bank_$bank_code"]['settings']['aceite'] = array(
    '#type' => 'textfield',
    '#title' => t('Aceite'),
    '#description' => t('The default is N (for No) when left empty.'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['aceite'],
    '#size' => 3,
  );

  $options = array();

  foreach (range(1, 30) as $key => $option) {
    $options[$option] = format_plural($option, 'Add 1 day on top of the issuing date.', 'Add @count days on top of the issuing date.');
  }

  $options[-1] = t('Cash Against Document.');

  $form["bank_$bank_code"]['settings']['data_vencimento'] = array(
    '#type' => 'select',
    '#title' => t('Data de Vencimento'),
    '#description' => t('Select the number of days that will be added to the issuing date for calculating the "due on" date.'),
    '#options' => $options,
    '#default_value' => $settings["bank_$bank_code"]['settings']['data_vencimento'],
  );

  $form["bank_$bank_code"]['settings']['demonstrativo1'] = array(
    '#type' => 'textfield',
    '#title' => t('Demonstrativo'),
    '#description' => t('Line 1 of Demonstrativo'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['demonstrativo1'],
  );

  $form["bank_$bank_code"]['settings']['demonstrativo2'] = array(
    '#type' => 'textfield',
    '#description' => t('Line 2 of Demonstrativo'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['demonstrativo2'],
  );

  $form["bank_$bank_code"]['settings']['demonstrativo3'] = array(
    '#type' => 'textfield',
    '#description' => t('Line 3 of Demonstrativo'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['demonstrativo3'],
  );

  $form["bank_$bank_code"]['settings']['instrucoes1'] = array(
    '#type' => 'textfield',
    '#title' => t('Instrucoes'),
    '#description' => t('Line 1 of Instrucoes'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['instrucoes1'],
  );

  $form["bank_$bank_code"]['settings']['instrucoes2'] = array(
    '#type' => 'textfield',
    '#description' => t('Line 2 of Instrucoes'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['instrucoes2'],
  );

  $form["bank_$bank_code"]['settings']['instrucoes3'] = array(
    '#type' => 'textfield',
    '#description' => t('Line 3 of Instrucoes'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['instrucoes3'],
  );

  $form["bank_$bank_code"]['settings']['instrucoes4'] = array(
    '#type' => 'textfield',
    '#description' => t('Line 4 of Instrucoes'),
    '#default_value' => $settings["bank_$bank_code"]['settings']['instrucoes4'],
  );
}
