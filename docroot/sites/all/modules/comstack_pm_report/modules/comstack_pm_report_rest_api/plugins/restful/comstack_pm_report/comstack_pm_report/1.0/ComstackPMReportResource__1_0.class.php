<?php

/**
 * @file
 * Contains ComstackPMReportResource__1_0.
 */

class ComstackPMReportResource__1_0 extends \ComstackRestfulEntityBase {
  /**
   * Overrides \RestfulEntityBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        // GET returns a list of reports.
        \RestfulInterface::GET => 'getList',
        \RestfulInterface::HEAD => 'getList',
        // Create a new report.
        \RestfulInterface::POST => 'reportConversation',
      ),
      '^.*$' => array(
        \RestfulInterface::GET => 'viewEntities',
        \RestfulInterface::HEAD => 'viewEntities',
        \RestfulInterface::PUT => 'putEntity',
        \RestfulInterface::PATCH => 'patchEntity',
        \RestfulInterface::DELETE => 'deleteEntity',
      ),
    );
  }

  /**
   * Overrides \ComstackRestfulEntityBase::getList().
   *
   * Before we offer up a listing first check that the user can view reports
   * in some way.
   */
  public function getList() {
    $account = $this->getAccount();

    if (!user_access('view comstack_pm_report entries', $account) && !user_access('view own comstack_pm_report entries', $account)) {
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(t('You do not have access to view reports.'));
    }

    return parent::getList();
  }

  /**
   * Overrides \RestfulEntityBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    // Reorder things.
    $id_field = $public_fields['id'];
    unset($public_fields['id']);

    $public_fields['type'] = array(
      'callback' => array('\RestfulManager::echoMessage', array('comstack_pm_report')),
    );

    $public_fields['id'] = $id_field;

    $public_fields['reporter'] = array(
      'property' => 'uid',
      'resource' => array(
        'user' => array(
          'name' => 'cs/users',
          'full_view' => TRUE,
        ),
      ),
    );

    $public_fields['conversation'] = array(
      'property' => 'field_cs_pm_report_conversation',
      'resource' => array(
        'cs_pm' => array(
          'name' => 'cs-pm/conversations',
          'full_view' => TRUE,
        ),
      ),
    );

    $public_fields['reasons'] = array(
      'property' => 'field_cs_pm_report_reason',
    );

    $public_fields['additional_information'] = array(
      'property' => 'field_cs_pm_report_additional',
    );

    $public_fields['created'] = array(
      'property' => 'created',
      'process_callbacks' => array(
        'date_iso8601',
      ),
    );

    $public_fields['updated'] = array(
      'property' => 'updated',
      'process_callbacks' => array(
        'date_iso8601',
      ),
    );

    $public_fields['dismissed'] = array(
      'property' => 'dismissed',
    );

    unset($public_fields['label']);
    unset($public_fields['self']);

    return $public_fields;
  }

  /**
   * Report a conversation.
   *
   * @throws RestfulNotFoundException.
   * @throws RestfulForbiddenException.
   * @throws RestfulBadRequestException.
   */
  public function reportConversation() {
    $account = $this->getAccount();

    // First check access to report a conversation.
    if (!user_access('create comstack_pm_report entries', $account)) {
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(t('You do not have access to report conversations.'));
    }

    // Validate that all of the required data is present.
    $request_data = $this->getRequestData();

    if (
      empty($request_data['conversation_id']) ||
      empty($request_data['reasons']) ||
      !empty($request_data['conversation_id']) && !ctype_digit((string) $request_data['conversation_id']) ||
      !empty($request_data['reasons']) && !is_array($request_data['reasons']) ||
      !empty($request_data['other_reason']) && !is_string($request_data['other_reason']) ||
      !empty($request_data['posts']) && !is_array($request_data['posts'])
    ) {
      $this->setHttpHeaders('Status', 400);
      watchdog('comstack_pm_report', 'Attempted to create a report with invalid or missing values. @values', array('@data' => print_r($request_data, TRUE)), WATCHDOG_ERROR);
      throw new \RestfulBadRequestException(t("You're either missing a required value or something that you're passing in is incorrect."));
    }

    // Try and load up the conversation.
    $conversation_id = $request_data['conversation_id'];
    $conversation = comstack_conversation_load($conversation_id, $account->uid);
    if (!$conversation) {
      $this->setHttpHeaders('Status', 404);
      throw new RestfulNotFoundException(t("Can't seem to find the conversation that you're trying to report."));
    }

    // Create the report
    $report = entity_create('comstack_pm_report', array(
      'uid' => $account->uid,
      'created' => REQUEST_TIME,
      'updated' => REQUEST_TIME,
    ));

    // Set the report entity values.
    $wrapper = entity_metadata_wrapper('comstack_pm_report', $report);
    $wrapper->field_cs_pm_report_conversation->set($conversation_id);
    $wrapper->field_cs_pm_report_reason->set($request_data['reasons']);

    if (!empty($request_data['other_reason'])) {
      $wrapper->field_cs_pm_report_additional->set($request_data['other_reason']);
    }

    if (!empty($request_data['posts'])) {
      $wrapper->field_cs_pm_messages->set($request_data['posts']);
    }

    // Save the report.
    $response = entity_save('comstack_pm_report', $report);

    // Set the response.
    if ($response) {
      $this->setHttpHeaders('Status', 200);
    }
    else {
      $this->setHttpHeaders('Status', 400);
      throw new \RestfulBadRequestException('Something has gone wrong reporting the conversation.');
    }
  }
}
