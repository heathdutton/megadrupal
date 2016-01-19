<?php
require_once(__DIR__ . '/../../lib/Drupal/smartling/QueueManager/BulkCheckStatusQueueManager.php');
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
 * SmartlingFileCleanTest.
 */
class BulkCheckStatusQueueManagerTest extends \PHPUnit_Framework_TestCase {

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

    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $manager->execute('hi.xml');
  }

  public function testExecuteEmptyData1() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->submissions_collection->expects($this->once())
      ->method('loadByCondition')
      ->will($this->returnValue($this->submissions_collection));

    $this->submissions_collection->expects($this->once())
      ->method('getCollection')
      ->will($this->returnValue(array()));

    $this->api_wrapper->expects($this->once())
      ->method('getStatusAllLocales')
      ->will($this->returnValue(new \stdClass()));

    $this->api_wrapper->expects($this->once())
      ->method('getLastModified')
      ->will($this->returnValue(array()));


//    $this->smartling_submission_wrapper->expects($this->once())
//      ->method('save');
//
//    $this->api_wrapper->expects($this->once())
//      ->method('getStatus')
//      ->will($this->returnValue(array()));

    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $res = $manager->execute('hi.xml');

    $this->assertEquals($res, null);
  }

  public function testExecuteEmptyData2() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->submissions_collection->expects($this->once())
      ->method('loadByCondition')
      ->will($this->returnValue($this->submissions_collection));

    $this->submissions_collection->expects($this->once())
      ->method('getCollection')
      ->will($this->returnValue(array((object) array('target_language' => 'nl', 'last_modified' => 12345))));

    $this->api_wrapper->expects($this->once())
      ->method('getStatusAllLocales')
      ->will($this->returnValue(array()));

    $this->api_wrapper->expects($this->once())
      ->method('getLastModified')
      ->will($this->returnValue(array('nl' => 12346)));


    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $res = $manager->execute('hi.xml');

    $this->assertEquals($res, null);
  }

  public function testExecuteEmptyData3() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->submissions_collection->expects($this->once())
      ->method('loadByCondition')
      ->will($this->returnValue($this->submissions_collection));

    $this->submissions_collection->expects($this->once())
      ->method('getCollection')
      ->will($this->returnValue(array((object) array('target_language' => 'nl', 'last_modified' => 12345))));

    $this->api_wrapper->expects($this->once())
      ->method('getStatusAllLocales')
      ->will($this->returnValue(array('nl' => array('authorizedStringCount' => 1, 'completedStringCount' => 1))));

    $this->api_wrapper->expects($this->once())
      ->method('getLastModified')
      ->will($this->returnValue(array()));


    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $res = $manager->execute('hi.xml');

    $this->assertEquals($res, null);
  }


  public function testExecuteDontAddToDownload1() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->submissions_collection->expects($this->once())
      ->method('loadByCondition')
      ->will($this->returnValue($this->submissions_collection));

    $this->submissions_collection->expects($this->once())
      ->method('getCollection')
      ->will($this->returnValue(array((object) array('target_language' => 'nl', 'last_modified' => 12345, 'entity_type' => 'node'))));

    $this->api_wrapper->expects($this->once())
      ->method('getStatusAllLocales')
      ->will($this->returnValue(array('nl' => array('authorizedStringCount' => 1, 'completedStringCount' => 1))));

    $this->api_wrapper->expects($this->once())
      ->method('getLastModified')
      ->will($this->returnValue(array('nl' => 12345)));

    $this->queue_download->expects($this->never())
      ->method('add');

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');

    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $res = $manager->execute('hi.xml');

    $this->assertEquals($res, array());
  }

  public function testExecuteDontAddToDownload2() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->submissions_collection->expects($this->once())
      ->method('loadByCondition')
      ->will($this->returnValue($this->submissions_collection));

    $this->submissions_collection->expects($this->once())
      ->method('getCollection')
      ->will($this->returnValue(array((object) array('target_language' => 'nl', 'last_modified' => 12345, 'entity_type' => 'node'))));

    $this->api_wrapper->expects($this->once())
      ->method('getStatusAllLocales')
      ->will($this->returnValue(array('nl' => array('authorizedStringCount' => 1, 'completedStringCount' => 0))));

    $this->api_wrapper->expects($this->once())
      ->method('getLastModified')
      ->will($this->returnValue(array('nl' => 12346)));

    $this->queue_download->expects($this->never())
      ->method('add');

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');

    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $res = $manager->execute('hi.xml');

    $this->assertEquals($res, array());
  }


  public function testExecuteAddToDownload() {
    $this->smartling_utils->expects($this->once())
      ->method('isConfigured')
      ->will($this->returnValue(TRUE));

    $this->submissions_collection->expects($this->once())
      ->method('loadByCondition')
      ->will($this->returnValue($this->submissions_collection));

    $this->submissions_collection->expects($this->once())
      ->method('getCollection')
      ->will($this->returnValue(array((object) array('target_language' => 'nl', 'last_modified' => 12345, 'entity_type' => 'node'))));

    $this->api_wrapper->expects($this->once())
      ->method('getStatusAllLocales')
      ->will($this->returnValue(array('nl' => array('authorizedStringCount' => 1, 'completedStringCount' => 1))));

    $this->api_wrapper->expects($this->once())
      ->method('getLastModified')
      ->will($this->returnValue(array('nl' => 12346)));

    $this->queue_download->expects($this->once())
      ->method('add');

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('setEntity')
      ->will($this->returnValue($this->smartling_submission_wrapper));

    $this->smartling_submission_wrapper->expects($this->once())
      ->method('save');

    $manager = new QueueManager\BulkCheckStatusQueueManager($this->api_wrapper, $this->smartling_submission_wrapper, $this->submissions_collection, $this->queue_download, $this->log, $this->smartling_utils, $this->drupal_wrapper);
    $res = $manager->execute('hi.xml');

    $this->assertEquals($res, array());
  }

}
