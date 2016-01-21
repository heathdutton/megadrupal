<?php
require_once(__DIR__ . '/../../lib/Drupal/smartling/QueueManager/CheckStatusQueueManager.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Settings/SmartlingSettingsHandler.php');
require_once(__DIR__ . '/../../api/lib/SmartlingApi.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/ApiWrapperInterface.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/ApiWrapper/SmartlingApiWrapper.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Wrappers/DrupalAPIWrapper.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Wrappers/SmartlingEntityDataWrapper.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Wrappers/SmartlingEntityDataWrapperCollection.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/QueueManager/DownloadQueueManager.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Log/LoggerInterface.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Log/DevNullLogger.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Log/SmartlingLog.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/Wrappers/SmartlingUtils.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/SmartlingExceptions/SmartlingGenericException.php');
require_once(__DIR__ . '/../../lib/Drupal/smartling/SmartlingExceptions/SmartlingNotConfigured.php');

use Drupal\smartling\QueueManager;

/**
 * @file
 * Tests for smartling.
 */
function t($str) {
}

function url($url) {
}

/**
 * SmartlingFileCleanTest.
 */
class CheckStatusQueueManagerTest extends \PHPUnit_Framework_TestCase {

  public function setUp() {
    $this->api_wrapper = $this->getMockBuilder('\Drupal\smartling\ApiWrapper\SmartlingApiWrapper')
      ->disableOriginalConstructor()
      ->getMock();

    $this->smartling_submission_wrapper = $this->getMockBuilder('\Drupal\smartling\Wrappers\SmartlingEntityDataWrapper')
      ->disableOriginalConstructor()
      ->getMock();

    $this->submissions_collection = $this->getMockBuilder('\Drupal\smartling\Wrappers\SmartlingEntityDataWrapperCollection')
      ->disableOriginalConstructor()
      ->getMock();

    $this->queue_download = $this->getMockBuilder('\Drupal\smartling\QueueManager\DownloadQueueManager')
      ->disableOriginalConstructor()
      ->getMock();

    $this->log = $this->getMockBuilder('\Drupal\smartling\Log\SmartlingLog')
      ->disableOriginalConstructor()
      ->getMock();

    $this->smartling_utils = $this->getMockBuilder('\Drupal\smartling\Wrappers\SmartlingUtils')
      ->disableOriginalConstructor()
      ->getMock();

    $this->drupal_wrapper = $this->getMockBuilder('\Drupal\smartling\Wrappers\DrupalAPIWrapper')
      ->disableOriginalConstructor()
      ->getMock();
  }


  public function testExecuteNotConfigured() {
    $this->setExpectedException('\Drupal\smartling\SmartlingExceptions\SmartlingNotConfigured');

    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(FALSE));

    $manager = new QueueManager\CheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $manager->execute(1);
  }

  public function testExecuteEmptyStatusCheckResult() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('loadByID')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('getEntity')
      ->will($this->returnValue(new \stdClass()));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');

    $this->api_wrapper->expects($this->once())
      ->method('getStatus')
      ->will($this->returnValue(array()));

    $manager = new QueueManager\CheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $manager->execute(1);

    $this->assertEquals(TRUE, TRUE);
  }

  public function testExecuteDoNotDownloadYet1() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('loadByID')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');


    $this->smartling_submission_wrapper->expects($this->once())
      ->method('getEntity')
      ->will($this->returnValue(new \stdClass()));

    $response = new \stdClass();
    $response->approvedStringCount = 1;
    $response->completedStringCount = 2;

    $this->api_wrapper->expects($this->once())
      ->method('getStatus')
      ->will($this->returnValue(array('response_data' => $response)));

    $manager = new QueueManager\CheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $manager->execute(1);

    $this->assertEquals(TRUE, TRUE);
  }

  public function testExecuteDoNotDownloadYet2() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('loadByID')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');


    $smartling_submission = new \stdClass();
    $smartling_submission->entity_type = 'smartling_interface_entity';
    $this->smartling_submission_wrapper->expects($this->once())
      ->method('getEntity')
      ->will($this->returnValue($smartling_submission));

    $response = new \stdClass();
    $response->approvedStringCount = 2;
    $response->completedStringCount = 2;

    $this->api_wrapper->expects($this->once())
      ->method('getStatus')
      ->will($this->returnValue(array('response_data' => $response)));

    $this->queue_download->expects($this->never())
      ->method('add');


    $manager = new QueueManager\CheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $manager->execute(1);

    $this->assertEquals(TRUE, TRUE);
  }

  public function testExecuteDoDownload() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('loadByID')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');


    $smartling_submission = new \stdClass();
    $smartling_submission->entity_type = 'node';
    $this->smartling_submission_wrapper->expects($this->once())
      ->method('getEntity')
      ->will($this->returnValue($smartling_submission));

    $response = new \stdClass();
    $response->approvedStringCount = 2;
    $response->completedStringCount = 2;

    $this->api_wrapper->expects($this->once())
      ->method('getStatus')
      ->will($this->returnValue(array('response_data' => $response)));

    $this->queue_download->expects($this->once())
      ->method('add');


    $manager = new QueueManager\CheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $manager->execute(1);

    $this->assertEquals(TRUE, TRUE);
  }
}
