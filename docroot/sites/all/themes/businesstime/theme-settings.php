<?php

drupal_add_css(drupal_get_path('theme', 'businesstime') . '/css/theme-settings.css', array('group' => CSS_THEME, 'weight' => 100));

// Get the number of columns
function get_columns() {
	$grid_size = theme_get_setting('grid_size');
	$columns = array();
	for ($grid_unit = 0; $grid_unit <= $grid_size; $grid_unit++) {
	 $columns[] = $grid_unit;
	 $columns[$grid_unit] = $grid_unit . ' columns';
	}
	return $columns;	
}

function businesstime_form_system_theme_settings_alter(&$form, $form_state) {

  $grid_size = theme_get_setting('grid_size');
  $col_number = get_columns();

  $form['advanced_settings'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Advanced Theme Settings'),
    '#description' => t('Customize widths of the Preface and Postscript regions.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
		'#weight' => -10,
    '#prefix' => t('<h3> Advanced Settings </h3>')
  );	
	
// Grid Settings		
	
  $form['advanced_settings']['grid_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grid Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
		'#weight' => -10,
  );	
	
  $form['advanced_settings']['grid_settings']['grid_size'] = array(
    '#type' => 'select',
    '#title' => t('Grid Size'),
    '#default_value' => theme_get_setting('grid_size'),
		'#options' => array(
			12 => t('12 columns')
		),
  );	
	
  $form['advanced_settings']['grid_settings']['sidebar_grid_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sidebar Grid Settings'),
    '#description' => t('Customize the Sidebars.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
				'class' => array('form-inline'),
     ),
		 '#prefix' => t('<h3> Sidebar Grid Settings </h3>')
  );		
	
  $form['advanced_settings']['grid_settings']['sidebar_grid_settings']['sidebar_layout'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar Layout'),
    '#default_value' => theme_get_setting('sidebar_layout'),
		'#options' => array(
			'sidebars-both-first' => t('Both Sidebars First'),
			'sidebars-both-second' => t('Both Sidebars Second'),
			'sidebars-split' => t('Split Sidebars'),
		),
  );		
	
  $form['advanced_settings']['grid_settings']['sidebar_grid_settings']['sidebar_first_width'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar First'),
    '#default_value' => theme_get_setting('sidebar_first_width'),
    '#options' => $col_number,
  );
	
  $form['advanced_settings']['grid_settings']['sidebar_grid_settings']['sidebar_second_width'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar Second'),
    '#default_value' => theme_get_setting('sidebar_second_width'),
    '#options' => $col_number,
  );	
	
  $form['advanced_settings']['grid_settings']['preface_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('More Grid Settings'),
    '#description' => t('Customize widths of the Preface regions.  Note that all four values combined should ideally add up to ' . $grid_size . '.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
				'class' => array('form-inline'),
     ),
		 '#prefix' => t('<h3> Preface Grid Widths </h3>')
  );	
	
  $form['advanced_settings']['grid_settings']['postscript_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('More Grid Settings'),
    '#description' => t('Customize widths of the Postscript regions.  Note that all four values combined should ideally add up to ' . $grid_size . '.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
				'class' => array('form-inline'),
     ),
		 '#prefix' => t('<h3> Postscript Grid Widths </h3>')
  );		
	
	for ($region_count = 1; $region_count <= 4; $region_count++) {
		
		$form['advanced_settings']['grid_settings']['preface_settings']['preface_' . $region_count . '_grid_width'] = array(
			'#type' => 'select',
			'#title' => t('Preface ' . $region_count),
			'#default_value' => theme_get_setting('preface_' . $region_count . '_grid_width'),
			'#options' => $col_number,
		);	
		
		$form['advanced_settings']['grid_settings']['postscript_settings']['postscript_' . $region_count . '_grid_width'] = array(
			'#type' => 'select',
			'#title' => t('Postscript ' . $region_count),
			'#default_value' => theme_get_setting('postscript_' . $region_count . '_grid_width'),
			'#options' => $col_number,
		);			
		
	}
	
}