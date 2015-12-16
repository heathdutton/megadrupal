<?php

drupal_add_css(
      ' .vertical-tabs fieldset div fieldset legend { display: block; } ',
      array( 'group' => CSS_THEME, 'type' => 'inline',)
);

/**
 * Create Options variables
 *
 */
 
 
$options_weight  = array('a'		=>	t('-10'),'b'	=>	t('-9'),'c'		=>	t('-8'),'d'		=>	t('-7'),'e'		=>	t('-6'),'f'		=>	t('-5'),'g'		=>	t('-4'),'h'		=>	t('-3'),'i'		=>	t('-2'),'j'		=>	t('-1'),'k'	=>	t('0'),'l'		=>	t('1'),'m'		=>	t('2'),'n'		=>	t('3'),'o'		=>	t('4'),'p'		=>	t('5'),'q'		=>	t('6'),'r'		=>	t('7'),'s'		=>	t('8'),'t'		=>	t('9'),'u'		=>	t('10'),);
$options_grid_12 = array('0'		=>	t('0'),'1'		=>	t('1'),'2'		=>	t('2'),'3'		=>	t('3'),'4'		=>	t('4'),'5'		=>	t('5'),'6'		=>	t('6'),'7'		=>	t('7'),'8'		=>	t('8'),'9'		=>	t('9'),'10'	=>	t('10'),'11'	=>	t('11'),'12'	=>	t('12'),);
$options_grid_16 = array('0'		=>	t('0'),'1'		=>	t('1'),'2'		=>	t('2'),'3'		=>	t('3'),'4'		=>	t('4'),'5'		=>	t('5'),'6'		=>	t('6'),'7'		=>	t('7'),'8'		=>	t('8'),'9'		=>	t('9'),'10'	=>	t('10'),'11'	=>	t('11'),'12'	=>	t('12'),'13'	=>	t('13'),'14'	=>	t('14'),'15'	=>	t('15'),'16'	=>	t('16'),);
$options_grid_24 = array('0'		=>	t('0'),'1'		=>	t('1'),'2'		=>	t('2'),'3'		=>	t('3'),'4'		=>	t('4'),'5'		=>	t('5'),'6'		=>	t('6'),'7'		=>	t('7'),'8'		=>	t('8'),'9'		=>	t('9'),'10'	=>	t('10'),'11'	=>	t('11'),'12'	=>	t('12'),'13'	=>	t('13'),'14'	=>	t('14'),'15'	=>	t('15'),'16'	=>	t('16'),'17'	=>	t('17'),'18'	=>	t('18'),'19'	=>	t('19'),'20'	=>	t('20'),'21'	=>	t('21'),'22'	=>	t('22'),'23'	=>	t('23'),'24'	=>	t('24'),);
$show_i_global = array();
$show_i_global = array(':input[name="container_grid_global"]' => array('checked' => FALSE), ':input[name="layout_type"]' => array('value' => 'grid'),);

/**
 * Form for theme-settings
 *
 */
	
$form['basic-settings'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Trotoar basic settings',
	'#weight'		=>	-10,
	'#description'	=>	t('You can edit Trotoar basic settings in here.'),
);

$form['basic-settings']['settings-tabs'] = array(
	'#type'			=>	'vertical_tabs',
);


	
$form['layout-settings'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Trotoar layout settings',
	'#weight'		=>	-9,
	'#description'	=>	t('You can edit Trotoar layout settings in here.'),
);

$form['layout-settings']['settings-tabs'] = array(
	'#type'			=>	'vertical_tabs',
);

/**
 * Breadcrumb
 *
 */

$form['basic-settings']['settings-tabs']['adv_tgl'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Advanced Toggle',
);

$form['basic-settings']['settings-tabs']['adv_tgl']['br_set_value'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show breadcrumb.',
	'#default_value'=>	theme_get_setting('br_set_value'),
);

