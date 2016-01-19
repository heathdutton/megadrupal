<?php
/**
 * @file simplyhired.node.inc
 * Contains all functionality related to the "Job Search" content type.
 */

/**
 * Implements hook_node_info().
 * Tell Drupal about the new 'Job Search page' node type.
 * 
 * @return array
 */
function simplyhired_listing_node_info() {
	return array(
		'job_listing' => array(
			'name' => t('SimplyHired Listing page'),
			'base' => 'job_listing',
			'description' => t('Content type that allows pre-configured search criteria to be sent to SimplyHired\'s API.'),
			'has_title' => TRUE,
			'locked' => TRUE,
		), 
	);
}

function job_listing_form($node, $form_state) {
	return node_content_form($node, $form_state);
}

function simplyhired_listing_node_view($node, $view_mode, $langcode) {
	switch ($node->type) {
		case 'job_listing':
			// Params that will be sent to the API call.
			$params = array();
			foreach (array_keys((array) $node) as $key) {
				if (!preg_match('/\Asimplyhired_[a-z_]+\Z/', $key)) continue;
				switch ($key ) {
					case 'simplyhired_query':
						$idx = 'query';
						break;
					case 'simplyhired_location':
						$idx = 'location';
						break;
					case 'simplyhired_distance':
						$idx = 'miles';
						break;
					case 'simplyhired_sort':
						$idx = 'sort';
						break;
					case 'simplyhired_per_page':
						$idx = 'size';
						break;
					case 'simplyhired_exerpt_only':
						$idx = 'exerpt';
						break;
					case 'simplyhired_fsr':
						$idx = 'type';
						break;
					case 'simplyhired_source':
						$idx = 'source';
						break;
          case 'simplyhired_employer_type':
            $idx = 'employer_type';
            break;
          case 'simplyhired_job_type':
            $idx = 'job_type';
            break;
          case 'simplyhired_education':
            $idx = 'education';
            break;
          case 'simplyhired_special':
            $idx = 'special';
            break;
          case 'simplyhired_post_date':
            $idx = 'post_date';
            break;
				}
				$params[$idx] = $node->$key;
			}
			
			$params['page'] = (isset($_GET['page']) ? intval($_GET['page'])+1 : 0);
			
			// Additional variables that won't be sent to the API.
			$params['date_format'] = $node->simplyhired_date_format;
			
			$data = _simplyhired_listings_search($params);
			
			if ( $data ) {
					$node->content['job_list'] = $data;
					$node->content['pager'] = array('#markup' => theme('pager'));
					$image_file = 'sh-jobsby-' . (variable_get('simplyhired_logo', 'default') ? 'normal' : 'light') . '.png';
					$image_data = array(
						'path' => '/' . drupal_get_path('module', 'simplyhired') . '/images/' . $image_file,
						'width' => 152,
						'height' => 20,
						'alt' => t('Jobs by SimplyHired'),
					);
					$image = theme('image', $image_data);
					$options = array(
						'html' => TRUE,
						'attributes' => array(
							'target' => '_blank',
							'title' => t('Jobs by SimplyHired'),
						),
					);
					$attribution = l($image, 'https://simplyhired.com', $options);

					$node->content['attribution'] = array(
						'#type' => 'item',
						'#markup' =>  $attribution,
						'#prefix' => '<div id="simplyhired-attribution">',
						'#suffix' => '</div>',
					);
					
					// Add our stylesheet.
					$node->content['#attached'] = array(
						'css' => array(
							drupal_get_path('module', 'simplyhired_listing') . '/css/simplyhired-listing.css',
						),
			    );
			}
	    break;
		default:
			// do nothing.
			break;
	}
}

/**
 * Implements hook_form_node_form_alter().
 * @param array $form
 * @param array $form_state
 */
