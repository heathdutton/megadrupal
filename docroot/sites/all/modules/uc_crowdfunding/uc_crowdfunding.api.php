<?php
/**
 * @file
 * Documents hooks provided by Ubercart Crowdfunding Feature module.
 *
 * By Arlina E. Rhoton ("Arlina", http://drupal.org/user/1055344)
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the crowdfunding properties added to a node.
 * 
 * This hook is used to alter the crowdfunding properties (sold, gross, income)
 * added to a node object.
 *
 * @param int $nid
 *   Node ID.
 * 
 * @param object $report
 *   An object with the following properties:
 *   - nid: Node ID.
 *   - sold: The quantity this product has sold.
 *   - gross: The gross income this product has generated.
 *   - revenue: The total revenue this product has generated.
 * 
 * @return object
 *   The $report object.
 */
function hook_uc_cf_product_report($nid, &$report) {
  // Optional alterations to $report.
  return $report;
}


/**
 * @} End of "addtogroup hooks".
 */
