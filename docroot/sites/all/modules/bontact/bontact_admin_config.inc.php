<?php

/**
 * Admin configurations
 * 
 *  @author Bhavin Joshi <bhavinjosi@joshics.in>
 */

/**
 * bontact_admin_config
 * 
 * Configure Bontact client
 * 
 * @author Bhavin Joshi <bhavinjosi@joshics.in>
 */
function bontact_admin_config() {
	$form = array();
	
	$form['bontact_customer'] = array(
			'#type' => 'textfield',
			'#title' => t('Bontact Customer ID'),
			'#description' => t('Unique Customer ID from your Bontact Dashboard. Use the one defined with \'var bontactCustomer\' in the javascript code, under Setting > Code embed '),
			'#default_value' => variable_get('bontact_customer', ''),
	);
	
	$form['signup'] = array(
			'#markup' => t('If you do not have an acount with Bontact, !signup', array('!signup' =>l(t('Sign up'), 'http://bit.ly/1OWETuG'))),
			'#weight' => -50,
	);
	
	return system_settings_form($form);
}