function simplyhired_listing_form_job_listing_node_form_alter(&$form, &$form_state) {
	$node = $form['#node'];
	
	/*
	 * @todo add search params in vertical tabs.
	 * @see http://www.wdtutorials.com/2013/10/20/drupal-7-snippet-how-hide-additional-settings-drupal-forms#.VEKwTYvz1g8
	 */
	$form['simplyhired'] = array(
		'#type' => 'fieldset',
		'#title' => t('Job search settings'),
		'#collapsed' => TRUE,
		'#group' => 'additional_settings',
		'#weight' => -5,
	);
	
	$form['simplyhired']['search_header'] = array(
		'#type' => 'item',
		'#markup' => t('API Parameters'),
		'#prefix' => '<h4>',
		'#suffix' => '</h4>'
	);

	$form['simplyhired']['simplyhired_source'] = array(
		'#type' => 'select',
		'#title' => t('API source'),
		'#default_value' => (!isset($node->nid) ? 'us' : $node->simplyhired_source),
		'#options' => _simplyhired_get_endpoints(),
		'#description' => t(''),
	);
	
	$link = l(t('portal documentation'), 
					'https://simply-partner.com/partner', 
					array('attributes' => array('target' => '_blank')));
	$form['simplyhired']['simplyhired_query'] = array(
		'#type' => 'textfield',
		'#title' => t('Search text'),
		'#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_query),
		'#size' => 40,
		'#maxlength' => 255,
		'#description' => t('Supports advanced AND/OR/NOT and (&nbsp;) grouping. See the !link for more information.', array('!link' => $link)),
		'#required' => TRUE,
	);
	
	$form['simplyhired']['simplyhired_location'] = array(
		'#type' => 'textfield',
		'#title' => t('Location'),
		'#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_location),
		'#size' => 10,
		'#description' => t('Location can be a zipcode, state, or city-state combination. Currently, there is no support for multiple location search.')
	);
	$desc = <<<DESC
The number of miles from the location specified. Miles value should be a number 
from "1" to "100". Miles represents the radius from the zip code, if specified 
in Location, or an approximate geographical "city center" if only city and state 
are present. If Miles is not specified, search will default to a radius of 
25 miles.					
DESC;
	$form['simplyhired']['simplyhired_distance'] = array(
		'#type' => 'textfield',
		'#title' => t('Miles'),
		'#default_value' => (!isset($node->nid) || $node->simplyhired_distance == 0 ? '' : $node->simplyhired_distance),
		'#size' => 10,
		'#description' => t($desc),
	);
	
	$form['simplyhired']['simplyhired_sort'] = array(
		'#type' => 'select',
		'#title' => t('Sort'),
		'#default_value' => (!isset($node->nid) ? 'rd' : $node->simplyhired_sort),
		'#options' => _simplyhired_get_sort_options(),
		'#description' => t('The sort order of organic jobs (sponsored jobs have a fixed sort order).'),
	);

  $form['simplyhired']['simplyhired_post_date'] = array(
    '#type' => 'select',
    '#title' => t('Date posted'),
    '#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_post_date),
    '#options' => _simplyhired_get_date_posted_options(),
    '#description' => t('Parameter indicating the date posted filter to use on the result set.'),
  );

  $form['simplyhired']['simplyhired_job_type'] = array(
    '#type' => 'select',
    '#title' => t('Job type'),
    '#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_job_type),
    '#options' => _simplyhired_get_job_type_options(),
    '#description' => t('Parameter indicating the job type filter to use on the result set.'),
  );

  $form['simplyhired']['simplyhired_education'] = array(
    '#type' => 'select',
    '#title' => t('Education'),
    '#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_education),
    '#options' => _simplyhired_get_education_options(),
    '#description' => t('Parameter indicating the education filter to use on the result set.'),
  );

  $form['simplyhired']['simplyhired_employer_type'] = array(
    '#type' => 'select',
    '#title' => t('Employer type'),
    '#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_employer_type),
    '#options' => _simplyhired_get_employer_type_options(),
    '#description' => t('Parameter indicating the indicating the employer type.'),
  );

  $form['simplyhired']['simplyhired_special'] = array(
    '#type' => 'select',
    '#title' => t('Special filters'),
    '#default_value' => (!isset($node->nid) ? '' : $node->simplyhired_special),
    '#options' => _simplyhired_get_special_options(),
    '#description' => t('Parameter indicating the special filter to use on the result set.'),
  );

	$form['simplyhired']['simplyhired_per_page'] = array(
		'#type' => 'select',
		'#title' => t('Result per page'),
		'#default_value' => (!isset($node->nid) ? 10 : $node->simplyhired_per_page),
		'#options' => drupal_map_assoc(array(10, 20, 30, 50, 100)),
		'#description' => t('The number of results returned. When available, the XML Results API will return 10 jobs by default. The API is limited to a maximum of 100 results per request.'),
	);
	
	$form['simplyhired']['simplyhired_fsr'] = array(
		'#type' => 'radios',
		'#title' => t(''),
		'#default_value' => (empty($node->simplyhired_fsr) ? 'all' : $node->simplyhired_fsr),
		'#options' => array('all' => t('All'), 'primary' => t('Exclude job boards'), 'job_board' => t('Job boards only')),
		'#description' => t('Job source type; job boards only, exclude job boards, or all.'),
	);
	
	$form['simplyhired']['display_header'] = array(
		'#type' => 'item',
		'#markup' => t('Jobs Display'),
		'#prefix' => '<h4>',
		'#suffix' => '</h4>'
	);
	
	// Get the available date formats to populate a select field.
	$opts = array();
	$formats = system_get_date_types();
	foreach ($formats as $key => $value) {
		$opts[$key] = $formats[$key]['title'];
	}
	
	$form['simplyhired']['simplyhired_date_format'] = array(
		'#type' => 'select',
		'#title' => t('Date format'),
		'#options' => $opts,
		'#default_value' => (!isset($node->nid) ? 'short' : $node->simplyhired_date_format),
		'#description' => t('Select the format that will be used for job posting dates.')
	);
	
	$form['simplyhired']['simplyhired_exerpt_only'] = array(
		'#type' => 'checkbox',
		'#title' => t('Exerpt only'),
		'#default_value' => (empty($node->simplyhired_exerpt_only) ? 0 : $node->simplyhired_exerpt_only),
		'#description' => t('Show clipped or full job description.')
	);
	
	$form['#validate'][] = 'simplyhired_listing_node_form_validate';
}

