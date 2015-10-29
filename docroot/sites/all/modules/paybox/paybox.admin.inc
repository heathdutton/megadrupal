<?php

/*
 * @file
 * Administration form
 */
function paybox_settings_form()
{
	$form = array ();
	
	$form['paybox_activate_real_payments'] = array(
		'#type' => 'checkbox',
		'#title' => t('Activate real payments.<br/><b>WARNING</b>: CHECK THIS ONLY ON <b>PRODUCTION MODE</b>. This will make payment requests use the Paybox production server, not the sandbox host.'),
		'#default_value' => variable_get('paybox_activate_real_payments', false),
	);
	
	$form['hosts'] = array(
		'#type' => 'fieldset',
		'#title' => t('Hosts'),
	);
	$form['hosts']['production'] = array(
		'#type' => 'fieldset',
		'#title' => t('Production'),
	);
	$form['hosts']['sandbox'] = array(
		'#type' => 'fieldset',
		'#title' => t('Sandbox'),
	);
	
	$form['hosts']['production']['paybox_production_host'] = array(
		'#type' => 'textfield',
		'#title' => t('Paybox production host'),
		'#default_value' => variable_get('paybox_production_host', ''),
		'#size' => 100,
		'#maxlength' => 100,
	);
	
	$form['hosts']['production']['paybox_production_hash_key'] = array (
		'#type' => 'textfield',
		'#title' => t('Hash key for hmac mode.'),
		'#default_value' => variable_get('paybox_production_hash_key', 'null'),
		'#description' => 'Generate this key ay admin.paybox.com.',
		'#size' => 200,
		'#disabled' => variable_get('paybox_mode', '1') == '1',		
	);
	
	$form['hosts']['sandbox']['paybox_sandbox_host'] = array(
		'#type' => 'textfield',
		'#title' => t('Paybox sandbox host'),
		'#default_value' => variable_get('paybox_sandbox_host', ''),
		'#size' => 100,
		'#maxlength' => 100,
	);
	
	$form['hosts']['sandbox']['paybox_sandbox_hash_key'] = array (
		'#type' => 'textfield',
		'#title' => t('Hash key for hmac mode.'),
		'#default_value' => variable_get('paybox_sandbox_hash_key', 'null'),
		'#description' => 'Generate this key ay preprod-admin.paybox.com.',
		'#size' => 200,
		'#required' => TRUE,
		'#disabled' => variable_get('paybox_mode', '1') == '1',		
	);
	
	// default values are for test account
	$form['paybox_PBX_SITE'] = array (
		'#type' => 'textfield',
		'#title' => t('Site number'),
		'#description' => 'PBX_SITE',
		'#default_value' => variable_get('paybox_PBX_SITE', '1999888'),
		'#size' => 7,
		'#maxlength' => 7,
		'#required' => TRUE,
		'#element_validate' => array (
			'paybox_site_number_validate'
		),
		
	);

	$form['paybox_PBX_RANG'] = array (
		'#type' => 'textfield',
		'#title' => t('Rank number'),
		'#description' => 'PBX_RANG',
		'#default_value' => variable_get('paybox_PBX_RANG', '99'),
		'#size' => 2,
		'#maxlength' => 2,
		'#required' => TRUE,
		'#element_validate' => array (
			'paybox_rank_number_validate'
		),
		
	);

	$form['paybox_PBX_IDENTIFIANT'] = array (
		'#type' => 'textfield',
		'#title' => t('PAYBOX identifier'),
		'#description' => 'PBX_IDENTIFIANT',
		'#default_value' => variable_get('paybox_PBX_IDENTIFIANT', '2'),
		'#size' => 9,
		'#maxlength' => 9,
		'#required' => TRUE,
		'#element_validate' => array (
			'paybox_identifier_validate'
		),
	);
	
	$form['paybox_authorized_ips'] = array(
		'#type' => 'textfield',
		'#title' => t('Authorized ips on callback urls, comma separated'),
		'#default_value' => variable_get('paybox_authorized_ips', '127.0.0.1'),
		'#size' => 200,
		'#required' => TRUE,
	);

	$form['paybox_effectue_message'] = array (
		'#type' => 'textfield',
		'#title' => t('Message when payment succeeded.'),
		'#default_value' => variable_get('paybox_effectue_message', t('Payment completed. Thank you.')),
		'#required' => TRUE,
		
	);

	$form['paybox_refuse_message'] = array (
		'#type' => 'textfield',
		'#title' => t('Message when payment was refused.'),
		'#default_value' => variable_get('paybox_refuse_message', t('Payment refused. Sorry.')),
		'#required' => TRUE,
		
	);

	$form['paybox_annule_message'] = array (
		'#type' => 'textfield',
		'#title' => t('Message when payment was cancelled.'),
		'#default_value' => variable_get('paybox_annule_message', t('Payment cancelled.')),
		'#required' => TRUE,
		
	);

	return system_settings_form($form);
}

function paybox_site_number_validate($element)
{
	if (preg_match('/[0-9]{7}/', $element['#value']))
		return;
	form_error($element, t('The site number must have 7 digits.'));
}

function paybox_rank_number_validate($element)
{
	if (preg_match('/[0-9]{2}/', $element['#value']))
		return;
	form_error($element, t('The rank number must have 2 digits.'));
}

function paybox_identifier_validate($element)
{
	if (preg_match('/[0-9]{1,9}/', $element['#value']))
		return;
	form_error($element, t('The identifier must have 1 to 9 digits.'));
}

?>
