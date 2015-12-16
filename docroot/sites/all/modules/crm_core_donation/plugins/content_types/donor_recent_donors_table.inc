<?php
/**
 * @file
 * Displays a list of new donors, sorted by date.
 */

$plugin = array(
  'single' => TRUE,
  // Just do this one, it is needed.
  'title' => t('Donors: Recent Donors table'),
  // Title to show up on the pane screen.
  'description' => t('Displays a list of new donors, sorted by date.'),
  // Description to show up on the pane screen.
  'category' => t('CRM Core Donations'),
  // A category to put this under.
  'edit form' => 'crm_core_donation_recent_donor_table_form',
  // A function that will return the settings form for the pane.
  'render callback' => 'crm_core_donation_recent_donor_table',
  // A function that will return the renderable content.
  'admin info' => 'crm_core_donation_recent_donor_table_info',
  // A function that will return the information displayed on the admin screen.
  'defaults' => array(),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function crm_core_donation_recent_donor_table_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Donors: recent donors table');
  $block->content = t('Displays recent donors who have come into the system, sorted by date of first donation.');

  return $block;
}

/**
 * Settings form.
 */
function crm_core_donation_recent_donor_table_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['donors_to_display'] = array(
    '#type' => 'textfield',
    '#title' => t('Donors to Display'),
    '#description' => t('Please select the number of donors to display in this table.'),
    '#default_value' => !empty($conf['donors_to_display']) ? $conf['donors_to_display'] : '10',
  );

  return $form;
}

/**
 * Submit handler for settings form.
 */
function crm_core_donation_recent_donor_table_form_submit($form, &$form_state) {
  $form_state['conf']['donors_to_display'] = $form_state['values']['donors_to_display'];
}

/**
 * Render callback.
 */
function crm_core_donation_recent_donor_table($subtype, $conf, $panel_args, $context = NULL) {

  $content = '';

  // Check and make sure there is a number of donors to display, otherwise,
  // Default to 10.
  if (isset($conf['donors_to_display'])) {
    $display = $conf['donors_to_display'];
  }
  else {
    $display = 10;
  }

  // Set up a table for displaying data.
  $header = array('Date', 'Donor', 'Amount', 'Source');
  $rows = array();

  // This returns the top list of donors, sorted by date of first donation.
  $sql = <<<SQL
  SELECT 
  	fdrd.entity_id AS aid, 
  	fdrd.field_cmcd_receive_date_value AS date, 
  	fap.field_activity_participants_target_id AS cid
  FROM {field_data_field_cmcd_receive_date} fdrd
    JOIN {field_data_field_activity_participants} fap
    ON fdrd.entity_id = fap.entity_id
  WHERE fdrd.field_cmcd_receive_date_value = (
  	SELECT MIN(fd.field_cmcd_receive_date_value)
  	FROM {field_data_field_cmcd_receive_date} fd
  	  JOIN {field_data_field_activity_participants} fp
	    ON fd.entity_id = fp.entity_id
  	WHERE fap.field_activity_participants_target_id = fp.field_activity_participants_target_id
  ) 
  GROUP BY cid
  ORDER BY date DESC, cid
  LIMIT 0, $display
SQL;

  // Set the limit.
  $args = array();

  // Execute the query.
  $donors = db_query($sql, $args);

  // Set up the records.
  foreach ($donors as $donor) {

    // There is probably a better way to do this
    // get the contact entity.
    $contact = entity_load('crm_core_contact', array('contact_id' => $donor->cid));
    $contact_name = field_view_field('crm_core_contact', $contact[$donor->cid], 'contact_name', 'full');

    // Get the activity entity.
    $activity = entity_load('crm_core_activity', array('activity_id' => $donor->aid));
    $activity_amt = field_view_field('crm_core_activity', $activity[$donor->aid], 'field_cmcd_amount', 'full');
    $activity_source = field_view_field('crm_core_activity', $activity[$donor->aid], 'field_cmcd_source', 'full');

    // Set a link for the contact.
    $contact_link = l(render($contact_name[0]), 'crm-core/contact/' . $donor->cid);

    // Source link.
    $source = render($activity_source[0]);
    $source_link = l($source, 'crm-core/reports/cmcd/donation-sources/summary/' . $source);

    // Populate the table.
    $rows[] = array(
      'data' => array(
        $donor->date,
        $contact_link,
        _crm_core_profile_commerce_items_currency_format($activity_amt[0]['#markup']),
        $source_link,
      ),
    );
  }

  // Output the table.
  // Adding wrapper block for fancy styling.
  $content .= '<div class="fancy-headers">';
  $content .= '<div class="table-container">' . theme('table', array('rows' => $rows, 'header' => $header)) . '</div>';
  $content .= '</div>';

  $block = new stdClass();
  $block->title = t('New Donors');
  $block->content = $content;

  return $block;
}