/**
 * Implements hook_load().
 * Load search configuration for the job_search nodes.
 * 
 * @param array $nodes
 */
function job_listing_load(&$nodes) {
	$result = db_query('SELECT * FROM {simplyhired_listing} WHERE nid IN (:nids)', 
					array(':nids' => array_keys($nodes)));
	foreach ( $result as $row ) {
		$nodes[$row->nid]->simplyhired_source = $row->source;
		$nodes[$row->nid]->simplyhired_query = $row->search_text;
		$nodes[$row->nid]->simplyhired_location = (empty($row->location) ? NULL : $row->location);
		$nodes[$row->nid]->simplyhired_distance = ($row->distance == 0 ? NULL : $row->distance);
		$nodes[$row->nid]->simplyhired_sort = $row->sort_by;
    $nodes[$row->nid]->simplyhired_post_date = (empty($row->fbd) ? NULL : $row->fbd);
    $nodes[$row->nid]->simplyhired_job_type = (empty($row->fjt) ? NULL : $row->fjt);
    $nodes[$row->nid]->simplyhired_employer_type = (empty($row->fem) ? NULL : $row->fem);
    $nodes[$row->nid]->simplyhired_special = (empty($row->frl) ? NULL : $row->frl);
    $nodes[$row->nid]->simplyhired_education = (empty($row->fed) ? 0 : $row->fed);
		$nodes[$row->nid]->simplyhired_per_page = $row->results_per_page;
		$nodes[$row->nid]->simplyhired_exerpt_only = ($row->exerpt_only == 1 ? TRUE : FALSE);
		$nodes[$row->nid]->simplyhired_fsr = trim($row->fsr);
		$nodes[$row->nid]->simplyhired_date_format = $row->date_format;
	}
}

/**
 * Implements hook_insert().
 * Inserts the search configuration values for job_search nodes.
 * 
 * @param object $node
 */
function job_listing_insert($node) {
	db_insert('simplyhired_listing')->fields(array(
		'nid' => $node->nid,
		'source' => $node->simplyhired_source,
		'search_text' => $node->simplyhired_query,
		'location' => $node->simplyhired_location,
		'distance' => (empty($node->simplyhired_distance) ? 0 : $node->simplyhired_distance),
		'sort_by' => $node->simplyhired_sort,
    'fbd' => $node->simplyhired_post_date,
    'fem' => $node->simplyhired_employer_type,
    'frl' => $node->simplyhired_special,
    'fed' => $node->simplyhired_education,
    'fjt' => $node->simplyhired_job_type,
		'results_per_page' => $node->simplyhired_per_page,
		'exerpt_only' => $node->simplyhired_exerpt_only,
		'fsr' => $node->simplyhired_fsr,
		'date_format' => $node->simplyhired_date_format,
	))->execute();
}

/**
 * Implements hook_update().
 * Updates the search configuration values for the job_search nodes.
 * @param object $node
 */
function job_listing_update($node) {
	db_update('simplyhired_listing')->fields(array(
		'source' => $node->simplyhired_source,
		'search_text' => $node->simplyhired_query,
		'location' => $node->simplyhired_location,
		'distance' => (empty($node->simplyhired_distance) ? 0 : $node->simplyhired_distance),
		'sort_by' => $node->simplyhired_sort,
    'fbd' => $node->simplyhired_post_date,
    'fem' => $node->simplyhired_employer_type,
    'frl' => $node->simplyhired_special,
    'fed' => $node->simplyhired_education,
    'fjt' => $node->simplyhired_job_type,
		'results_per_page' => $node->simplyhired_per_page,
		'exerpt_only' => $node->simplyhired_exerpt_only,
		'fsr' => $node->simplyhired_fsr,
		'date_format' => $node->simplyhired_date_format,
	))
		->condition('nid', $node->nid)
		->execute();
}

