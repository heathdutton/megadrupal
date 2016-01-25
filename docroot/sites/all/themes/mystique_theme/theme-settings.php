<?php
/**
 * ALter the themes settings
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function mystique_theme_form_system_theme_settings_alter(&$form, &$form_state) {
	$form['layout_settings'] = array(
			'#type' => 'fieldset',
			'#title' => t('Color settings')
	);
	$form['layout_settings']['color_settings'] = array(
    '#type' => 'radios',
	  '#options' => array(
		'green' => t('Green'),
		'blue' => t('Blue'),
	  'red' => t('Red'),
	  'grey' => t('Grey'),
	),
    '#title' => t('Color Settings'),
    '#default_value' => theme_get_setting('color_settings'),
  );
	
	$form['cufon_settings'] = array(
			'#type' => 'fieldset',
			'#title' => t('Font  settings')
	);
	$form['cufon_settings']['font_settings'] = array(
			'#type' => 'radios',
			'#options' => array(
					'arial' => t('Helvetica/Arial'),					
					'georgia' => t('Georgia (sans serif)'),
					'lucida' => t('Lucida Grande/Sans (Mac/Windows)'),
			),
			'#title' => t('Font Settings'),
			'#default_value' => theme_get_setting('font_settings'),
	);
	
  $form['layout_settings']['page_width'] = array(
    '#type' => 'radios',
    '#title' => t('Page Width'),
    '#default_value' => theme_get_setting('page_width'),
    '#options' => array(
      'fixed' => t('Fixed (960gs)'),
      'fluid' => t('Fluid (100%)'),
    ),
  );
 	
}
