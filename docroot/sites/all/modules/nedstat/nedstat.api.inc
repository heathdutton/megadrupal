<?php


/**
 * Implements hook_nedstat_countername_alter
 * This function allows you to alter the countername
 *
 * @param countername
 *
 * @return void
 */
function hook_nedstat_countername_alter(&$countername) {
  
}


/**
 * Implements hook_nedstat_labels()
 *
 * With this hook, you can define labels and their options
 * This function requires an associative array to be returned
 *
 * return array(
 *   '0' => array(
 *     'name' => 'labelname',
 *     'machinename' => 'machinename of this labelname',
 *     'description' => 'description of this label',
 *     'options => array(
 *       '0' => array(
 *         'name' => 'option_1',
 *         'is_default' => FALSE,
 *       ),
 *       '1' => array(
 *         'name' => 'option_2',
 *         'is_default' => TRUE,
 *       ),
 *       '2' => array(
 *         'name' => 'option_3',
 *         'is_default' => FALSE,
 *       ),
 *       ...
 *       'default' => 'option_1'
 *     ),
 *   ),
 *   '1' => array(
 *     'name' => 'labelname',
 *     'machinename' => 'machinename of this labelname',
 *     'description' => 'description of this label',
 *     'options => array(
 *       '0' => array(
 *         'name' => 'option_1',
 *         'is_default' => TRUE,
 *       ),
 *       '1' => array(
 *         'name' => 'option_2',
 *         'is_default' => FALSE,
 *       ),
 *       '2' => array(
 *         'name' => 'option_3',
 *         'is_default' => FALSE,
 *       ),
 *       ...
 *       'default' => 'option_2'
 *     ),
 *   ),
 *   ...
 * );
 *
 * @return void
 */
function hook_nedstat_labels() {
  
}

