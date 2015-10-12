<?php

function robokassa_payments_pre() {

  if (!empty($_POST['payments']) && isset($_POST['operation']) && ($_POST['operation'] == 'delete')) {
    return drupal_get_form('robokassa_payments_delete_confirm');
  }
  else {
    return drupal_get_form('robokassa_payments');
  }
}

function robokassa_payments($form, &$form_state) {

  $header = array(
    'pid' => array('data' => t('ID'), 'field' => 'r.pid', 'sort' => 'desc'),
    'created' => array('data' => t('Created'), 'field' => 'r.created'),
    'name' => array('data' => t('User'), 'field' => 'r.uid'),
    'amount' => array('data' => t('Amount'), 'field' => 'r.amount'),
    'memo' => t('Memo'),
    'enrolled' => array('data' => t('Enrolled'), 'field' => 'r.enrolled'),
  );

  $select = db_select('robokassa', 'r')->extend('PagerDefault')->extend('TableSort');
  $result = $select->fields('r', array('pid', 'uid', 'created', 'amount', 'memo', 'enrolled', 'error'))
    ->orderByHeader($header)->limit(50)->execute();
  $payments = array();
  foreach ($result as $payment) {
    $user = array('account' => user_load($payment->uid));
    $payments[$payment->pid] = array(
      'pid' => $payment->pid,
      'created' => date("m/d/Y H:i", $payment->created),
      'name' => theme('username', $user),
      'amount' => number_format($payment->amount, 2).variable_get('robokassa_currency_suffix', ' руб.'),
      'memo' => $payment->memo,
      'enrolled' => (($payment->enrolled > 0 && $payment->error == '') ? date("m/d/Y H:i", $payment->enrolled) :
        (!empty($payment->error) ? '<small><B>Error: </B>' . $payment->error . ' (' . date("m/d/Y H:i", $payment->enrolled) . ')</smal>' : '-'))
    );
  }

  $form['payments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $payments,
    '#empty' => t('No payments available.'),
  );
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#weight' => -1
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => array('delete' => t('Delete selected'), 'enroll' => t('Enroll selected')),
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
  );


  $form['pager'] = array('#theme' => 'pager', '#weight' => 5);
  return $form;

}

function robokassa_payments_validate($form, &$form_state) {
  $form_state['values']['payments'] = array_filter($form_state['values']['payments']);
  if (count($form_state['values']['payments']) == 0) {
    form_set_error('', t('No payments selected.'));
  }
}

function robokassa_payments_submit($form, &$form_state) {
  $payments = array_filter($form_state['values']['payments']);
  switch ($form_state['values']['operation']) {
    case 'enroll':
      $t = time();
      foreach ($payments as $pid) {
        if (_robokassa_enrollpayment($pid, $t)) {
          module_invoke_all('robokassa', 'enrolled', $pid);
          drupal_set_message(t('The payments have been enrolled.'));
        }
      }
      break;
  }
}

function robokassa_payments_delete_confirm($form, &$form_state) {
  $edit = $form_state['input'];
  $form['payments'] = array('#tree' => TRUE);
  foreach (array_filter($edit['payments']) as $pid => $value) {
    $form['payments'][$pid] = array('#type' => 'hidden', '#value' => $pid);
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  return confirm_form($form,
    t('Are you sure you want to delete selected payments?'),
    'admin/robokassa/payments', t('This action cannot be undone.'),
    t('Delete all selected'), t('Cancel'));
}

function robokassa_payments_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['payments'] as $pid => $value) {
      _robokassa_deletepayment($pid);
    }
    drupal_set_message(t('The payments have been deleted.'));
  }
  $form_state['redirect'] = 'admin/robokassa/payments';
  return;
}

// ---------------------------------------------------------------

function robokassa_settingsform($form, &$form_state) {

  global $base_url;
  $form['robokassa_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Robokassa login'),
    '#default_value' => variable_get('robokassa_login', ''),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE
  );

  $form['payment_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Robokassa merchant URL'),
    '#default_value' => variable_get('robokassa_payment_url', ROBOKASSA_MERCHANT_URL),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE
  );

  $form['secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment password #1'),
    '#default_value' => variable_get('robokassa_secretkey', ''),
    '#size' => 40,
    '#maxlength' => 100,
    '#required' => FALSE
  );

  $form['secret_key2'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment password #2'),
    '#default_value' => variable_get('robokassa_secretkey2', ''),
    '#size' => 40,
    '#maxlength' => 100,
    '#required' => FALSE
  );


  $form['default_currency'] = array(
    '#type' => 'textfield',
    '#title' => t('Default currency'),
    '#default_value' => variable_get('robokassa_default_currency', 'BANKOCEAN2R'),
    '#description' => t("<a href=\"https://merchant.roboxchange.com/WebService/Service.asmx/GetCurrencies?MerchantLogin=demo&Language=ru\">List of currencies</a> on Robokassa"),
    '#size' => 40,
    '#maxlength' => 100,
    '#required' => FALSE
  );

  $form['currency_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency suffix'),
    '#default_value' => variable_get('robokassa_currency_suffix', ' руб.'),
    '#description' => t("Change currency suffix"),
    '#size' => 20,
    '#maxlength' => 30,
    '#required' => FALSE
  );

  $form['result_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Result URL'),
    '#default_value' => $base_url . '/' . drupal_get_path_alias('robokassa/status'),
    '#description' => t("Change default value to increase security"),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => FALSE
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

