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
      // Listings.
      '' => array(
        \RestfulInterface::POST => 'reportConversation',
      ),
    );
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
      throw new \RestfulBadRequestException(format_string("You're either missing a required value or something that you're passing in is incorrect. @data", array('@data' => print_r($request_data, TRUE))));
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
    $wrapper->field_cs_pm_report_conversation->set(array($conversation_id));
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
