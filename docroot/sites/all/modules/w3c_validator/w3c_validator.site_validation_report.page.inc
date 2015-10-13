<?php

/**
 * @file
 * Page description for site W3C validation report.
 */

/**
 * W3C validation report page definition.
 * 
 * @param array $form
 * @param array $form_state
 */
function w3c_validator_site_validation_report_page($form, $form_state) {
	$header = $rows = array();
	
	// ---
	// Build the advanced operation form
	$warning = t('This will revalidate all following pages. This operation may be long.');
	$method = variable_get('w3c_validator_method', 'w3c_markup_validator');
	if ($method == 'w3c_markup_validator') {
		$baseUrl = variable_get('w3c_validator_api_endpoint_uri', 'http://validator.w3.org/check');
		if ($baseUrl == 'http://validator.w3.org/check') {
			$warning .= '<br/><br/><i><b>' . t('BEWARE : You are using the online W3C HTML Validator at http://validator.w3.org/check.') . '<br/>' . t('This may be consider spam and abuse of service. Therefore, performing this operation, you may get banned temporarily.') . '</b></i>';
		}
	}
	$form['advanced_operations'] = array(
		'#type'         => 'fieldset',
		'#title'        => t('advanced operations'),
		'#description' 	=> $warning,
		'#collapsible'  => TRUE,
		'#collapsed'    => TRUE,
	);
	
	$form['advanced_operations']['w3c_validator_revalidate_all'] = array(
    '#type' => 'submit',
    '#value' => 'Re-validate all pages',
		'#prefix'				=> '<br/>',
	);
	
	
	// ---
	// Build the report form
	
	
	// Retrieve the list of all page to validate.
	$all_site_pages = _w3c_validator_find_all_pages_to_validate();
	// Retrieve the list of already validated pges.
	$all_validated_pages = _w3c_validator_find_all_validated_pages();
		
	foreach ($all_site_pages as $page_url => $page) {

		$is_validated = FALSE;
		
		// Check if the page is validated.
		if (isset($all_validated_pages[$page_url])) {
			
			// Retrieve the validation result.
			$validation_result = $all_validated_pages[$page_url];
			$is_validated = TRUE;
			
			if ($validation_result->need_validation) {
				$validation_class = 'warning';
			} else if ($validation_result->validity) {
				$validation_class = ($validation_result->warning_count) ? 'warning' : 'status';
			} else {
				$validation_class = 'error';
			}
			
		} else {
			$validation_result = null;
			$validation_class = 'unknown';
		}
		
		// Render a row for a result
		$row = theme('w3c_validator_site_validation_report_row', array(
				'page' 							=> $page,
				'validation'				=> $validation_result,
		));
		
		$rows[] = array(
			'data' => array(
					array(
							'data' => $row,
							'class' => 'w3c_validator-wrapper collapsed',
					),
			),
			'class' => array($validation_class),
		);
	}
	$output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('w3c_validator-report'))));
	
	$form['w3c_validation-report'] = array(
		'#type' 	=> 'markup',
		'#markup' => $output,
	);
	
	// We use the core update module CSS to re-use the color definitions.
	// Plus add our own css and js.
	drupal_add_css(drupal_get_path('module', 'update') . '/update.css');
	drupal_add_css(drupal_get_path('module', 'w3c_validator') . '/css/w3c_validator.css');
	drupal_add_js(drupal_get_path('module', 'w3c_validator') . '/js/w3c_validator.js');
	
	return $form;
}

/**
 * Find all pages URL to validate in the site.
 * 
 * Currently, this method returns :
 * 	- frontpage
 *  - nodes
 *  @todo: return other pages.
 * @return Array
 */