function robokassa_settingsform_validate($form, &$form_state) {
  global $base_url;
  if (!empty($form_state['values']['result_url'])) {
    if (!preg_match("|^" . $base_url . "|", $form_state['values']['result_url'])) {
      form_set_error('result_url', t('You can not change site address, only change path.'));
    }
    elseif (!valid_url($form_state['values']['result_url'], TRUE)) {
      form_set_error('result_url', t('Wrong URL.'));
    }
  }
}

function robokassa_settingsform_submit(&$form, $form_state) {
  global $base_url;
  $values = $form_state['values'];
  path_delete(array('source' => 'robokassa/status'));

  if (isset($values['result_url'])) {
    $values['result_url'] = trim(str_replace($base_url, '', $values['result_url']), '/');
    if ($values['result_url'] != 'robokassa/status') {
      $path = array('source' => 'robokassa/status', 'alias' => $values['result_url']);
      path_save($path);
    }
  }

  variable_set('robokassa_payment_url', $values['payment_url']);
  variable_set('robokassa_secretkey', $values['secret_key']);
  variable_set('robokassa_secretkey2', $values['secret_key2']);
  variable_set('robokassa_login', $values['robokassa_login']);
  variable_set('robokassa_default_currency', $values['default_currency']);
  variable_set('robokassa_currency_suffix', $values['currency_suffix']);

  drupal_set_message(t("Settings has been saved."));
  drupal_goto('admin/robokassa/settings');
}

// ---------------------------------------------------------------
function robokassa_sample() {
  global $base_url;
  return '<br><h3><u>' . t('Setting up Robokassa Merchant Account') . '</u></h3>
1. Зарегистрируте Мерчант на сайте <a href="https://www.roboxchange.com/Environment/Partners/Reg/Register.aspx?reg=Merchant" target="_blank">robokassa.ru</a><br>
2. <a href="/admin/robokassa/settings">Настройте</a> данный модуль, указав логин, который Вы зарегистрировали в п.1 и пароль платежей (Это не пароль входа на сайт! Придумайте свой.)<br>
3. Перейдите на <a href="https://www.roboxchange.com/Environment/Partners/Login/Merchant/Administration.aspx" target="_blank">страницу настройки мерчанта</a> и введите настройки как показано ниже:
  <table border="0" cellpadding="0" cellspacing="0" style="text-align: left;">
                <tr>
                    <td width="30%">Пароль #1</span>:
                    <br />[<small>используется интерфейсом инициализации оплаты</small>]</td>
                    <td style="white-space:nowrap"><input type="text" style="width:100%" value="' . variable_get('robokassa_secretkey', 'введите такой же пароль как в настройках этого модуля') . '"/></td>
                </tr>                
                <tr>
                    <td>Пароль #2:
                    <br />[<small>используется интерфейсом оповещения о платеже, XML-интерфейсах</small>]</td>
                    <td style="white-space:nowrap"><input type="text" style="width:100%" value="' . variable_get('robokassa_secretkey2', 'введите такой же пароль как в настройках этого модуля') . '" />
</td>
                </tr>
                <tr><td colspan="2" class="nopadding"><div class="line"></div></td></tr> 
                <tr>
                    <td>Result URL:
                    <br />[<small>используется для оповещения о платеже, если метод отсылки - email, то email-адрес</small>]</td>
                    <td><input type="text" value="' . $base_url . '/' . drupal_get_path_alias('robokassa/status') . '" style="width:100%" />
                        </td>
                </tr>
                <tr><td colspan="2" class="nopadding"><div class="line"></div></td></tr>                
                <tr>
                    <td>
                        Метод отсылки данных по Result URL:</td>
                    <td>
                        <select>
	<option value="G">GET</option>
	<option selected="selected" value="P">POST</option>
	<option value="E">e-mail</option>
 
</select></td>
                </tr>
                <tr><td colspan="2" class="nopadding"><div class="line"></div></td></tr>
                <tr>
                    <td>        
                        Success URL:
                        <br />[<small>используется в случае успешного проведения платежа</small>]</td>
                    <td>
                        <input type="text" value="' . $base_url . '/robokassa/success' . '" style="width:100%" /></td>
                </tr>
                <tr><td colspan="2"><div class="line"></div></td></tr>
                <tr>
                    <td>
                        Метод отсылки данных по Success URL:</td>
                    <td>
                        <select>
	<option selected="selected" value="G">GET</option>
	<option value="P">POST</option>
 
</select></td>                    
                </tr>
                <tr><td colspan="2" class="nopadding"><div class="line"></div></td></tr>
                <tr>
                    <td>        
                        Fail URL:
                        <br />[<small>используется в случае отказа проведения платежа</small>]</td>
                    <td>
                        <input type="text" value="' . $base_url . '/robokassa/fail' . '" style="width:100%" /></td>
                </tr>
                <tr><td colspan="2" class="nopadding"><div class="line"></div></td></tr>
                <tr>
                    <td>
                        Метод отсылки данных по Fail URL:</td>
                    <td>
                        <select>
	<option selected="selected" value="G">GET</option>
	<option value="P">POST</option>
 
</select></td>                    
                </tr>
</table>';
}