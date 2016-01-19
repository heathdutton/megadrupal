<?php 
/**
 * @file
 * Table
 * Displays the best time of day for donations
 */
$plugin = array(
  'single' => TRUE,  
  'title' => t('Donations by Page: Donation Page Time table'),  
  'description' => t('Displays a table displaying the best time of day for receiving donations on the selected page..'),
  'category' => t('CRM Core Donations'), 
  'edit form' => 'crm_core_donation_page_time_table_form', 
  'render callback' => 'crm_core_donation_page_time_table', 
  'admin info' => 'crm_core_donation_page_time_table_info', 
  'defaults' => array(),
  'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
);

/**
 * admin info
 */
function crm_core_donation_page_time_table_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Donation page: time of day table');
  $block->content = t('Displays the times of day when donations have been received.');
  return $block;
}

/**
 * Settings form
 */
function crm_core_donation_page_time_table_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}
/**
 * submit handler for settings form
 */
function crm_core_donation_page_time_table_form_submit($form, &$form_state) {
}

/**
 * Render callback
 */
function crm_core_donation_page_time_table($subtype, $conf, $panel_args, $context = NULL) {
  
  // set up a table for displaying raw data
  $header = array('Time', 'Donors', 'Total');
  $rows = array();
  $best_time = '';
  $high = 0;
  
  // select all donations for this page
  // return time of day, total donations and amount
  $sql = "SELECT 
	  	SUM(fda.field_cmcd_amount_value) AS amount, 
	  	COUNT(fda.field_cmcd_amount_value) AS total, 
    	DATE_FORMAT(field_cmcd_receive_date_value, '%H:00') AS hours
  	FROM {field_data_field_cmcd_amount} fda 
    	JOIN {field_data_field_cmcd_receive_date} fdd
    	ON fda.entity_id = fdd.entity_id 
    	JOIN {field_data_field_cmcd_page} fdp
    	ON fdd.entity_id = fdp.entity_id
  	WHERE fdd.field_cmcd_receive_date_value != ''
  	  AND fdd.field_cmcd_receive_date_value IS NOT NULL";
  // account for missing parameters
  if (isset($panel_args[0])) {
    $sql .= ' AND fdp.field_cmcd_page_target_id = :page';
  }

  $sql .= " GROUP BY hours ORDER BY hours;";
  
  // set up the arguments
  // we will not always have an id, so account for that
  $args = array();
  if (isset($panel_args[0])) {
    $args = array(':page' => decode_entities($panel_args[0]));
  }
  
  // execute the query
  $dates = db_query($sql, $args);
  
  // set up the datastore
  for ($i = 0; $i < 24; $i++) {
    $hrs_mark = $i;
    if (strlen($i) < 2) {
      $hrs_mark = '0' . $i;
    }
    $rows[] = array(
      'data' => array(
        array(
          'data' => $hrs_mark . ':00',
        	'class'=> 'table-field-time-of-day'
        ),    
        array(
          'data' => 0,
        	'class'=> 'table-field-total-donations'
        ),    
        array(
          'data' => _crm_core_profile_commerce_items_currency_format(0),
        	'class'=> 'table-field-total-amount-by-time'
        ),    
      )
    );
  }
  
  // populate the datastore
  foreach ($dates as $date) {
    
    if ($date->amount > $high) {
      $high = $date->amount;
      $best_time = $date->hours;
    }
    
    foreach ($rows as &$row) {
      if ($row['data'][0]['data'] == $date->hours) {
        $row['data'] = array(
          array(
            'data' => $date->hours,
          	'class'=> 'table-field-time-of-day'
          ),    
          array(
            'data' => $date->total,
          	'class'=> 'table-field-total-donations'
          ),    
          array(
            'data' => _crm_core_profile_commerce_items_currency_format($date->amount),
          	'class'=> 'table-field-total-amount-by-time'
          ),
        );
      }
    }
  }
  
  // output the table
  $content = t('<div class="widget-summary-info">Based on when you have received donations, the best time of day for this donation page is <span class="widget-summary-info-detail">@hours.</span></div>', array('@hours' => $best_time));
  $content .= '<div class="table-container">' . theme('table', array( 'rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('table-donation-time-of-day')))) . '</div>';
  
  $block = new stdClass();
  $block->title = t('Time of Day');
  $block->content = $content;
  
  return $block;  
  
}
