<?php
/**
 * @file
 * Hooks provided by the OAI Harvester module.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @addtogroup global variables
 * @{
 */

/**
 * The collector of the time based statistics of the harvesting.
 * @var (array)
 */
global $_oaiharvester_statistics;

/**
 * Flag indicates whether or not the current harvesting process is an initial
 * harvesting or an incremental one.
 * @var (boolean)
 */
global $_oaiharvester_is_initial_harvest;

/**
 * Flag indicates whether the harvesting should produce some debugging log entries.
 * @var (boolean)
 */
global $_oaiharvester_need_debug;
/**
 * @} End of "addtogroup global variables".
 */


/**
 * @addtogroup hooks
 * @{
 */

/**
 * Process a harvested record
 *
 * This hook is triggered inside an iteration of every harvested records, so it
 * calls on each record sequentially. If you want to do yomething with the
 * record (usually: storing, indexing for search), implement this hook. The
 * record is a complex structure: it is an array created from the XML <record>
 * element of OAI-PMH response. The actual metadata part is built as DOMElement.
 *
 * @param array $record
 *   The harvested record in an OAI-PMH response to the ListRecords verb request
 *   The record is a complex array with the following internal structure:
 *   <ul>
 *     <li>'header' The header part of the record
 *       <ul>
 *         <li>'identifier' The record identifier</li>
 *         <li>'datestamp' The time of the last modification or the creation</li>
 *         <li>'setSpec' The identifier of the sets in which the record take place</li>
 *         <li>'status' Optional information, contains 'delete' if the record was deleted on the repository</li>
 *        </ul>
 *     </li>
 *     <li>'about' Optional and repeatable container to hold data about the metadata part of the record</li>
 *     <li>'metadata' The metadata part of the record. It could be in one of several metadata formats (like
 *                    Dublin Core, MARCXML, EAD etc.)
 *       <ul>
 *         <li>'namespaceURI' The URI of namespace of the metadata format</li>
 *         <li>'childNode' The content of the metadata. It is in DOM object</li>
 *       </ul>
 *     </li>
 *   </ul>
 * @param int $schedule_id
 *   The schedule's ID
 */
function hook_oaiharvester_process_record($record, $schedule_id) {
  // processing the record
}

/**
 * Respond to the event that a single OAI request is issued
 *
 * @param $parameters (Array)
 *   The OAI request parameters
 */
function hook_oaiharvester_request_started($parameters) {

}

/**
 * Respond to the event that a single OAI request has been processes.
 *
 * This hook is triggered after a single OAI request processed. Do not confuse
 * with hook_oaiharvester_harvest_finished which is triggered after all requests
 * are processed for a given initial URL.
 *
 * @param $parameters (array)
 *   The parameters of the harvesting schedule
 * @param $sandbox (array)
 *
 */
function hook_oaiharvester_request_processed($parameters, $sandbox) {
  // the request was processed
}

/**
 * Respond to the event, that the harvest was finished.
 *
 * Triggered after a schedule is finished (sucessfully or not). A schedule may
 * contain multiple initial URLs.
 *
 * @param $success (Boolean)
 *   Boolean value designating the success of the harvesting
 * @param $results (Array)
 *   An array containing information about the process
 * @param $operations (Array)
 *   Currently unused parameter
 */
function hook_oaiharvester_harvest_finished($success, $results, $operations) {

}

/**
 * Add one or more additional batch definitions you wish to run after the harvesting
 * process.
 *
 * If your module would like to add additional task into the harvesting process
 * which will run after the schedule, it can be done with this hook. The structure
 * is similar as the input parameter of the batch_set() function. The oaiharvester
 * module will use only the operations. To keep track of the whole batch process,
 * oaiharvester module will add $saved_batch_id (the identifyer of the
 * oaiharvester_batch record, which stores information about the harvest) and
 * $operation_id (the count number of the function among all steps) as additional
 * parameters for the original functions, so if you implements additional steps,
 * please add this two parameters to the subscription of your functions.
 *
 * @see batch_set
 *
 * @link http://api.drupal.org/api/drupal/includes--form.inc/function/batch_set/6
 *
 * @param $schedule_id (int)
 *   The identifier of the schedule. Drupal adds batch sets to this schedule.
 *   The sets will run after the schedule's main operations.
 *
 * @return (array)
 *   An array of Batch definitions
 */
