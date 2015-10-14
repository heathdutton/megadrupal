<?php

/**
 * @file
 * Corporate Disk Fulfillment variable module include file.
 */

/**
 * Implements hook_variable_info().
 */
function corporate_disk_company_variable_info($options) {

  $variable['corporate_disk_company_companyid'] = array(
    'type' => 'number',
    'title' => t('Company ID'),
    'description' => t('Disk.com company id number.'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_live_status'] = array(
    'title' => t('Enable live transactions'),
    'description' => t('This will enable the module to send order fulfillment requests to Disk.com'),
    'type' => 'boolean',
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
    'default' => '0', 
  );
  $variable['corporate_disk_company_requested_by_name'] = array(
    'type' => 'string',
    'title' => t('Requested by name'),
    'description' => t('This is the requested by field in Disk.com order entries.'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_cron_status'] = array(
    'title' => t('Enable cron to run.'),
    'description' => t('Boolean switch to enable or disable the cron function call.'),
    'type' => 'boolean',
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_interval'] = array(
    'title' => t('Cron Interval'),
    'description' => t('Select the interval for fulfillment to be run.'),
    'type' => 'string',
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_backtrack_time'] = array(
    'title' => t('Backtrack Interval'),
    'description' => t('Select the amount of time to backtrack on the order query that pulls in the disk orders.'),
    'type' => 'string',
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_base_sku_list'] = array(
    'title' => t('Baseline SKU List'),
    'type' => 'string',
    'description' => t('This is a Key Value pair list of the baseline product SKUs.  The keys are integer values. The values are the SKU.  These values must match what is stored in Disk.com.  <br />An example would be: 07|106-2'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_base_sku_substitution_needle'] = array(
    'title' => t('Substitution Search'),
    'type' => 'string',
    'description' => t('If we are running a substitution suffix.  What is the suffix we are looking for.  The needle in a string search'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_base_sku_substitution_haystack'] = array(
    'title' => t('Substitution Replacement'),
    'type' => 'string',
    'description' => t('What is our substitution.  <strong>This must match the patterns in the <i>Baseline SKU List</i> above or you will break this module!</strong>'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_bundle_sku_list'] = array(
    'title' => t('Bundled SKU List'),
    'type' => 'string',
    'description' => t('<b>These are processed before substitutions and hence you must manual translate the substitution here!</b><br />This is a Key Value pair list of the <b>BUNDLED</b> product SKUs.  The keys are bundle SKU. The values are the base SKUs the bundle represents delimitered by spaces.  <b>The values must match the baseline SKUs that are stored on Disk.com listed above!</b>  An example would be: <br /> 502-2|106-2 200-2'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_shipping_services'] = array(
    'title' => t('Available shipping methods'),
    'type' => 'string',
    'description' => t('<b>This is a list of the available shipping methods offered at Disk.com as they exist in their API.</b><br />One per line.'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_shipping_default_conus'] = array(
    'title' => t('Default Continental United States Shipping'),
    'description' => t('Shipping method to use when none is specified as a line item in the order.'),
    'type' => 'string',
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_shipping_default_oconus'] = array(
    'title' => t('Default International Shipping'),
    'description' => t('Shipping method to use when none is specified as a line item in the order.'),
    'type' => 'string',
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $variable['corporate_disk_company_order_view_message_footer'] = array(
    'type' => 'string',
    'title' => t('Order View Footer Message'),
    'description' => t('This message will appear below the display of the tracking numbers in the order view.  You can use HTML in this field for links.'),
    'access' => 'access corporate disk module',
    'group' => 'corporate_disk_company',
  );
  $commerce_shipping_services = commerce_shipping_services();
  if (!empty($commerce_shipping_services)) {
    foreach ($commerce_shipping_services AS $key => $value) {
      $variable['corporate_disk_company_shipping_services_' . $key] = array(
        'title' => $value['display_title'],
        'description' => t('Shipping method to use for Commerce Shipping' . ' ' . $value['display_title']),
        'type' => 'string',
        'access' => 'access corporate disk module',
        'group' => 'corporate_disk_company',
      );
    }
  }
  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function corporate_disk_company_variable_group_info() {
  $groups['corporate_disk_company'] = array(
    'title' => t('Corporate Disk Fulfillment'),
    'description' => t('Configure product fulfillment from Drupal Commerce for Corporate Disk Company.'),
    'access' => 'access corporate disk module',
    'path' => array('admin/config/services/corporatedisk'),
  );

  return $groups;
}
