<?php

function perfectmoney_sci_payments_pre() {

  if (!empty($_POST['payments']) && isset($_POST['operation']) && ($_POST['operation'] == 'delete')) {
    return drupal_get_form('perfectmoney_sci_payments_delete_confirm');
  }
  else {
    return drupal_get_form('perfectmoney_sci_payments');
  }
}

function perfectmoney_sci_payments($form, &$form_state) {

  $header = array(
    'pid' => array('data' => t('ID'), 'field' => 'r.pid', 'sort' => 'desc'),
    'created' => array('data' => t('Created'), 'field' => 'r.created'),
    'user' => array('data' => t('User'), 'field' => 'r.uid'),
    'amount' => array('data' => t('Amount'), 'field' => 'r.amount'),
    'currency' => array('data' => t('Currency'), 'field' => 'r.currency'),
    'memo' => t('Memo'),
    'enrolled' => array('data' => t('Enrolled'), 'field' => 'r.enrolled'),
    'batch' => array('data' => t('PM batch'), 'field' => 'r.batch'),
    'payee_account' => array('data' => t('Payee account'), 'field' => 'r.payee_account'),
    'payer_account' => array('data' => t('Payer account'), 'field' => 'r.payer_account'),
  );

  $currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize(_perfectmoney_sci_GetDefCurSetts())));

  $select = db_select('perfectmoney_sci', 'r')->extend('PagerDefault')->extend('TableSort');
  $result = $select->fields('r', array('pid', 'uid', 'created', 'amount', 'currency', 'memo', 'enrolled', 'batch', 'payee_account', 'payer_account', 'error'))
    ->orderByHeader($header)->limit(50)->execute();
  $payments = array();
  foreach ($result as $payment) {
    $user = array('account' => user_load($payment->uid));
    $payments[$payment->pid] = array(
      'pid' => $payment->pid,
      'created' => date("m/d/Y H:i", $payment->created),
      'user' => theme('username', $user),
      'amount' =>  round($payment->amount, $currency_settings[$payment->currency]['presc']),
      'currency' => $payment->currency,
      'memo' => $payment->memo,
      'enrolled' => (($payment->enrolled > 0 && $payment->error == '') ? date("m/d/Y H:i", $payment->enrolled) :
        (!empty($payment->error) ? '<small><B>Error: </B>' . $payment->error . ' (' . date("m/d/Y H:i", $payment->enrolled) . ')</smal>' : '-')),
      'batch' => $payment->batch,
      'payee_account' => $payment->payee_account,
      'payer_account' => $payment->payer_account
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

function perfectmoney_sci_payments_validate($form, &$form_state) {
  $form_state['values']['payments'] = array_filter($form_state['values']['payments']);
  if (count($form_state['values']['payments']) == 0) {
    form_set_error('', t('No payments selected.'));
  }
}

function perfectmoney_sci_payments_submit($form, &$form_state) {
  $payments = array_filter($form_state['values']['payments']);
  switch($form_state['values']['operation']){
  case 'enroll':
	  $t=time();
	  foreach ($payments as $pid){
		if(_perfectmoney_sci_enrollpayment($pid, 'MANUALLY', $t))
			module_invoke_all('perfectmoney_sci', 'enrolled', $pid);
	  }
	  drupal_set_message(t('The payments have been enrolled.'));
  break;
  }
}

function perfectmoney_sci_payments_delete_confirm($form, &$form_state) {
  $edit = $form_state['input'];
  $form['payments'] = array('#tree' => TRUE);
  foreach (array_filter($edit['payments']) as $pid => $value) {
    $form['payments'][$pid] = array('#type' => 'hidden', '#value' => $pid);
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  return confirm_form($form,
    t('Are you sure you want to delete selected payments?'),
    'admin/perfectmoney/payments', t('This action cannot be undone.'),
    t('Delete all selected'), t('Cancel'));
}

function perfectmoney_sci_payments_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['payments'] as $pid => $value) {
      _perfectmoney_sci_deletepayment($pid);
    }
    drupal_set_message(t('The payments have been deleted.'));
  }
  $form_state['redirect'] = 'admin/perfectmoney/payments';
  return;
}

// ---------------------------------------------------------------