$form['basic-settings']['settings-tabs']['adv_tgl']['br_set_sprt'] = array(
	'#type'			=>	'textfield',
	'#title'		=>	'Breadcrumb separator',
	'#description'	=>	t('<em>Don&#39t forget to add space in prefix and suffix</em>'),
	'#default_value'=>	theme_get_setting('br_set_sprt'),
	'#states'		=>	array(
		'visible'	=>	array(
			':input[name="br_set_value"]' => array('checked' => TRUE),),),
);

$form['basic-settings']['settings-tabs']['adv_tgl']['br_set_home'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show home link in breadcrumb',
	'#default_value'=>	theme_get_setting('br_set_home'),
	'#states'		=>	array(
		'visible'	=>	array(
			':input[name="br_set_value"]' => array('checked' => TRUE),),),
);

$form['basic-settings']['settings-tabs']['adv_tgl']['msg_show'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show Messages',
	'#default_value'=>	theme_get_setting('msg_show'),
);

$form['basic-settings']['settings-tabs']['adv_tgl']['title_show'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show Page Title',
	'#default_value'=>	theme_get_setting('title_show'),
);

$form['basic-settings']['settings-tabs']['adv_tgl']['tabs_show'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show Tabs',
	'#default_value'=>	theme_get_setting('tabs_show'),
);

$form['basic-settings']['settings-tabs']['adv_tgl']['action_links_show'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show Action Links',
	'#default_value'=>	theme_get_setting('action_links_show'),
);


/**
 * Debug
 *
 */

$form['basic-settings']['settings-tabs']['debug'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Debug',
);

$form['basic-settings']['settings-tabs']['debug']['hover'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to use blue hover.',
	'#default_value'=>	theme_get_setting('hover'),
);

$form['basic-settings']['settings-tabs']['debug']['container_grid_show_png'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Check to show guide line.<br/>(Just show when you in 960gs or fixed width.)',
	'#default_value'=>	theme_get_setting('container_grid_show_png'),
);

$form['basic-settings']['settings-tabs']['debug']['responsive_respond'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add Respond.js JavaScript to add basic CSS3 media query support to IE 6-8. '),
    '#default_value' => theme_get_setting('responsive_respond'),
);
  
$form['basic-settings']['settings-tabs']['debug']['responsive_html5'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add HTML5 shim JavaScript to add support to IE 6-8. '),
    '#default_value' => theme_get_setting('responsive_html5'),
);
  
$form['basic-settings']['settings-tabs']['debug']['responsive_meta'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add meta tags to support responsive design on mobile devices.'),
    '#default_value' => theme_get_setting('responsive_meta'),
);

/**
 * Form for Layout
 *
 */

$form['layout-settings']['settings-tabs']['layout'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Layout',
	'#weight'		=> 	-20,
);

$form['layout-settings']['settings-tabs']['layout']['layout_type'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Type of layout',
	'#weight'		=> 	-12,
	'#description'	=>	t('<em>Choose your type of layout</em>'),
	'#default_value'=>	theme_get_setting('layout_type'),
	'#options'		=>	array(
		'grid'	=>	t('Grid 960gs'),
		'fluid'	=>	t('Fluid'),)
);