/**
 * Implements hook_delete().
 * Removes search configuration values for job_search nodes being deleted.
 * @param object $node
 */
function job_listing_delete($node) {
	db_delete('simplyhired_listing')->condition('nid', $node->nid)->execute();
}

/**
 * Implements node form validation.
 * @param array $form
 * @param array $form_state
 */
function simplyhired_listing_node_form_validate($form, &$form_state) {
	$distance = trim($form_state['values']['simplyhired_distance']);
	
	if ($distance != '') {
		$distance = $distance;
		if (strlen($distance) > 3 || $distance < 1 || $distance > 100) {
			form_set_error('simplyhired_distance', t('Miles value should empty OR an integer between 1 and 100.'));
		}
	}
	
}

/**
 * Implements hook_node_submit().
 * 
 * @param object $node
 * @param array $form
 * @param array $form_state
 */
function simplyhired_listing_node_submit(&$node, $form, &$form_state) {
	
}

/**
 * Utility function that will call the API.
 * @param array $params
 * @return mixed
 */
function _simplyhired_listings_search($params = array()) {
	if (!is_array($params) || empty($params)) {
		watchdog('simplyhired_listings', t('SimplyHired Listing search called with empty parameters.'), WATCHDOG_WARNING);
		return FALSE;
	}
	$record_stats = FALSE;
  $query = $params['query'];
  $exerpt = (bool)$params['exerpt'];
  unset($params['query'], $params['exerpt']);

  try {
    $search = simplyhired_search($query, $exerpt, $params);
		
		pager_default_initialize($search['total_visible'], $params['size']);
		$message = FALSE;
		// Build our render array for the results.
		$items = array();
		
		if (isset($search['items']) && count($search['items'])) {
      foreach ($search['items'] as $job) {
        $zebra = (count($items) % 2 == 0 && count($items) != 0 ? 'even' : 'odd');
        $company = $job->getCompany();
        $source = $job->getSource();
        $location = $job->getLocation();

        // Check the company and source. If they have the same name,
        // we'll set company to FALSE so it's not rendered.
        if ($source['name'] == $company['name']) {
          $company_element = FALSE;
        }
        else {
          $company_element = array();
          if (empty($company['url'])) {
            $company_element['#markup'] = check_plain($company['name']);
          }
          else {
            $company_element['#type'] = 'link';
            $company_element['#title'] = check_plain($company['name']);
            $company_element['#href'] = $company['url'];
            $company_element['#options'] = array('attributes' => array('target' => '_blank'));
          }
        }

        $item = array(
          '#theme' => 'simplyhired_job',
          'zebra' => $zebra,
          '#title' => array(
            '#type' => 'link',
            '#title' => check_plain($job->getJobTitle()),
            '#href' => $source['url'],
            '#options' => array(
              'attributes' => array('target' => '_blank'),
            ),
          ),
          '#source' => array(
            '#type' => 'link',
            '#title' => check_plain($source['name']),
            '#href' => $source['url'],
            '#options' => array(
              'attributes' => array('target' => '_blank'),
            ),
          ),
          '#post_date' => array(
            '#markup' => format_date($job->getPostDate(), $params['date_format']),
          ),
          '#company' => $company_element,
          '#location' => array(
            '#theme' => 'simplyhired_location',
            '#city' => $location->city,
            '#province' => $location->state,
            '#postal_code' => $location->postal_code,
            '#county' => $location->county,
            '#country' => $location->country,
            '#source' => $params['source'],
            'raw' => $location->raw,
          ),
          '#description' => array(
            '#markup' => check_markup($job->getExerpt(), 'plain_text'),
          ),

        );
        $items[] = $item;
      }
    } elseif (isset($search['items']) && count($search['items']) == 0) {
      $message = array(
        '#prefix' => '<div class="simplyhired-listing-message">',
        '#suffix' => '</div>',
        '#markup' => t('Your job search did not return any results.'),
      );
    } else {
			// return error message because it "should" be populated.
			$items = FALSE;
			$message = array(
				'#prefix' => '<div class="simplyhired-listing-message">',
				'#suffix' => '</div>',
				'#markup' => $search['error'],
			);
		}
		
		$element = array(
			'#theme' => 'simplyhired_listings',
			'#title' => check_plain($search['title']),
			'#jobs' => $items,
			'#message' => $message,
		);
		
		return $element;
		
	} catch (Exception $e) {
		watchdog('simplyhired_listings', $e->getMessage(), NULL, WATCHDOG_ERROR);
		drupal_set_message($e->getMessage(), 'error');
		return FALSE;
	}
}