<?php

namespace EntityXliffFtp\Tests;

use EntityXliffFtp\Querier;

class QuerierTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the Querier constructor will throw an exception in the case
   * that an SFTP client is passed in that is not connected / logged in.
   *
   * @test
   * @expectedException \Exception
   * @expectedExceptionMessage The provided SFTP client must already be connected.
   */
  public function constructorExpectsConnection() {
    $mockClient = $this->getMockBuilder('Net_SFTP')
      ->disableOriginalConstructor()
      ->getMock();
    $querier = new Querier($mockClient);
  }

  /**
   * Tests that optional injected dependencies are dynamically instantiated if
   * not initially provided.
   *
   * @test
   */
  public function constructorInstantiatesDefault() {
    $mockClient = $this->getConnectedClientMock();
    $querier = new Querier($mockClient);
    $props = get_object_vars($querier);
    $this->assertTrue(get_class($props['drupal']) === 'EntityXliffFtp\Utils\DrupalHandler');
  }

  /**
   * Tests that Querier::getProcessable will attempt to pull default installed
   * languages from the DrupalHandler if no languages are provided.
   *
   * @test
   */
  public function getProcessableGetsDefaultInstalledLanguages() {
    $mockClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::languageList() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn(array());

    // Instantiate Querier and call getProcessable without languages.
    $querier = new Querier($mockClient, $observerDrupal);
    $querier->getProcessable();
  }

  /**
   * Tests that Querier::getProcessable will ignore English (not attempt to find
   * translated XLIFFs whose target language is English).
   *
   * @test
   */
  public function getProcessableIgnoresEnglish() {
    $observerClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::languageList() will never be called.
    $observerDrupal->expects($this->never())
      ->method('languageList');

    // We expect that Net_SFTP::rawList will never be called.
    $observerClient->expects($this->never())
      ->method('rawList');

    // Instantiate Querier and call getProcessable with ONLY English.
    $querier = new Querier($observerClient, $observerDrupal);
    $querier->getProcessable(array('en' => (object) array()));
  }

  /**
   * Tests that Querier::getProcessable will Net_SFTP::rawList the expected
   * remote path and return results in the expected format.
   *
   * @test
   */
  public function getProcessableReturnsResults() {
    $expectedSourceRoot = 'path/to/source';
    $languageList = $this->getValidLangObjects();
    $expectedFileList = array(
      'drupal-123-node-1.xlf' => array('type' => 1),
      'unmanaged-file.xlf' => array('type' => 1),
      'unfortunately_named_directory-1.xlf' => array('type' => 2),
    );
    $expectedResponse = array(
      'fr' => array('node' => array(1 => 1)),
      'de' => array('node' => array(1 => 1)),
    );

    $observerClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::variableGet() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with(Querier::SOURCEROOTVAR)
      ->willReturn($expectedSourceRoot);

    // We expect that DrupalHandler::languageList() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn($languageList);

    // We expect that Net_SFTP::rawList will be called as many times as there
    // are languages.
    $observerClient->expects($this->exactly(count($languageList)))
      ->method('rawList')
      ->withConsecutive(
        array($this->equalTo($expectedSourceRoot . '/fr')),
        array($this->equalTo($expectedSourceRoot . '/de'))
      )
      ->willReturn($expectedFileList);

    // Instantiate a querier observer.
    $observerQuerier = $this->getMockBuilder('EntityXliffFtp\Querier')
      ->setConstructorArgs(array($observerClient, $observerDrupal))
      ->setMethods(array('parseFilename'))
      ->getMock();

    // When Querier::parseFile is called, return as expected.
    $observerQuerier->expects($this->any())
      ->method('parseFilename')
      ->will($this->returnCallback(function($filename) {
        return $filename === 'drupal-123-node-1.xlf' ? array('type' => 'node', 'identifier' => 1) : array();
      }));

    // Instantiate Querier and call getProcessable with ONLY English.
    $this->assertEquals($expectedResponse, $observerQuerier->getProcessable());
  }

  /**
   * Tests that Querier::getProcessableByEntity will call the getProcessable
   * method and format the results as expected.
   *
   * @test
   */
  public function getProcessableByEntity() {
    $expectedLangs = $this->getValidLangObjects();
    $expectedResponse = array('node' => array(1 => array('fr' => 'fr', 'de' => 'de')));

    $observerQuerier = $this->getMockBuilder('EntityXliffFtp\Querier')
      ->disableOriginalConstructor()
      ->setMethods(array('getProcessable'))
      ->getMock();

    // We expect that Querier::getProcessable() will be called exactly once.
    $observerQuerier->expects($this->once())
      ->method('getProcessable')
      ->with($expectedLangs)
      ->willReturn(array(
        'fr' => array('node' => array(1 => 1)),
        'de' => array('node' => array(1 => 1)),
      ));

    // Call Querier::getProcessable(), validate expectations.
    $this->assertEquals($expectedResponse, $observerQuerier->getProcessableByEntity($expectedLangs));
  }

  /**
   * Tests that Querier::getProcessed will attempt to pull default installed
   * languages from the DrupalHandler if no languages are provided.
   *
   * @test
   */
  public function getProcessedGetsDefaultInstalledLanguages() {
    $mockClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::languageList() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn(array());

    // Instantiate Querier and call getProcessable without languages.
    $querier = new Querier($mockClient, $observerDrupal);
    $querier->getProcessed();
  }

  /**
   * Tests that Querier::getProcessed will ignore English (not attempt to find
   * processed XLIFFs whose target language is English).
   *
   * @test
   */
  public function getProcessedIgnoresEnglish() {
    $observerClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::languageList() will never be called.
    $observerDrupal->expects($this->never())
      ->method('languageList');

    // We expect that Net_SFTP::rawList will never be called.
    $observerClient->expects($this->never())
      ->method('rawList');

    // Instantiate Querier and call getProcessable with ONLY English.
    $querier = new Querier($observerClient, $observerDrupal);
    $querier->getProcessed(array('en' => (object) array()));
  }

  /**
   * Tests that Querier::getProcessed will Net_SFTP::rawList the expected
   * remote path and return results in the expected format.
   *
   * @test
   */
  public function getProcessedReturnsResults() {
    $expectedSourceRoot = 'path/to/source';
    $languageList = $this->getValidLangObjects();
    $expectedFileList = array(
      'drupal-123-node-1.xlf' => array(
        'type' => 1,
        'filename' => 'node-1.xlf',
        'size' => 12345,
        'mtime' => 98765,
        'atime' => 43210,
      ),
      'unmanaged-file.xlf' => array(
        'type' => 1
      ),
      'unfortunately_named_directory-1.xlf' => array(
        'type' => 2
      ),
    );
    $expectedResponse = array(
      'fr' => array('node' => array(1 => array(
        'filename' => 'node-1.xlf',
        'size' => 12345,
        'modified' => 98765,
        'accessed' => 43210,
      ))),
      'de' => array('node' => array(1 => array(
        'filename' => 'node-1.xlf',
        'size' => 12345,
        'modified' => 98765,
        'accessed' => 43210,
      ))),
    );

    $observerClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::variableGet() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with(Querier::SOURCEROOTVAR)
      ->willReturn($expectedSourceRoot);

    // We expect that DrupalHandler::languageList() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn($languageList);

    // We expect that Net_SFTP::rawList will be called as many times as there
    // are languages.
    $observerClient->expects($this->exactly(count($languageList)))
      ->method('rawList')
      ->withConsecutive(
        array($this->equalTo($expectedSourceRoot . '/fr/processed')),
        array($this->equalTo($expectedSourceRoot . '/de/processed'))
      )
      ->willReturn($expectedFileList);

    // Instantiate a querier observer.
    $observerQuerier = $this->getMockBuilder('EntityXliffFtp\Querier')
      ->setConstructorArgs(array($observerClient, $observerDrupal))
      ->setMethods(array('parseFilename'))
      ->getMock();

    // When Querier::parseFile is called, return as expected.
    $observerQuerier->expects($this->any())
      ->method('parseFilename')
      ->will($this->returnCallback(function($filename) {
        return $filename === 'drupal-123-node-1.xlf' ? array('type' => 'node', 'identifier' => 1) : array();
      }));

    // Instantiate Querier and call getProcessable with ONLY English.
    $this->assertEquals($expectedResponse, $observerQuerier->getProcessed());
  }

  /**
   * Tests that Querier::getProcessedByEntity will call the getProcessed
   * method and format the results as expected.
   *
   * @test
   */
  public function getProcessedByEntity() {
    $expectedLangs = $this->getValidLangObjects();
    $expectedResponse = array(
      'node' => array(
        1 => array(
          'fr' => array(
            'filename' => 'node-1.xlf',
            'size' => 12345,
            'modified' => 98765,
            'accessed' => 43210,
          ),
          'de' => array(
            'filename' => 'node-1.xlf',
            'size' => 12345,
            'modified' => 98765,
            'accessed' => 43210,
          ),
        )));

    $observerQuerier = $this->getMockBuilder('EntityXliffFtp\Querier')
      ->disableOriginalConstructor()
      ->setMethods(array('getProcessed'))
      ->getMock();

    // We expect that Querier::getProcessable() will be called exactly once.
    $observerQuerier->expects($this->once())
      ->method('getProcessed')
      ->with($expectedLangs)
      ->willReturn(array(
        'fr' => array('node' => array(1 => array(
          'filename' => 'node-1.xlf',
          'size' => 12345,
          'modified' => 98765,
          'accessed' => 43210,
        ))),
        'de' => array('node' => array(1 => array(
          'filename' => 'node-1.xlf',
          'size' => 12345,
          'modified' => 98765,
          'accessed' => 43210,
        ))),
      ));

    // Call Querier::getProcessable(), validate expectations.
    $this->assertEquals($expectedResponse, $observerQuerier->getProcessedByEntity($expectedLangs));
  }

  /**
   * Tests that Querier::parseFileName parses file names in the expected format.
   *
   * @test
   * @dataProvider fileNameProvider
   */
  public function parseFileName($filename, $expectedPrefix, $expectedResponse) {
    $mockClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect that DrupalHandler::variableGet() will be called exactly once.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with(Querier::FILEPREFIXVAR)
      ->willReturn($expectedPrefix);

    $querier = new Querier($mockClient, $observerDrupal);
    $this->assertEquals($expectedResponse, $querier->parseFilename($filename));
  }

  /**
   * Provides data for QuerierTest::parseFileName.
   *
   * @return array
   *   An array whose keys are as described:
   *   -0: The filename to be parsed.
   *   -1: The expected file prefix.
   *   -2: The expected response (an array).
   */
  public function fileNameProvider() {
    return array(
      array('drupal-123-node-123.xlf', 'drupal-123', array('type' => 'node', 'identifier' => 123)),
      array('drupal-123-taxonomy_term-999999.xlf', 'drupal-123', array('type' => 'taxonomy_term', 'identifier' => 999999)),
      array('Not-real-111.xlf', 'drupal-123', array()),
      array('drupal-123-12345-node.xlf', 'drupal-123', array()),
      array('Another file', 'drupal-123', array()),
    );
  }

  /**
   * Returns a mock SFTP client that is connected.
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getConnectedClientMock() {
    $mockClient = $this->getMockBuilder('Net_SFTP')
      ->disableOriginalConstructor()
      ->getMock();
    $mockClient->expects($this->any())
      ->method('isConnected')
      ->willReturn(TRUE);
    return $mockClient;
  }

  /**
   * Returns an array of valid Drupal language objects (or at least valid
   * insofar as MiddleWare needs it to be).
   *
   * @return object[]
   */
  protected function getValidLangObjects() {
    return array(
      'fr' => (object) array('language' => 'fr', 'name' => 'French'),
      'de' => (object) array('language' => 'de', 'name' => 'German'),
    );
  }

}