$form['layout-settings']['settings-tabs']['layout']['container_grid'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Global Section',
	'#description'	=>	t('<em>Grid column will you choose for Global section.</em>'),
	'#default_value'=>	theme_get_setting('container_grid'),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$form['layout-settings']['settings-tabs']['layout']['container_grid_global'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Use Global comparison column',
	'#description'	=>	t('<em>Check to use global comparison grid column.<br/>(save configuration first to get the effect)</em>'),
	'#default_value'=>	theme_get_setting('container_grid_global'),
);


/**
 * Form for header regions
 *
 */

$form['layout-settings']['settings-tabs']['header'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Header',
	'#weight'		=>	-7,
	'#description'	=>	'Configure the Header Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['header']['hdr_i'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Total regions',
	'#description'	=>	t('<em>How many regions will you take in Header Regions</em>'),
	'#default_value'=>	theme_get_setting('hdr_i'),
	'#options'		=>	array(
		'0'	=>	t('0'),
		'1'	=>	t('1'),
		'2'	=>	t('2'),
		'3'	=>	t('3'),
		'4'	=>	t('4'),
		'5'	=>	t('5'),)
);

$form['layout-settings']['settings-tabs']['header']['hdr_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of header.</em>'),
	'#default_value'=>	theme_get_setting('hdr_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);

$form['layout-settings']['settings-tabs']['header']['hdr_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('hdr_weight'),
	'#options'		=>	$options_weight,
);

for ($i = 1; $i <= 5 ; $i++){

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . ''] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Header-' . $i . '',
	'#description'	=>	'Configure the Header-' . $i . ' Regions',
    '#collapsible'  => 	TRUE,
    '#collapsed'    => 	TRUE,
	'#dependency'	=> 	array(':input[name="hdr_i"]' => array('' . $i+0 . '','' . $i+1 . '','' . $i+2 . '','' . $i+3 . '','' . $i+4 . ''),),
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('hdr_grid_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="hdr_grid_' . $i . '"]';

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_12_' . $i . '']['hdr_grid_12_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('hdr_grid_12_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_12_' . $i . '']['hdr_grid_12_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_12_' . $i . '']['hdr_grid_12_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_16_' . $i . '']['hdr_grid_16_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('hdr_grid_16_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_16_' . $i . '']['hdr_grid_16_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_16_' . $i . '']['hdr_grid_16_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_24_' . $i . '']['hdr_grid_24_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('hdr_grid_24_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_24_' . $i . '']['hdr_grid_24_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['header']['hdr_' . $i . '']['hdr_grid_24_' . $i . '']['hdr_grid_24_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
}

/**
 * Form for Branding regions
 *
 */


$form['layout-settings']['settings-tabs']['branding'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Branding',
	'#weight'		=>	-10,
	'#description'	=>	'Configure the Branding Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['branding']['brd_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of Branding.</em>'),
	'#default_value'=>	theme_get_setting('brd_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);

$form['layout-settings']['settings-tabs']['branding']['brd_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('brd_weight'),
	'#options'		=>	$options_weight,
);


$form['layout-settings']['settings-tabs']['branding']['brd_grid'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('brd_grid'),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="brd_grid"]';

$form['layout-settings']['settings-tabs']['branding']['brd_grid_12']['brd_grid_12_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('brd_grid_12_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['branding']['brd_grid_12']['brd_grid_12_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['branding']['brd_grid_12']['brd_grid_12_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['branding']['brd_grid_16']['brd_grid_16_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('brd_grid_16_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['branding']['brd_grid_16']['brd_grid_16_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['branding']['brd_grid_16']['brd_grid_16_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['branding']['brd_grid_24']['brd_grid_24_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('brd_grid_24_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['branding']['brd_grid_24']['brd_grid_24_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['branding']['brd_grid_24']['brd_grid_24_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

/**
 * Form for Menu regions
 *
 */


$form['layout-settings']['settings-tabs']['menu'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Menu',
	'#weight'		=>	-9,
	'#description'	=>	'Configure the Menu Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['menu']['menu_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of Menu.</em>'),
	'#default_value'=>	theme_get_setting('menu_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);

$form['layout-settings']['settings-tabs']['menu']['menu_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('menu_weight'),
	'#options'		=>	$options_weight,
);


$form['layout-settings']['settings-tabs']['menu']['menu_grid'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('menu_grid'),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="menu_grid"]';

$form['layout-settings']['settings-tabs']['menu']['menu_grid_12']['menu_grid_12_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('menu_grid_12_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['menu']['menu_grid_12']['menu_grid_12_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['menu']['menu_grid_12']['menu_grid_12_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['menu']['menu_grid_16']['menu_grid_16_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('menu_grid_16_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['menu']['menu_grid_16']['menu_grid_16_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['menu']['menu_grid_16']['menu_grid_16_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['menu']['menu_grid_24']['menu_grid_24_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('menu_grid_24_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['menu']['menu_grid_24']['menu_grid_24_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['menu']['menu_grid_24']['menu_grid_24_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

/**
 * Form for Preface regions
 *
 */


$form['layout-settings']['settings-tabs']['preface'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Preface',
	'#weight'		=>	0,
	'#description'	=>	'Configure the Preface Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['preface']['preface_i'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Total regions',
	'#description'	=>	t('<em>How many regions will you take in Preface Regions</em>'),
	'#default_value'=>	theme_get_setting('preface_i'),
	'#options'		=>	array(
		'0'	=>	t('0'),
		'1'	=>	t('1'),
		'2'	=>	t('2'),
		'3'	=>	t('3'),
		'4'	=>	t('4'),
		'5'	=>	t('5'),)
);

$form['layout-settings']['settings-tabs']['preface']['preface_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of Preface.</em>'),
	'#default_value'=>	theme_get_setting('preface_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);

$form['layout-settings']['settings-tabs']['preface']['preface_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('preface_weight'),
	'#options'		=>	$options_weight,
);


for ($i = 1; $i <= 5 ; $i++){

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . ''] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'preface-' . $i . '',
	'#description'	=>	'Configure the preface-' . $i . ' Regions',
    '#collapsible'  => 	TRUE,
    '#collapsed'    => 	TRUE,
	'#dependency'	=> 	array(':input[name="preface_i"]' => array('' . $i+0 . '','' . $i+1 . '','' . $i+2 . '','' . $i+3 . '','' . $i+4 . ''),),
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('preface_grid_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="preface_grid_' . $i . '"]';

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_12_' . $i . '']['preface_grid_12_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('preface_grid_12_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_12_' . $i . '']['preface_grid_12_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_12_' . $i . '']['preface_grid_12_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_16_' . $i . '']['preface_grid_16_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('preface_grid_16_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_16_' . $i . '']['preface_grid_16_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_16_' . $i . '']['preface_grid_16_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_24_' . $i . '']['preface_grid_24_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('preface_grid_24_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_24_' . $i . '']['preface_grid_24_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['preface']['preface_' . $i . '']['preface_grid_24_' . $i . '']['preface_grid_24_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
}

/**
 * Form for bottom teaser regions
 *
 */


$form['layout-settings']['settings-tabs']['teaser'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Bottom-teaser',
	'#weight'		=>	4,
	'#description'	=>	'Configure the Bottom teaser Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_i'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Total regions',
	'#description'	=>	t('<em>How many regions will you take in Bottom teaser Regions</em>'),
	'#default_value'=>	theme_get_setting('teaser_i'),
	'#options'		=>	array(
		'0'	=>	t('0'),
		'1'	=>	t('1'),
		'2'	=>	t('2'),
		'3'	=>	t('3'),
		'4'	=>	t('4'),
		'5'	=>	t('5'),)
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of Bottom teaser.</em>'),
	'#default_value'=>	theme_get_setting('teaser_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('teaser_weight'),
	'#options'		=>	$options_weight,
);


for ($i = 1; $i <= 5 ; $i++){

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . ''] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Bottom teaser-' . $i . '',
	'#description'	=>	'Configure the Bottom teaser-' . $i . ' Regions',
    '#collapsible'  => 	TRUE,
    '#collapsed'    => 	TRUE,
	'#dependency'	=> 	array(':input[name="teaser_i"]' => array('' . $i+0 . '','' . $i+1 . '','' . $i+2 . '','' . $i+3 . '','' . $i+4 . ''),),
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('teaser_grid_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="teaser_grid_' . $i . '"]';

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_12_' . $i . '']['teaser_grid_12_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('teaser_grid_12_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_12_' . $i . '']['teaser_grid_12_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_12_' . $i . '']['teaser_grid_12_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_16_' . $i . '']['teaser_grid_16_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('teaser_grid_16_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_16_' . $i . '']['teaser_grid_16_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_16_' . $i . '']['teaser_grid_16_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_24_' . $i . '']['teaser_grid_24_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('teaser_grid_24_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_24_' . $i . '']['teaser_grid_24_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['teaser']['teaser_' . $i . '']['teaser_grid_24_' . $i . '']['teaser_grid_24_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
}

/**
 * Content-wrapper regions
 *
 */
  
$form['layout-settings']['settings-tabs']['content'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Content',
	'#weight'		=>	1,
	'#description'	=>	'Configure the Content Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['content']['content_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of Content.</em>'),
	'#default_value'=>	theme_get_setting('content_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);


$form['layout-settings']['settings-tabs']['content']['ctn_pos'] = array(
	'#type'			=>	'radios',
	'#title'		=>	'Layout content',
	'#weight'		=> 	-11,
	'#description'	=>	t('<em>Choose your layout of content</em>'),
	'#default_value'=>	theme_get_setting('ctn_pos'),
	'#options'		=>	array(
		'left'		=>	t('Left'),
		'center'	=>	t('Center'),
		'right'		=>	t('Right'),)
);




$form['layout-settings']['settings-tabs']['content']['content_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('content_weight'),
	'#options'		=>	$options_weight,
);

/*--------- sidebar first--------------------------------------------------------------------------*/
$form['layout-settings']['settings-tabs']['content']['sdf'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Sidebar first',
	'#weight'		=>	5,
	'#description'	=>	'Configure the Sidebar first Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_show'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Show this regions',
	'#default_value'=>	theme_get_setting('sdf_show'),
	'#weight'		=>	-11,
	'#description'	=>	'Check to show this regions',
);


$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('sdf_grid'),
	'#states'		=>	array(
		'visible'	=>	array(
			':input[name="container_grid_global"]' => array('checked' => FALSE),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="sdf_grid"]';

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_12']['sdf_grid_12_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('sdf_grid_12_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_12']['sdf_grid_12_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_12']['sdf_grid_12_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_16']['sdf_grid_16_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('sdf_grid_16_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_16']['sdf_grid_16_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_16']['sdf_grid_16_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_24']['sdf_grid_24_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('sdf_grid_24_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_24']['sdf_grid_24_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['content']['sdf']['sdf_grid_24']['sdf_grid_24_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),
			':input[name="sdf_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_24,
);


/*--------- main content --------------------------------------------------------------------------*/
$form['layout-settings']['settings-tabs']['content']['ctn'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Main content',
	'#weight'		=>	2,
	'#description'	=>	'Configure the Main ontent Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);
$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('ctn_grid'),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="ctn_grid"]';

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_12']['ctn_grid_12_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('ctn_grid_12_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_12']['ctn_grid_12_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_12']['ctn_grid_12_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_16']['ctn_grid_16_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('ctn_grid_16_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_16']['ctn_grid_16_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_16']['ctn_grid_16_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_24']['ctn_grid_24_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('ctn_grid_24_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_24']['ctn_grid_24_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['content']['ctn']['ctn_grid_24']['ctn_grid_24_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);


/*--------- sidebar second --------------------------------------------------------------------------*/
$form['layout-settings']['settings-tabs']['content']['sds'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Sidebar second',
	'#weight'		=>	7,
	'#description'	=>	'Configure the Sidebar second Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_show'] = array(
	'#type'			=>	'checkbox',
	'#title'		=>	'Show this regions',
	'#default_value'=>	theme_get_setting('sds_show'),
	'#weight'		=>	-11,
	'#description'	=>	'Check to show this regions',
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('sds_grid'),
	'#states'		=>	array(
		'visible'	=>	array(
			':input[name="container_grid_global"]' => array('checked' => FALSE),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="sds_grid"]';

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_12']['sds_grid_12_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('sds_grid_12_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_12']['sds_grid_12_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_12']['sds_grid_12_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_16']['sds_grid_16_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('sds_grid_16_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_16']['sds_grid_16_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_16']['sds_grid_16_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_24']['sds_grid_24_width'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('sds_grid_24_width'),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_24']['sds_grid_24_prefix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['content']['sds']['sds_grid_24']['sds_grid_24_suffix'] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),
			':input[name="sds_show"]' => array('checked' => TRUE),),),
	'#options'		=>	$options_grid_24,
);




/**
 * Footer regions
 *
 */
 
$form['layout-settings']['settings-tabs']['footer'] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'Footer',
	'#weight'		=>	6,
	'#description'	=>	'Configure the Footer Regions',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_i'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Total regions',
	'#description'	=>	t('<em>How many regions will you take in Footer Regions</em>'),
	'#default_value'=>	theme_get_setting('ftr_i'),
	'#options'		=>	array(
		'0'	=>	t('0'),
		'1'	=>	t('1'),
		'2'	=>	t('2'),
		'3'	=>	t('3'),
		'4'	=>	t('4'),
		'5'	=>	t('5'),)
);

$form['layout-settings']['settings-tabs']['footer']['ftr_pos'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Position',
	'#description'	=>	t('<em>Where will you put the position of Footer.</em>'),
	'#default_value'=>	theme_get_setting('ftr_pos'),
	'#options'		=>	array(
		'top'	=>	t('Top'),
		'middle'=>	t('Middle'),
		'bottom'=>	t('Bottom'),)
);

$form['layout-settings']['settings-tabs']['footer']['ftr_weight'] = array(
	'#type'			=>	'select',
	'#title'		=>	'Weight',
	'#default_value'=>	theme_get_setting('ftr_weight'),
	'#options'		=>	$options_weight,
);


for ($i = 1; $i <= 5 ; $i++){

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . ''] = array(
	'#type'			=>	'fieldset',
	'#title'		=>	'footer-' . $i . '',
	'#description'	=>	'Configure the footer-' . $i . ' Regions',
    '#collapsible'  => 	TRUE,
    '#collapsed'    => 	TRUE,
	'#dependency'	=> 	array(':input[name="ftr_i"]' => array('' . $i+0 . '','' . $i+1 . '','' . $i+2 . '','' . $i+3 . '','' . $i+4 . ''),),
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Set column',
	'#description'	=>	t('<em>Grid column will you choose.</em>'),
	'#default_value'=>	theme_get_setting('ftr_grid_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	$show_i_global),
	'#options'		=>	array(
		'12'		=>	t('12 column'),
		'16'		=>	t('16 column'),
		'24'		=>	t('24 column'),)
);

$show_grid_global = (theme_get_setting('container_grid_global') == 1) ? ':input[name="container_grid"]' : ':input[name="ftr_grid_' . $i . '"]';

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_12_' . $i . '']['ftr_grid_12_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 12 column)</em>'),
	'#default_value'=>	theme_get_setting('ftr_grid_12_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_12_' . $i . '']['ftr_grid_12_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_12_' . $i . '']['ftr_grid_12_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 12 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '12'),),),
	'#options'		=>	$options_grid_12,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_16_' . $i . '']['ftr_grid_16_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 16 column)</em>'),
	'#default_value'=>	theme_get_setting('ftr_grid_16_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_16_' . $i . '']['ftr_grid_16_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_16_' . $i . '']['ftr_grid_16_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 16 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '16'),),),
	'#options'		=>	$options_grid_16,
);


$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_24_' . $i . '']['ftr_grid_24_width_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Width',
	'#description'	=>	t('<em>Width of this regions (in 24 column)</em>'),
	'#default_value'=>	theme_get_setting('ftr_grid_24_width_' . $i . ''),
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);

$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_24_' . $i . '']['ftr_grid_24_prefix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'Prefix',
	'#description'	=>	t('<em>Prefix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
$form['layout-settings']['settings-tabs']['footer']['ftr_' . $i . '']['ftr_grid_24_' . $i . '']['ftr_grid_24_suffix_' . $i . ''] = array(
	'#type'			=>	'select',
	'#title'		=>	'suffix',
	'#description'	=>	t('<em>suffix of this regions (in 24 column)</em>'),
	'#default_value'=>	'0',
	'#states'		=>	array(
		'visible'	=>	array(
			$show_grid_global => array('value' => '24'),),),
	'#options'		=>	$options_grid_24,
);
}
