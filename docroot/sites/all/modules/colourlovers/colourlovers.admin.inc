<?php

/**
 * Page callback: Colourlovers Settings
 *
 * @see colourlovers_menu()
 */
function colourlovers_admin() {
  $form = array();

  $form['colourlovers_description'] = array(
    '#type' => 'fieldset',
		'#weight' => 0,
		'#collapsible' => FALSE, 
		'#collapsed' => FALSE,
  );
  $form['colourlovers_description']['colourlovers_description_text'] = array(
  '#markup' => '<h2>Welcome to the COLOURlovers API</h2><br /><p>This form is simply a playground for the API to show you what it&#39;s capable of, you can then implement it in your own custom modules by adding it as a dependency and calling the appropriate functions.<br /><br />All documentation can be found at: <strong><a href="http://www.colourlovers.com/api" target="_blank">http://www.colourlovers.com/api</a></strong></p>',
	);

  $form['colourlovers_type'] = array(
    '#type' => 'select',
    '#title' => t('Colourlovers Type'),
    '#default_value' => variable_get('colourlovers_type', ''),
    '#options' => array(
    	'colors' => t('Colors'),
    	'palettes' => t('Palettes'),
    	'patterns' => t('Patterns'),
    	//'Lover' => t('Lover'), // Lover really isn't applicable, but maybe we can do something with it later.
    ),
    '#description' => t('Select what kind of information you want to query. Color, Palette, or Pattern.'),
    '#required' => TRUE,
  );

  $form['colourlovers_mode'] = array(
    '#type' => 'select',
    '#title' => t('Colourlovers Mode'),
    '#default_value' => variable_get('colourlovers_mode', FALSE),
     '#options' => array(
    	'default' => t('Default'),
    	'top' => t('Top'),
    	'new' => t('New'),
    	'random' => t('Random'),
    ),
    '#description' => t('The mode to retrieve colors. Regular, New, Top, and Random. !note Random does not have settings.', array('!note' => '<br /><strong>'.t('NOTE').':</strong>')),
    '#required' => TRUE,
  );

  // Color information.
  $form['colourlovers_color'] = array(
    '#type' => 'fieldset',
		'#title' => t('Color Options'), 
		'#weight' => 1,
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
    '#states' => array(
			'visible' => array(
				':input[name="colourlovers_type"]' => array('value' => 'colors'),
			),
		),
  );
    $form['color_info'] = array(
    '#type' => 'fieldset',
    '#group' => 'colourlovers_color',
		'#title' => t('Example Parameters'),
		'#weight' => -1,
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
  );
  $form['color_info']['info'] = array(
  	'#markup' => '<table><tr><th>Parameter</th><th>&nbsp;</th><th>Value</th></tr><tr><td>lover</td><td style="width: 20px;">=</td><td>COLOURlover username</td></tr><tr><td>keywords</td><td style="width: 20px;">=</td><td>search+term</td></tr><tr><td>keywordExact</td><td style="width: 20px;">=</td><td>0 or 1 [Perform an exact search for the keywords passed. Default 0]</td></tr><tr><td>orderCol</td><td style="width: 20px;">=</td><td>X [Where X can be: dateCreated, score, name, numVotes, or numViews]</td></tr><tr><td>sortBy</td><td style="width: 20px;">=</td><td>X [Where X can be: ASC or DESC. Default ASC]</td></tr><tr><td>numResults</td><td style="width: 20px;">=</td><td>20 [Number of results to return, maximum of 100. Default 20]</td></tr><tr><td>resultOffset</td><td style="width: 20px;">=</td><td>5   [Result offset, for paging. Default 0]</td></tr><tr><td colspan=3><strong>Requests with <code>orderCol</code> defined will override any <code>/new</code> or <code>/top</code> switch</strong></td></tr></table>',
  );
  $form['colourlovers_color']['colourlovers_colors_lover'] = array(
		'#type' => 'textfield',
		'#title' => t('Lover'),
		'#description' => t('COLOURlover username'),
		'#default_value' => variable_get('colourlovers_colors_lover'),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_color']['colourlovers_colors_keywords'] = array(
		'#type' => 'textfield',
		'#title' => t('Keywords'),
		'#description' => t('Search for color by keywords. Separate terms by commas.'),
		'#default_value' => variable_get('colourlovers_colors_keywords', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_color']['colourlovers_colors_keywordexact'] = array(
		'#type' => 'select',
		'#title' => t('Exact keyword matching'),
		'#description' => t('Match keywords exactly, yes or no.'),
		'#default_value' => variable_get('colourlovers_colors_keywordexact', ''),
		'#options' => array(
			0 => t('No'),
			1 => t('Yes'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_color']['colourlovers_colors_ordercol'] = array(
		'#type' => 'select',
		'#title' => t('Order by column'),
		'#description' => t('Can be: dateCreated, score, name, numVotes, or numViews.'),
		'#default_value' => variable_get('colourlovers_colors_ordercol', FALSE),
		'#options' => array(
			'' => '',
			'dateCreated' => t('Date Created'),
			'score' => t('Score'),
			'name' => t('Name'),
			'numVotes' => t('Number of Votes'),
			'numViews' => t('Number of Views'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_color']['colourlovers_colors_sortby'] = array(
		'#type' => 'select',
		'#title' => t('Sort By'),
		'#description' => t('Can be ascending or descending. Default: ASC'),
		'#default_value' => variable_get('colourlovers_colors_sortby', 'ASC'),
		'#options' => array(
			'ASC' => t('Ascending'),
			'DESC' => t('Descending'),
		),
		'#states' => array(
			'invisible' => array(
				':input[name="colourlovers_colors_ordercol"]' => array('value' => ''),
			),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_color']['colourlovers_colors_numresults'] = array(
		'#type' => 'select',
		'#title' => t('Number of Results'),
		'#description' => t('Maximum to 100. Default: 20'),
		'#default_value' => variable_get('colourlovers_colors_numresults', 20),
		'#options' => drupal_map_assoc(range(1, 100)),
		'#required' => FALSE,
  );
  $form['colourlovers_color']['colourlovers_colors_resultoffset'] = array(
		'#type' => 'textfield',
		'#title' => t('Result Offset'),
		'#description' => t('Offset results for paging. Default: 0'),
		'#default_value' => variable_get('colourlovers_colors_resultoffset', 0),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );

  // Palette information.
  $form['colourlovers_palette'] = array(
    '#type' => 'fieldset',
		'#title' => t('Palette Options'), 
		'#weight' => 1,
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
    '#states' => array(
			'visible' => array(
				':input[name="colourlovers_type"]' => array('value' => 'palettes'),
			),
		),
  );
  $form['palette_info'] = array(
    '#type' => 'fieldset',
    '#group' => 'colourlovers_palette',
		'#title' => t('Example Parameters'),
		'#weight' => -1,
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
  );
  $form['palette_info']['info'] = array(
  	'#markup' => '<table><tr><th>Parameter</th><th>&nbsp;</th><th>Value</th></tr><tr><td>lover</td><td style="width: 20px;">=</td><td>COLOURlover username</td></tr><tr><td>hex</td><td style="width: 20px;">=</td><td><code>00FF33</code>  or  <code>00FF33,CC00FF,DD0033,003333,0033FF</code><br />[Possible Values: any valid 6-char hex value or list of up to five (5) hex values, separated by commas]</td></tr><tr><td>hex_logic</td><td style="width: 20px;">=</td><td>AND  or  OR<br />Sets the comparison logic for the hex list. Passing AND will find palettes with all provided hex values, passing OR will find palettes withany of the provided hex values.<br />[Possible Values: AND or OR. Default AND]</td></tr><tr><td>keywords</td><td style="width: 20px;">=</td><td>search+term</td></tr><tr><td>keywordExact</td><td style="width: 20px;">=</td><td>0 or 1 [Perform an exact search for the keywords passed. Default 0]</td></tr><tr><td>orderCol</td><td style="width: 20px;">=</td><td>X [Where X can be: dateCreated, score, name, numVotes, or numViews]</td></tr><tr><td>sortBy</td><td style="width: 20px;">=</td><td>X [Where X can be: ASC or DESC. Default ASC]</td></tr><tr><td>numResults</td><td style="width: 20px;">=</td><td>20 [Number of results to return, maximum of 100. Default 20]</td></tr><tr><td>resultOffset</td><td style="width: 20px;">=</td><td>5   [Result offset, for paging. Default 0]</td></tr><tr><td>showPaletteWidths</td><td style="width: 20px;">=</td><td>0 or 1 [Shows palette\'s color\'s widths. Default 0]</td></tr><tr><td colspan=3><strong>Requests with <code>orderCol</code> defined will override any <code>/new</code> or <code>/top</code> switch</strong></td></tr></table>',
  );
  $form['colourlovers_palette']['colourlovers_palettes_lover'] = array(
		'#type' => 'textfield',
		'#title' => t('Lover'),
		'#description' => t('COLOURlover username'),
		'#default_value' => variable_get('colourlovers_palettes_lover', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_hex'] = array(
		'#type' => 'textfield',
		'#title' => t('Hex values'),
		'#description' => t('Possible Values: any valid 6-char hex value or list of up to five (5) hex values, separated by commas.').'<br />00FF33  or  00FF33,CC00FF,DD0033,003333,0033FF',
		'#default_value' => variable_get('colourlovers_palettes_hex', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_hexlogic'] = array(
		'#type' => 'select',
		'#title' => t('Hex Logic'),
		'#description' => t('AND or OR. Passing AND will find palette with ALL provided hex values, OR will find palette with any provided value. Default: AND'),
		'#default_value' => variable_get('colourlovers_palettes_hexlogic', 'AND'),
		'#options' => array(
			'and' => t('AND'),
			'or' => t('OR'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_keywords'] = array(
		'#type' => 'textfield',
		'#title' => t('Keywords'),
		'#description' => t('Search for palettes by keywords. Separate terms by commas.'),
		'#default_value' => variable_get('colourlovers_palettes_keywords', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_keywordexact'] = array(
		'#type' => 'select',
		'#title' => t('Exact keyword matching'),
		'#description' => t('Match keywords exactly, yes or no.'),
		'#default_value' => variable_get('colourlovers_palettes_keywordexact', FALSE),
		'#options' => array(
			0 => t('No'),
			1 => t('Yes'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_ordercol'] = array(
		'#type' => 'select',
		'#title' => t('Order by column'),
		'#description' => t('Can be: dateCreated, score, name, numVotes, or numViews.'),
		'#default_value' => variable_get('colourlovers_palettes_ordercol', FALSE),
		'#options' => array(
			'' => '',
			'dateCreated' => t('Date Created'),
			'score' => t('Score'),
			'name' => t('Name'),
			'numVotes' => t('Number of Votes'),
			'numViews' => t('Number of Views'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_sortby'] = array(
		'#type' => 'select',
		'#title' => t('Sort By'),
		'#description' => t('Can be ascending or descending. Default: ASC'),
		'#default_value' => variable_get('colourlovers_palettes_sortby', 'ASC'),
		'#options' => array(
			'ASC' => t('Ascending'),
			'DESC' => t('Descending'),
		),
		'#states' => array(
			'invisible' => array(
				':input[name="colourlovers_palettes_ordercol"]' => array('value' => ''),
			),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_numresults'] = array(
		'#type' => 'select',
		'#title' => t('Number of Results'),
		'#description' => t('Maximum to 100. Default: 20'),
		'#default_value' => variable_get('colourlovers_palettes_numresults', 20),
		'#options' => drupal_map_assoc(range(1, 100)),
		'#required' => FALSE,
  );
  $form['colourlovers_palette']['colourlovers_palettes_resultoffset'] = array(
		'#type' => 'textfield',
		'#title' => t('Result Offset'),
		'#description' => t('Offset results for paging. Default: 0'),
		'#default_value' => variable_get('colourlovers_palettes_resultoffset', 0),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );

  // Pattern information.
  $form['colourlovers_pattern'] = array(
    '#type' => 'fieldset',
		'#title' => t('Pattern Options'), 
		'#weight' => 1,
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
    '#states' => array(
			'visible' => array(
				':input[name="colourlovers_type"]' => array('value' => 'patterns'),
			),
		),
  );
  $form['pattern_info'] = array(
    '#type' => 'fieldset',
    '#group' => 'colourlovers_pattern',
		'#title' => t('Example Parameters'),
		'#weight' => -1,
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
  );
  $form['pattern_info']['info'] = array(
  	'#markup' => '<table><tr><th>Parameter</th><th>&nbsp;</th><th>Value</th></tr><tr><td>lover</td><td style="width: 20px;">=</td><td>COLOURlover username</td></tr><tr><td>hex</td><td style="width: 20px;">=</td><td><code>00FF33</code>  or  <code>00FF33,CC00FF,DD0033,003333,0033FF</code><br />[Possible Values: any valid 6-char hex value or list of up to five (5) hex values, separated by commas]</td></tr><tr><td>hex_logic</td><td style="width: 20px;">=</td><td>AND  or  OR<br />Sets the comparison logic for the hex list. Passing AND will find palettes with all provided hex values, passing OR will find palettes withany of the provided hex values.<br />[Possible Values: AND or OR. Default AND]</td></tr><tr><td>keywords</td><td style="width: 20px;">=</td><td>search+term</td></tr><tr><td>keywordExact</td><td style="width: 20px;">=</td><td>0 or 1 [Perform an exact search for the keywords passed. Default 0]</td></tr><tr><td>orderCol</td><td style="width: 20px;">=</td><td>X [Where X can be: dateCreated, score, name, numVotes, or numViews]</td></tr><tr><td>sortBy</td><td style="width: 20px;">=</td><td>X [Where X can be: ASC or DESC. Default ASC]</td></tr><tr><td>numResults</td><td style="width: 20px;">=</td><td>20 [Number of results to return, maximum of 100. Default 20]</td></tr><tr><td>resultOffset</td><td style="width: 20px;">=</td><td>5   [Result offset, for paging. Default 0]</td></tr><tr><td>showPaletteWidths</td><td style="width: 20px;">=</td><td>0 or 1 [Shows palette\'s color\'s widths. Default 0]</td></tr><tr><td colspan=3><strong>Requests with <code>orderCol</code> defined will override any <code>/new</code> or <code>/top</code> switch</strong></td></tr></table>',
  );
  $form['colourlovers_pattern']['colourlovers_patterns_lover'] = array(
		'#type' => 'textfield',
		'#title' => t('Lover'),
		'#description' => t('COLOURlover username'),
		'#default_value' => variable_get('colourlovers_patterns_lover', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_hex'] = array(
		'#type' => 'textfield',
		'#title' => t('Hex values'),
		'#description' => t('Possible Values: any valid 6-char hex value or list of up to five (5) hex values, separated by commas.').'<br />00FF33  or  00FF33,CC00FF,DD0033,003333,0033FF',
		'#default_value' => variable_get('colourlovers_patterns_hex', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_hexlogic'] = array(
		'#type' => 'select',
		'#title' => t('Hex Logic'),
		'#description' => t('AND or OR. Passing AND will find patterns with ALL provided hex values, OR will find patterns with any provided value. Default: AND'),
		'#default_value' => variable_get('colourlovers_patterns_hexlogic', 'AND'),
		'#options' => array(
			'and' => t('AND'),
			'or' => t('OR'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_keywords'] = array(
		'#type' => 'textfield',
		'#title' => t('Keywords'),
		'#description' => t('Search for patterns by keywords. Separate terms by commas.'),
		'#default_value' => variable_get('colourlovers_patterns_keywords', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_keywordexact'] = array(
		'#type' => 'select',
		'#title' => t('Exact keyword matching'),
		'#description' => t('Match keywords exactly, yes or no.'),
		'#default_value' => variable_get('colourlovers_patterns_keywordexact', FALSE),
		'#options' => array(
			0 => t('No'),
			1 => t('Yes'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_ordercol'] = array(
		'#type' => 'select',
		'#title' => t('Order by column'),
		'#description' => t('Can be: dateCreated, score, name, numVotes, or numViews.'),
		'#default_value' => variable_get('colourlovers_patterns_ordercol', FALSE),
		'#options' => array(
			'' => '',
			'dateCreated' => t('Date Created'),
			'score' => t('Score'),
			'name' => t('Name'),
			'numVotes' => t('Number of Votes'),
			'numViews' => t('Number of Views'),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_sortby'] = array(
		'#type' => 'select',
		'#title' => t('Sort By'),
		'#description' => t('Can be ascending or descending. Default: ASC'),
		'#default_value' => variable_get('colourlovers_patterns_sortby', 'ASC'),
		'#options' => array(
			'ASC' => t('Ascending'),
			'DESC' => t('Descending'),
		),
		'#states' => array(
			'invisible' => array(
				':input[name="colourlovers_patterns_ordercol"]' => array('value' => ''),
			),
		),
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_numresults'] = array(
		'#type' => 'select',
		'#title' => t('Number of Results'),
		'#description' => t('Maximum to 100. Default: 20'),
		'#default_value' => variable_get('colourlovers_patterns_numresults', 20),
		'#options' => drupal_map_assoc(range(1, 100)),
		'#required' => FALSE,
  );
  $form['colourlovers_pattern']['colourlovers_patterns_resultoffset'] = array(
		'#type' => 'textfield',
		'#title' => t('Result Offset'),
		'#description' => t('Offset results for paging. Default: 0'),
		'#default_value' => variable_get('colourlovers_patterns_resultoffset', 0),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => FALSE,
  );

  // Demo information and content.
  $form['colourlovers_demo'] = array(
    '#type' => 'fieldset',
		'#title' => t('Demo'),
		'#description' => t('This is only for demonstration.'),
		'#weight' => 4,
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
  );
	$form['colourlovers_demo']['colourlovers_demo_api'] = array(
		'#type' => 'textfield',
		'#title' => t('Demo API URL'),
		'#default_value' => variable_get('colourlovers_demo_api'),
		'#size' => 60,
		'#disabled' => TRUE,
		'#maxlength' => 128,
		'#required' => FALSE,
  );
  $form['colourlovers_demo']['colourlovers_demo_html'] = array(
  	'#markup' => variable_get('colourlovers_demo_html', 'No demo content generated.'),
  );

  $form['#validate'][] = 'colourlovers_admin_validate';

  return system_settings_form($form);
}

function colourlovers_admin_validate($form, &$form_state) {
	$type = $form_state['input']['colourlovers_type'];
	$mode = $form_state['input']['colourlovers_mode'];
	$query = array();

	foreach ($form_state['input'] as $key => $value) {
		$in = explode('_', $key);
		
		// Assign form values to array.
		if (isset($in[1]) && $in[0] == 'colourlovers' && isset($in[2]) && !empty($value)) {
			$query[$in[1]][$in[2]] = $value;
		}

		// Validate hex values.	
		if (isset($in[2]) && $in[2] == 'hex' && !empty($value)) {
			$form_state['values'][$key] = _colourlovers_hex_check($value);
		} else {
			// Remove hexlogic from query
			if (isset($query[$type]['hexlogic'])) {
				unset($query[$type]['hexlogic']);
			}
		}
		// Format keywords
		if (isset($in[2]) && $in[2] == 'keywords' && !empty($value)) {
			$query[$in[1]][$in[2]] = _colourlovers_format_keyword($value);
		}
	} // end foreach

	// Remove unnecessary sortby and num results.
	if (empty($query[$type]['ordercol'])) {
		unset($query[$type]['sortby']);
	}
	if ($query[$type]['numresults'] == 20) {
		unset($query[$type]['numresults']);
	}

	// Build what the API URL would look like
	$demo_path = 'http://www.colourlovers.com/api/'.$type;
	$demo_path .= ($mode != 'default') ? '/'.$mode : '';
	$options = array(
		'query' => $query[$type],
	);

	// Set demo api url.
	$demo_path = ($mode != 'random') ? url($demo_path, $options) : url($demo_path);
	$form_state['values']['colourlovers_demo_api'] = $demo_path;

	// Set up demo and palettes.
	$param = _colourlovers_clean_params($options['query']);
	$demo_content = _colourlovers_method($type, $mode, $param['num'], $param['params']);
	$demo_badges = array();

	if (!empty($demo_content)) {
		$theme_palette = array();
		foreach ($demo_content as $key => $value) {
			// If using palettes, set up Color module palette.
			if ($type == 'palettes' || $type == 'patterns') {
				$theme_palette[$value->id] = array(
					'title' => t($value->title),
					'colors' => $value->colors,
				);
			}
			// Set up badge images.
			$image_vars = array(
				'path' => $value->badgeUrl,
				'alt' => t($value->title),
				'title' => t($value->title),
				'attributes' => '',
			);
			$demo_badge[] = '<div class="colourlovers-image" style="float:left;width:25%;">'.l(theme_image($image_vars), $value->url, array('html' => true,'attributes' => array('target' => '_blank'))).'</div>';
		}
		$demo_badges = '<div class="colourlovers-images" style="width:100%;">'.implode('', $demo_badge).'</div>';
		variable_set('colourlovers_theme_palette', $theme_palette);
		variable_set('colourlovers_demo_html', $demo_badges);
	} else {
		variable_set('colourlovers_demo_html', NULL);
	}
}

/*
 * Helper function for formatting hex values.
 */
function _colourlovers_format_hex($color) {
	//Check for a hex color string '#c1c2b4'
	if (preg_match('/^#[a-f0-9]{6}$/i', $color)) {
		$color = substr($color, 1);
		return $color;
	} 
	//Check for a hex color string without hash 'c1c2b4'
	else if (preg_match('/^[a-f0-9]{6}$/i', $color)) {
		return $color;
	}
}

function _colourlovers_hex_check($value) {
	$hex = explode(',', $value);
	if (count($hex) > 6) {
		form_set_error($key, t('You have too many hex values. Please limit to 5.'));
	}
	else {
		foreach ($hex as $n => $x) {
			$hex[$n] = _colourlovers_format_hex(trim($x));
		}
		return implode(',', array_values(array_filter($hex)));
	}
}

function _colourlovers_format_keyword($value) {
	$keywords = explode(',', $value);
	foreach ($keywords as $key => $value) {
		$keywords[$key] = trim($value);
	}
	return implode('+', array_values(array_filter($keywords)));
}

/*
 * Helper function to separate parameters for object calls.
 */
function _colourlovers_clean_params($parameters = array()) {
	foreach ($parameters as $key => $value) {
		if ($key == 'numresults') {
			$parameters['num'] = $value;
			unset($parameters[$key]);
		} else {
			$parameters['params'][$key] = $value;
			unset($parameters[$key]);
		}
	}
	// Sometimes num doesn't exist because 20 is the default.
	// If it's empty, put in 20.
	$parameters['num'] = (empty($parameters['num'])) ? 20 : $parameters['num'];
	$parameters['params'] = (empty($parameters['params'])) ? array() : $parameters['params'];

	return $parameters;
}

/*
 * Helper function for choosing which method to run.
 */
function _colourlovers_method($type = 'palettes', $mode = 'default', $num = 20, $parameters = array()) {
	switch ($type) {
		case 'colors': {
			switch ($mode) {
				case 'top':
					return colourlovers_client()->getColorsTop($num, $parameters);
					break;
				case 'new':
					return colourlovers_client()->getColorsNew($num, $parameters);
					break;
				case 'random':
					return colourlovers_client()->getColorsRandom();
					break;
				
				default:
					return colourlovers_client()->getColors($num, $parameters);
					break;
			}
			break;
		}
		case 'palettes': {
			switch ($mode) {
				case 'top':
					return colourlovers_client()->getPalettesTop($num, $parameters);
					break;
				case 'new':
					return colourlovers_client()->getPalettesNew($num, $parameters);
					break;
				case 'random':
					return colourlovers_client()->getPalettesRandom();
					break;
				
				default:
					return colourlovers_client()->getPalettes($num, $parameters);
					break;
			}
			break;
		}
		case 'patterns': {
			switch ($mode) {
				case 'top':
					return colourlovers_client()->getPatternsTop($num, $parameters);
					break;
				case 'new':
					return colourlovers_client()->getPatternsNew($num, $parameters);
					break;
				case 'random':
					return colourlovers_client()->getPatternsRandom();
					break;
				
				default:
					return colourlovers_client()->getPatterns($num, $parameters);
					break;
			}
			break;
		}
		
		default: {
			# code...
			break;
		}
	}
}