function _w3c_validator_find_all_pages_to_validate() {
	$all_site_pages = array();
	
	// add frontpage
	$site_frontpage = variable_get('site_frontpage', 'node');
	$all_site_pages[$site_frontpage] = array('url' => $site_frontpage, 'title' => t('Frontpage'));
	
	// Add all nodes
	$query = db_select('node', 'n')
	->fields('n', array('nid', 'title'));
	$query->addExpression("CONCAT('node/', n.nid)", 'url');
	$db_nodes = $query->execute();
	$nodes = $db_nodes->fetchAllAssoc('url', PDO::FETCH_ASSOC);
	$all_site_pages = array_merge($all_site_pages, $nodes);
	
	return $all_site_pages;
}

/**
 * Find all already validated pages and their validation result.
 *
 * @return Array
 */
function _w3c_validator_find_all_validated_pages() {

	$db_result = db_select('w3c_validator', 'w')
	->fields('w')
	->execute();
	
	return $db_result->fetchAllAssoc('uri');
	
}

/**
 * Format a w3c validation report page row.
 *
 * @param $variables
 *   An associative array containing:
 *    - page: An array containing the page info (url and title).
 *    - validation: An W3C_Response instance reprensenting the validation result for the page.
 *
 * @return string
 *   HTML output.
 */
function theme_w3c_validator_site_validation_report_row($variables) {
	
	$page 				= $variables['page'];
	$validation 	= $variables['validation'];
	
	
	// Output project title and project summary status.
	$output = theme('w3c_validator_site_validation_report_row_page_status', array(
			'page' 				=> $page,
			'validation' 	=> $validation,
	));
	
	if ($validation) {
		$output .= '<div class="fieldset-wrapper w3c_analysis-results">' . "\n";
		$output .= _w3c_validator_display_result($validation);
		$output .= "</div>\n";
	}
	
	return $output;
	
}

/**
 * Format a w3c validation report page row status bar.
 *
 * @param $variables
 *   An associative array containing:
 *    - page: An array containing the page info (url and title).
 *    - validation: An W3C_Response instance reprensenting the validation result for the page.
 *
 * @return string
 *   HTML output.
 */
function theme_w3c_validator_site_validation_report_row_page_status($variables) {
	
	$page 				= $variables['page'];
	$validation 	= $variables['validation'];
	
	$page_title 	= $page['title'];
	
	$output = '<div class="page">';
	$output .= '<span class="page-icon"></span>';
	$output .= '<span class="page-title">' . check_plain($page_title) . '</span>';
	if ($validation != null) {
		$output .= '<span class="page-infos">';
		$output .= '<span class="page-errors">' . t('@error_count errors', array('@error_count' => $validation->error_count)) . '</span>';
		$output .= '<span class="page-warning">' . t('@warning_count warnings', array('@warning_count' => $validation->warning_count)) . '</span>';
		$output .= '</span>';
		$output .= '<span class="page-result">';
		if ($validation->need_validation) {
			$output .= '<span class="outdated">' . t('Outdated') . '</span>';
		} else if ($validation->validity && $validation->warning_count) {
			$output .= '<span class="warning">' . t('Warning') . '</span>';
		} else if ($validation->validity) {
			$output .= '<span class="valid">' . t('Valid') . '</span>';
		}else {
			$output .= '<span class="error">' . t('Unvalid') . '</span>';
		}
		$output .= '</span>';
	} else {
		$output .= '<span class="page-infos page-unknown">' . t('Not yet validated') . '</span>';
		$output .= '<span class="page-result"><span class="unknown">' . t('Unknown') . '</span></span>';
	}
	
	$output .= "</div>\n";
	
	return $output;
}

function w3c_validator_site_validation_report_page_submit($form, &$form_state) {
  batch_set(w3c_validator_site_validation_batch_set());
}

function w3c_validator_site_validation_batch_set() {
	
	$all_pages = _w3c_validator_find_all_pages_to_validate();
	
	$operations = array();
	
	$i = 0;
	foreach ($all_pages as $uri => $page) {
		
		$operations[] = array(
				'_w3c_validator_full_validation_operation',
				array(
						$uri,
						false,
				),
		);
	}
	$batch = array(
			'operations' => $operations,
	);
	return $batch;
}