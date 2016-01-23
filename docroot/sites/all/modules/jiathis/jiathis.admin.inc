<?php

/**
 * @file
 * Administration page for the JiaThis Share Button
 */

/**
 * Menu callback: displays the JiaThis Share Button module settings page.
 *
 * @ingroup forms
 */
function jiathis_page_admin_settings() {
	$script = '';

  $locations = array();
  $entity_info = entity_get_info('node');
  $view_modes = $entity_info['view modes'];
  foreach ($view_modes as $name => $info) {
    $locations[$name] = $info['label'];
  }

	$form['display'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Display Settings'),
	    '#description' => t('These are settings for nodes: the button will be created dynamically for each node using its URL. <br />On the othesr hand, if you only need a block with a fixed URL like your homepage, go to the <a href="@blocks">blocks</a> page to enable and configure the <em>JiaThis Share Button</em> block.', array('@blocks' => url('admin/build/block'))),
	  );

	$form['display']['jiathis_node_types'] = array(
	    '#type' => 'checkboxes',
	    '#title' => t('Display the button on these content types:'),
	    '#options' => node_type_get_names(),
	    '#default_value' => variable_get('jiathis_node_types', array()),
	  );

	  $form['display']['jiathis_node_location'] = array(
	    '#type' => 'checkboxes',
	    '#title' => t('Display the button in these view modes:'),
	    '#options' => $locations + array('link' => t('Node links')),
	    '#default_value' => variable_get('jiathis_node_location', array('full')),
	  );

	  $form['display']['jiathis_weight'] = array(
	     '#type' => 'weight',
	     '#title' => t('Weight'),
	     '#delta' => 50,
	     '#default_value' => variable_get('jiathis_weight', '-10'),
	     '#description' => t('Heavier items will sink. The default weight -10 will show the button at the top of the node content.'),
	  );
	  $available_sizes = array(
	  	'4'	=> t('Tiny'),
	    '3' => t('Small'),
	    '2' => t('Medium'),
	    '1' => t('Large'),
	  );
	  $available_styles = array(
	    '0' => t('Standard'),
	    '1' => t('Mini'),
	  );
	  $default = array(
			'iconstyle' => 0,
	    'count' => 0,
	    'size' => 1,  // standard
	    'css' => 'margin: 0 1em 1em 1em;float:right',
	  );

	  $button_settings = variable_get('jiathis_button_settings', $default);

	  $form['jiathis_button_settings'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Button Settings'),
	    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
	  );
	  $form['jiathis_button_settings']['iconstyle'] = array(
	    '#type' => 'radios',
	    '#title' => t('Style'),
	    '#options' => $available_styles,
	    '#default_value' => $button_settings['iconstyle'],
		  );
	  $form['jiathis_button_settings']['count'] = array(
	    '#type' => 'radios',
	    '#title' => t('Include count?'),
	    '#options' => array(t('No'), t('Yes')),
	    '#default_value' => $button_settings['count'],
	  );
	  $form['jiathis_button_settings']['size'] = array(
	    '#type' => 'radios',
	    '#title' => t('Size'),
	    '#options' => $available_sizes,
	    '#default_value' => $button_settings['size'],
	  );
	  $form['jiathis_button_settings']['css'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Optional wrapper with CSS'),
	    '#maxlength' => 256,
	    '#default_value' => $button_settings['css'],
	    '#description' => t('To help with the layout and placement of the button, it will be rendered by default inside a wrapper: <br />	&lt;div class="wb_sharebutton-wrapper"&gt;	&lt;/div&gt;<br/>You can enter CSS rules to style this wrapper. By default <em>margin: 0 1em 1em 1em;float:right</em><br />To disable totally the wrapper, input the word <em>nowrapper</em>'),
	  );
	  return system_settings_form($form);
}



function jiathis_block_admin_settings() {

	$form['display'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Display Settings'),
	    '#description' => t('These are settings for the block. To use the block, go to the <a href="@blocks">blocks</a> page to enable and configure the <em>JiaThis Share Button</em> block.', array('@blocks' => url('admin/structure/block'))),
	  );

	  $available_sizes = array(
	  	'4'	=> t('Tiny'),
	    '3' => t('Small'),
	    '2' => t('Medium'),
	    '1' => t('Large'),

	  );
	  $available_styles = array(
	    '0' => t('Standard'),
	    '1' => t('Mini'),
	  );

	  $default = array(
			'iconstyle_block' => 0,
	    'count_block' => 0,
	    'size_block' => 1,  // standard
	    'css_block' => 'margin: 0 1em 1em 1em;float:right',
	  );

	  $button_settings = variable_get('jiathis_block_button_settings', $default);

	  $form['jiathis_block_button_settings'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Button Settings'),
	    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
	  );
	  $form['jiathis_block_button_settings']['iconstyle_block'] = array(
	    '#type' => 'radios',
	    '#title' => t('Style'),
	    '#options' => $available_styles,
	    '#default_value' => $button_settings['iconstyle_block'],
		  );
	  $form['jiathis_block_button_settings']['count_block'] = array(
	    '#type' => 'radios',
	    '#title' => t('Include count?'),
	    '#options' => array(t('No'), t('Yes')),
	    '#default_value' => $button_settings['count_block'],
	  );
	  $form['jiathis_block_button_settings']['size_block'] = array(
	    '#type' => 'radios',
	    '#title' => t('Size'),
	    '#options' => $available_sizes,
	    '#default_value' => $button_settings['size_block'],
	  );
	  $form['jiathis_block_button_settings']['css_block'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Optional wrapper with CSS'),
	    '#maxlength' => 256,
	    '#default_value' => $button_settings['css_block'],
	    '#description' => t('To help with the layout and placement of the button, it will be rendered by default inside a wrapper: <br />	&lt;div class="wb_sharebutton-wrapper"&gt;	&lt;/div&gt;<br/>You can enter CSS rules to style this wrapper. By default <em>margin: 0 1em 1em 1em;float:right</em><br />To disable totally the wrapper, input the word <em>nowrapper</em>'),
	  );
	  return system_settings_form($form);
}



/*
 * Trim and polish text input fields
*/
function jiathis_page_admin_settings_validate($form, &$form_state) {
	  $form_state['values']['jiathis_button_settings']['css'] = jiathis_trim($form_state['values']['jiathis_button_settings']['css'], ';');
	}

/**
 * Helper function to trim whitespace and make sure to remove
 * the last character if it is a delimiter.
 *
 * @param $list
 *   A string representing a list of items
 * @param $delimiter
 *   A string that contains a delimiter.
 * @return
 *   A whitespace-trimmed string
 *
*/
function jiathis_trim($list, $delimiter) {
	$list = trim(check_plain($list));
	  if (!empty($list)) {
	    if (drupal_substr($list, -1) === $delimiter) {
	      $list = drupal_substr($list, 0, -1);
	    }
	  }
	return $list;
}

