<?php
function lightword_form_system_theme_settings_alter(&$form, &$form_state) {
	$form['layout_settings'] = array(
			'#type' => 'fieldset',
			'#title' => t('Layout settings')
	);
	$form['layout_settings']['screen_settings'] = array(
    '#type' => 'select',
	'#options' => array(
			'normal' => t('Normal'),
			'wider' => t('Wider'),
	),
    '#title' => t('Screen Settings'),
    '#default_value' => theme_get_setting('screen_settings'),
  );
	
	$form['cufon_settings'] = array(
			'#type' => 'fieldset',
			'#title' => t('Cufon settings')
	);
	$form['cufon_settings']['cuf_settings'] = array(
			'#type' => 'select',
			'#options' => array(
					'disabled' => t('Disabled'),
					'enabled' => t('Enabled'),					
					'extra' => t('Extra'),
					'css3' => t('CSS3 Font-face (lightweight)'),
			),
			'#title' => t('Cufon Settings'),
			'#default_value' => theme_get_setting('cuf_settings'),
	);
	
	
}