function perfectmoney_sci_settingsform($form, &$form_state) {

  global $base_url;
  $form['payee_name'] = array(
		'#type' => 'textfield', 
		'#description' => t("Example: 'ABC Car Inc'"),
		'#title' => t('Payee name'), 
		'#default_value' => variable_get('perfectmoney_sci_payee_name',  variable_get('site_name', 'Drupal')), 
		'#size' => 60, 
		'#maxlength' => 255, 
		'#required' => TRUE
	);

	$form['payment_url'] = array(
		'#type' => 'textfield', 
		'#title' => t('SCI URL'), 
		'#default_value' => variable_get('perfectmoney_sci_payment_url', PERFECTMONEY_SCI_PAYMENT_URL), 
		'#size' => 60, 
		'#maxlength' => 255, 
		'#required' => TRUE
	);

	$form['secret_key'] = array(
		'#type' => 'textfield', 
		'#title' => t('Alternate PassPhrase'), 
		'#default_value' =>  variable_get('perfectmoney_sci_secretkey', ''), 
		'#size' => 40, 
		'#maxlength' => 100, 
		'#required' => FALSE
	);

	$form['result_url'] = array(
		'#type' => 'textfield', 
		'#title' => t('Result URL'), 
		'#default_value' => $base_url.'/'.drupal_get_path_alias('perfectmoney/status'), 
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

function perfectmoney_sci_settingsform_validate($form, &$form_state) {
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

function perfectmoney_sci_settingsform_submit(&$form, $form_state) {
  global $base_url;
  $values = $form_state['values'];
  path_delete(array('source' => 'perfectmoney/status'));

  if (isset($values['result_url'])) {
    $values['result_url'] = trim(str_replace($base_url, '', $values['result_url']), '/');
    if ($values['result_url'] != 'perfectmoney/status') {
      $path = array('source' => 'perfectmoney/status', 'alias' => $values['result_url']);
      path_save($path);
    }
  }

  variable_set('perfectmoney_sci_payment_url',  $form_state['values']['payment_url']);

  variable_set('perfectmoney_sci_secretkey',  $form_state['values']['secret_key']);

  variable_set('perfectmoney_sci_payee_name',  $form_state['values']['payee_name']);


  drupal_set_message(t("Settings has been saved."));
  drupal_goto('admin/perfectmoney/settings');
}

function perfectmoney_sci_currencies(){

	$default_currency_settings=_perfectmoney_sci_GetDefCurSetts();

	$currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize($default_currency_settings)));


	foreach($currency_settings as $key=>$value){
		
			$row=array();
  
			$row[] = array('data' => $key);
			$row[] = array('data' => ((empty($value['account'])) ? 'n/a' : $value['account']));
			$row[] = array('data' => (($value['enabled']==1) ? 'yes' : 'no'));
			$row[] = array('data' => l('edit', 'admin/perfectmoney/currencies/edit/'.$key).' '.l('delete', 'admin/perfectmoney/currencies/delete/'.$key).' '.l('view sample', 'admin/perfectmoney/sample/'.$key));
 
			$rows[] = $row;

	}
 
	// Individual table headers.
	$header = array();
	$header[] = t('Currency');
	$header[] = t('PM-account');
	$header[] = t('Enabled');
	$header[] = t('Action');
 
	$output = theme_table(array(
		'header'=>$header, 
		'rows'=>$rows, 
		'attributes'=>array(), 
		'caption'=>'', 
		'colgroups'=>array(),
		'empty' => t('Please add at least one currency.'),
		'sticky'=>false
	));

	return $output;

}


function perfectmoney_sci_currency_edit($form, &$form_state, $cur=''){

		$currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize(_perfectmoney_sci_GetDefCurSetts())));

		if(!empty($cur)) $currency=$currency_settings[$cur];


		// interface data:

		if(!empty($cur)){

			$form['cur'] = array(
			  '#type' => 'item',
			  '#title' => t('Currency'),
			  '#description' => $cur
			);

			$form['currency'] = array(
				'#type' => 'hidden',
				'#value' => $cur,
			);


		}else{

			$form['currency'] = array(
				'#type' => 'textfield', 
				'#title' => t('Currency'), 
				'#description' => t('Enter currency ID string like "USD"'), 
				'#default_value' => '', 
				'#size' => 10, 
				'#maxlength' => 15, 
				'#required' => TRUE
			);

			$currency['account']='';

			$currency['presc']='';

			$currency['enabled']='';

		}

	   

		$form['account'] = array(
			'#type' => 'textfield', 
			'#title' => t('PM account'), 
			'#default_value' => $currency['account'], 
			'#size' => 20, 
			'#maxlength' => 15, 
			'#required' => FALSE
		);

		$form['presc'] = array(
			'#type' => 'textfield', 
			'#title' => t('Decimals'), 
			'#default_value' => $currency['presc'], 
			'#size' => 5, 
			'#maxlength' => 3, 
			'#required' => TRUE
		);

		$form['enabled'] = array(
		  '#type' => 'checkbox', 
		  '#title' => t('Enabled'),
		  '#default_value' => $currency['enabled']
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save changes'),
		);


		return $form;

}

