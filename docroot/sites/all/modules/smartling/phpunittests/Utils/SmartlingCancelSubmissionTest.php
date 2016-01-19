<?php
require_once(__DIR__ . '/../../lib/Drupal/smartling/Utils/SmartlingCancelSubmission.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Settings/SmartlingSettingsHandler.php');
require_once(__DIR__ . '/../../api/lib/SmartlingApi.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/ApiWrapperInterface.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/ApiWrapper/SmartlingApiWrapper.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Wrappers/SmartlingEntityDataWrapper.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/QueueManager/UploadQueueManager.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Log/LoggerInterface.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Log/DevNullLogger.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Log/SmartlingLog.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/SmartlingExceptions/SmartlingGenericException.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/SmartlingExceptions/SmartlingNotConfigured.php');

use Drupal\smartling\Utils;

define('SMARTLING_STATUS_PENDING_CANCEL', 5);
define('SMARTLING_STATUS_CANCELED', 5);

class SmartlingCancelSubmission_EmptyQueue extends Utils\SmartlingCancelSubmission {
  protected function getPreCanceledSubmissions($limit) {
    return array();
  }
}

class SmartlingCancelSubmission_Queue1 extends Utils\SmartlingCancelSubmission {
  protected function getPreCanceledSubmissions($limit) {
    return array((object) array('rid' => 1, 'entity_type' => 'node'));
  }

  protected function getSubmissionsByCondition($conditions) {
    return array(
      (object) array(
        'status' => 5,
        'eid' => 1,
        'file_name' => 'hi.xml'
      )
    );
  }
}

class SmartlingCancelSubmission_Queue2 extends Utils\SmartlingCancelSubmission {
  protected function getPreCanceledSubmissions($limit) {
    return array((object) array('rid' => 1, 'entity_type' => 'node'));
  }

  protected function getSubmissionsByCondition($conditions) {
    return array(
      (object) array(
        'status' => 1,
        'eid' => 1,
        'file_name' => 'hi.xml'
      )
    );
  }
}

class SmartlingCancelSubmission_Queue3 extends Utils\SmartlingCancelSubmission {
  protected function getPreCanceledSubmissions($limit) {
    return array((object) array('rid' => 1, 'entity_type' => 'node'));
  }

  protected function getSubmissionsByCondition($conditions) {
    return array(
      (object) array(
        'status' => 1,
        'eid' => 1,
        'file_name' => 'hi.xml'
      ),
      (object) array('status' => 5, 'eid' => 1, 'file_name' => 'hi.xml')
    );
  }
}

/**
 * SmartlingFileCleanTest.
 */
class SmartlingCancelSubmissionTest extends \PHPUnit_Framework_TestCase {

  public function setUp() {
    //$log, $settings, $api_wrapper, $upload_queue, $submission_wrapper
    $this->log = $this->getMockBuilder('\Drupal\smartling\Log\SmartlingLog')
      ->disableOriginalConstructor()
      ->getMock();

    $this->settings = $this->getMockBuilder('\Drupal\smartling\Settings\SmartlingSettingsHandler')
      ->disableOriginalConstructor()
      ->getMock();

    $this->api_wrapper = $this->getMockBuilder('\Drupal\smartling\ApiWrapper\SmartlingApiWrapper')
      ->disableOriginalConstructor()
      ->getMock();

    $this->queue_upload = $this->getMockBuilder('\Drupal\smartling\QueueManager\UploadQueueManager')
      ->disableOriginalConstructor()
      ->getMock();

    $this->smartling_submission_wrapper = $this->getMockBuilder('\Drupal\smartling\Wrappers\SmartlingEntityDataWrapper')
      ->disableOriginalConstructor()
      ->getMock();

  }


  public function testCancelNoItems() {
    $canceler = new SmartlingCancelSubmission_EmptyQueue($this->log, $this->settings, $this->api_wrapper, $this->queue_upload, $this->smartling_submission_wrapper);
    $res = $canceler->cancel(3);

    $this->assertEquals(NULL, $res);
  }

  public function testCancelSomeItems1() {
    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setStatus')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');

    $this->api_wrapper->expects($this->once())
      ->method('deleteFile');

    $canceler = new SmartlingCancelSubmission_Queue1($this->log, $this->settings, $this->api_wrapper, $this->queue_upload, $this->smartling_submission_wrapper);
    $res = $canceler->cancel(3);

    $this->assertEquals(NULL, $res);
  }

  public function testCancelSomeItems2() {
    $this->smartling_submission_wrapper->expects($this->never())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->never())
      ->method('setStatus')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->never())
      ->method('save');

    $this->api_wrapper->expects($this->never())
      ->method('deleteFile');

    $this->settings->expects($this->once())
      ->method('setOverwriteApprovedLocales');

    $this->queue_upload->expects($this->once())
      ->method('execute');

    $canceler = new SmartlingCancelSubmission_Queue2($this->log, $this->settings, $this->api_wrapper, $this->queue_upload, $this->smartling_submission_wrapper);
    $res = $canceler->cancel(3);

    $this->assertEquals(NULL, $res);
  }

  public function testCancelSomeItems3() {
    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setStatus')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');

    $this->api_wrapper->expects($this->never())
      ->method('deleteFile');

    $this->settings->expects($this->once())
      ->method('setOverwriteApprovedLocales');

    $this->queue_upload->expects($this->once())
      ->method('execute');

    $canceler = new SmartlingCancelSubmission_Queue3($this->log, $this->settings, $this->api_wrapper, $this->queue_upload, $this->smartling_submission_wrapper);
    $res = $canceler->cancel(3);

    $this->assertEquals(NULL, $res);
  }
}