function hook_oaiharvester_additional_harvest_steps($schedule_id) {
  //
  return array(
    array(
      'operations' => array(
        array($function1, $param1_1, $param1_2),
        array($function2, $param2_1, $param2_2),
      ),
      'title' => $title1,
      'init_message' => $init_message1,
      'progress_message' => $progress_message1,
      'finished' => $finishing_function1,
    ),
    array(
      'operations' => array(
        array($function3, $param3_1, $param3_2),
        array($function4, $param4_1, $param4_2),
      ),
      'title' => $title2,
      'init_message' => $init_message2,
      'progress_message' => $progress_message2,
      'finished' => $finishing_function2,
    ),
  );
}

/**
 * Respond to the event that harvest was started
 *
 * @param $schedule_id (int)
 *   The schedule ID
 */
function hook_oaiharvester_harvest_starting($schedule_ids) {

}

/**
 * Respond to the event that a batch has been started
 *
 * @param $parameters (array)
 *   The harvest parameters.
 */
function hook_oaiharvester_batch_started($parameters) {

}

/**
 * Triggered after a step (a pair of set-metadata format) has been harvested.
 * OAI-PMH data providers might split the collections into parts, for example
 * a library may have 'books', 'journals'. The OAI-PMH protocoll calls them 'sets'.
 * According to the protocoll it is not possible to request multiple sets in one
 * harvest, so if you want to harvest multiple sets, Drupal Toolkit will create
 * "steps" at the background. Each steps harvests one set. So if the admin
 * select "books" and "journals", Drupal will harvest books, then trigger this
 * hook, then harvest journals, and trigger this hook again, and finally, since
 * this was the last step it calls hook_oaiharvester_batch_processed.
 *
 * @param $has_errors (boolean)
 *   Flag denoting whether the harvest had errors.
 * @param $original_parameters (array)
 *   The associative array of the harvest parameters.
 * @param $start_time (int)
 *   Timestamp of harvest start time.
 */
function hook_oaiharvester_step_processed($has_errors, $original_parameters, $start_time) {

}

/**
 * Triggered after the whole harvester batch has been processed (all steps of
 * the sets-metadata formats pairs has been harvested). So this hook is invoked
 * only one time contrary to hook_oaiharvester_step_processed which is called
 * multiple time if there were multiple set-format pairs.
 *
 * @param $sets (array)
 *   All the batch sets. Each set is an array with two elements, where the first
 *   element is the name of the function, the second element is the list of
 *   parameters as an array.
 * @param $current_operation_id (int)
 *   The current operation's identifier, which actually is the index of the operation in the sets array.
 * @param $schedule_id (int)
 *   The schedule ID
 */
function hook_oaiharvester_batch_processed($sets, $current_operation, $schedule_id) {

}

/**
 * Add additional rows to the properties page of the schedule.
 *
 * When you made modification of the schedule's form, you would like to view
 * your properties and their values at the schedule properties page
 * (admin/xc/harvester/schedule/%schedule_id). This hook returns additional properties
 * of the schedule as an array of label-value arrays.
 *
 * @param $schedule_id (int)
 *   The identifier of the schedule
 *
 * @return (array)
 *   List of label-values pairs
 */
function hook_oaiharvester_schedule_view($schedule_id) {
  //
  return array(
    array(t('Storage locations'), theme('item_list', array('items' => $location_links))),
    array(t('Is Solr running?'), theme('item_list', array('items' => $ping_report))),
    array(t("Run 'preparing metadata for search' step?"), $steps_label),
  );
}

/**
 * Extends the harvest report with additional information. Called when a report is
 * beeing displayed. A module can add additional keys to oaiharvester_batch
 * record's "report" column (which is an associative array) during the harvest.
 * The main purpose of this function is to format those raw data.
 *
 * @param $reports (array)
 *   The content of oaiharvester_batch record's "report" column.
 */
function hook_oaiharvester_harvest_report_view($reports) {

}
/**
 * @} End of "addtogroup hooks".
 */