function perfectmoney_sci_currency_edit_validate($form, &$form_state){
	

	if (empty($form_state['values']['currency'])) 
		form_set_error('', t('Currency ID does not specified.'));
	
	if($form_state['values']['enabled']=='1'){
		if(empty($form_state['values']['account']))
			form_set_error('account', t('PM account can not be empty for enabled currency'));
	}

	if(!empty($form_state['values']['account']) && !empty($form_state['values']['currency'])){

		$letter='';
		switch($form_state['values']['currency']){
		case 'USD': $letter='U'; break;
		case 'EUR': $letter='E'; break;
		case 'GOLD': $letter='G'; break;
		}

		if($letter!=''){

			if($letter!=substr($form_state['values']['account'], 0, 1))
				form_set_error('account', t('PM account must start from !letter for !currency currency', array('!letter'=>$letter, '!currency'=>$form_state['values']['currency'])));
		}

	}


}


function perfectmoney_sci_currency_edit_submit(&$form, &$form_state){
	
	$currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize(_perfectmoney_sci_GetDefCurSetts())));

	$currency_settings["{$form_state['values']['currency']}"]=array(
		'enabled' => $form_state['values']['enabled'],
		'account' => $form_state['values']['account'],
		'presc' =>  (int)$form_state['values']['presc']
	);

	variable_set('perfectmoney_sci_currencies', serialize($currency_settings));

	drupal_set_message("Currency has been saved.");

	$form_state['redirect'] = 'admin/perfectmoney/currencies';

}



function perfectmoney_sci_currency_delete($form, &$form_state, $cur) {

  $form['cur'] = array(
    '#type' => 'value',
    '#value' => $cur,
  );

  return confirm_form($form,
    t('Are you sure you want to delete %cur ?', array('%cur' => $cur)),
    'admin/perfectmoney/currencies',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function perfectmoney_sci_currency_delete_submit($form, &$form_state) {

	if ($form_state['values']['confirm']){

		$currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize(_perfectmoney_sci_GetDefCurSetts())));
		
		$out=array();
		foreach($currency_settings as $key=>$value){
			if($key!=$form_state['values']['cur']) $out[$key]=$value;
		}

		if(count($out)<count($currency_settings)){
			variable_set('perfectmoney_sci_currencies', serialize($out));
			drupal_set_message("Currency has been deleted.");
		}
		else drupal_set_message("Currency has NOT been deleted.");


	}else drupal_set_message("Currency has NOT been deleted.");

	$form_state['redirect'] = 'admin/perfectmoney/currencies';

}
// ---------------------------------------------------------------
function perfectmoney_sci_sample() {

	$default_currency_settings=_perfectmoney_sci_GetDefCurSetts();

  	$currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize($default_currency_settings)));

	$cur=arg(3);
	if(!empty($cur) && array_key_exists($cur, $currency_settings)) $account=$currency_settings[$cur]['account'];
	else $account=$currency_settings['USD']['account'];

  return t('<h1>Setting up Perfect Money</h1>
<p><A HREF="https://perfectmoney.is/login.html">Login</A> to your Perfect Money account, click on <A HREF="https://perfectmoney.is/settings.html">Settings</A> menu section and fill in <B>Alternate Passphrase</B> field:
<table cellpadding="5">
<tbody><tr>
<td nowrap="nowrap">Alternate Passphrase:</td>
<td align="left"><input style="display: inline;" value="!secret_key" size="50" id="m_name" name="m_name" type="text"></td>
</tr>
</tbody></table>',array(
     '!secret_key' => variable_get('perfectmoney_sci_secretkey', ''),
     ));

}