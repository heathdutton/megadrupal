<?php 
/**
 * @file
 * Donation page day table.
 * 
 */
$plugin = array(
  'single' => TRUE, 
  'title' => t('Donations by Page: Donation Page Day table'),
  'description' => t('Displays a table displaying the best day of the week for receiving donations on the selected page..'),
  'category' => t('CRM Core Donations'),
  'edit form' => 'crm_core_donation_page_day_table_form',
  'render callback' => 'crm_core_donation_page_day_table', 
  'admin info' => 'crm_core_donation_page_day_table_info', 
  'defaults' => array(),
  'all contexts' => TRUE,
);

/**
 * admin info
 */
function crm_core_donation_page_day_table_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Donation page: day of week table');
  $block->content = t('Displays the days of the week when donations have been received.');
  return $block;
}

/**
 * Settings form
 */
function crm_core_donation_page_day_table_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}
/**
 * submit handler for settings form
 */
function crm_core_donation_page_day_table_form_submit($form, &$form_state) {
}

/**
 * Render callback
 */
function crm_core_donation_page_day_table($subtype, $conf, $panel_args, $context = NULL) {
  
  // set up a table for displaying raw data
  $header = array('Day', 'Donors', 'Total');
  $rows = array();
  $days = array(
    t('Sunday'),
    t('Monday'),
    t('Tuesday'),
    t('Wednesday'),
    t('Thursday'),
    t('Friday'),
    t('Saturday')
  );
  $high = '';
  $best_day = '';
  
  // select all donations for this page
  // return time of day, total donations and amount
  $sql = "SELECT 
	  	SUM(fda.field_cmcd_amount_value) AS amount, 
	  	COUNT(fda.field_cmcd_amount_value) AS total, 
    	DATE_FORMAT(field_cmcd_receive_date_value, '%W') AS day
  	FROM {field_data_field_cmcd_amount} fda 
    	JOIN {field_data_field_cmcd_receive_date} fdd
    	ON fda.entity_id = fdd.entity_id 
    	JOIN {field_data_field_cmcd_page} fdp
    	ON fdd.entity_id = fdp.entity_id
  	WHERE fdd.field_cmcd_receive_date_value != ''
  	  AND fdd.field_cmcd_receive_date_value IS NOT NULL";

  if (isset($panel_args[0])) {
    $sql .= " AND fdp.field_cmcd_page_target_id = :page";
  }
  
  $sql .= " GROUP BY day ORDER BY day;";
  
  // set up the arguments
  // we will not always have an id, so account for that
  $args = array();
  if (isset($panel_args[0])) {
    $args = array(':page' => decode_entities($panel_args[0]));
  }
  
  // execute the query
  $dates = db_query($sql, $args);
  
  // set up the datastore
  foreach ($days as $day) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => $day,
        	'class'=> 'table-field-day-of-week'
        ),    
        array(
          'data' => 0,
        	'class'=> 'table-field-total-donors'
        ),    
        array(
          'data' => _crm_core_profile_commerce_items_currency_format(0),
        	'class'=> 'table-field-total-amount'
        ),    
      )
    );
  } 
  
  // populate the datastore
  foreach ($dates as $date) {
    
    if ($date->amount > $high) {
      $best_day = $date->day;
      $high = $date->amount;
    }
    
    foreach ($rows as &$row) {
      if ($row['data'][0]['data'] == $date->day) {
        $row['data'][0]['data'] = $date->day;
        $row['data'][1]['data'] = $date->total;
        $row['data'][2]['data'] = _crm_core_profile_commerce_items_currency_format($date->amount);
      }
    }
  }
  
  // output the table
  $content = t('<div class="widget-summary-info">Based on when you have received donations, the best day of the week for this donation page is <span class="widget-summary-info-detail">@day</span>.</div>', array('@day' => $best_day));
  $content .= '<div class="table-container">' . theme('table', array( 'rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('table-donation-day-of-week')))) . '</div>';
  
  $block = new stdClass();
  $block->title = t('Day of Week');
  $block->content = $content;
  
  return $block;  
  
}








