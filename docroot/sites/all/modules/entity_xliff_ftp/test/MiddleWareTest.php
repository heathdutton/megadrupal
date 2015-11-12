<?php

namespace EntityXliffFtp\Tests;

use EntityXliffFtp\MiddleWare;

class MiddleWareTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the MiddleWare constructor will throw an exception in the case
   * that an SFTP client is passed in that is not connected / logged in.
   *
   * @test
   * @expectedException \Exception
   * @expectedExceptionMessage The provided SFTP client must already be connected.
   */
  public function constructorExpectsConnection() {
    $mockWrapper = $this->getWrapperMock();
    $mockClient = $this->getMockBuilder('Net_SFTP')
      ->disableOriginalConstructor()
      ->getMock();

    $middleware = new MiddleWare($mockClient, $mockWrapper);
  }

  /**
   * Tests that MiddleWare::putXliffs will attempt to pull default installed
   * languages from the DrupalHandler if no languages are provided.
   *
   * @test
   */
  public function putXliffsGetsDefaultInstalledLanguages() {
    $mockWrapper = $this->getWrapperMock(array('type', 'getIdentifier'));
    $mockClient = $this->getConnectedClientMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn(array('en' => (object) array()));

    // Instantiate MiddleWare and invoke MiddleWare::putXliffs()
    $middleWare = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $middleWare->putXliffs();
  }

  /**
   * Tests that MiddleWare::putXliffs will ignore English (not upload English-
   * to-English XLIFF files).
   *
   * @test
   */
  public function putXliffsIgnoresEnglish() {
    $mockWrapper = $this->getWrapperMock();
    $mockClient = $this->getConnectedClientMock();
    $mockDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getXliff', 'putXliff', 'getFilename'))
      ->setConstructorArgs(array($mockClient, $mockWrapper, NULL, $mockDrupal))
      ->getMock();

    // The getXliff and putXliff methods should never be called.
    $mockMiddleWare->expects($this->never())
      ->method('getXliff');
    $mockMiddleWare->expects($this->never())
      ->method('putXliff');

    // Invoke MiddleWare::putXliffs on our test double.
    $mockMiddleWare->putXliffs(array('en' => (object) array()));
  }

  /**
   * Tests that MiddleWare::putXliffs will get XLIFF data and Net_SFTP::put
   * that data to the expected location.
   *
   * @test
   */
  public function putXliffsGetsAndPutsXliffData() {
    $expectedXlfData = '<xml></xml>';
    $expectedLangPathBase = 'en_to_';
    $expectedFilename = 'file.xlf';

    $mockWrapper = $this->getWrapperMock();
    $mockClient = $this->getConnectedClientMock();
    $mockDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getXliff', 'putXliff', 'getFilename'))
      ->setConstructorArgs(array($mockClient, $mockWrapper, NULL, $mockDrupal))
      ->getMock();

    // Ensure getFilename just returns the filename.
    $mockMiddleWare->expects($this->once())
      ->method('getFilename')
      ->willReturn($expectedFilename);

    // The getXliff method should be called with 'fr' and 'de' in that order.
    $mockMiddleWare->expects($this->exactly(2))
      ->method('getXliff')
      ->withConsecutive(array($this->equalTo('fr')), array($this->equalTo('de')))
      ->willReturn($expectedXlfData);

    // The putXliff file should be called with expected data and params.
    $mockMiddleWare->expects($this->exactly(2))
      ->method('putXliff')
      ->withConsecutive(
        array($expectedXlfData, $expectedLangPathBase . 'fr', $expectedFilename),
        array($expectedXlfData, $expectedLangPathBase . 'de', $expectedFilename)
      )
      ->willReturn(FALSE);

    // Invoke MiddleWare::putXliffs on our test double.
    $mockMiddleWare->putXliffs($this->getValidLangObjects());
  }

  /**
   * Tests that MiddleWare::putXliffs will set success messages for each
   * valid FTP put command run.
   *
   * @test
   */
  public function putXliffsSetsDrupalMessages() {
    $expectedMessage = 'Success message.';

    $mockWrapper = $this->getWrapperMock(array('type', 'label'));
    $mockClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // The t method should be called twice with the expected values.
    $observerDrupal->expects($this->exactly(2))
      ->method('t')
      ->with($this->equalTo('Successfully uploaded @language XLIFF file for @type %label'))
      ->willReturn($expectedMessage);

    // The setMessage method should be called twice with the expected values.
    $observerDrupal->expects($this->exactly(2))
      ->method('setMessage')
      ->with($this->equalTo($expectedMessage), $this->equalTo('status'));

    // Build a mock double for MiddleWare (to inject observers on itself).
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getXliff', 'putXliff', 'getFilename'))
      ->setConstructorArgs(array($mockClient, $mockWrapper, $observerDrupal))
      ->getMock();

    // Force MiddleWare::putXliff to return TRUE so setMessage is called.
    $mockMiddleWare->expects($this->any())
      ->method('putXliff')
      ->willReturn(TRUE);

    // Invoke MiddleWare::putXliffs on our test double.
    $mockMiddleWare->putXliffs($this->getValidLangObjects());
  }

  /**
   * Tests that MiddleWare::putXliff DOES NOT call NetSFTP::put. Also ensures
   * that a Drupal message is set.
   *
   * @test
   */
  public function putXliffTargetDoesNotExist() {
    $translatedMessage = 'Translated no target directory message.';
    $expectedResponse = FALSE;
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the SFTP client.
    $observerClient = $this->getConnectedClientMock();

    // The client's put method should never be called.
    $observerClient->expects($this->never())
      ->method('put');

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array(
      'variableGet',
      'setMessage',
      't',
    ));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with($this->equalTo(MiddleWare::TARGETROOTVAR), $this->equalTo(FALSE))
      ->willReturn($expectedResponse);

    $observerDrupal->expects($this->once())
      ->method('t')
      ->with($this->equalTo('No target directory is configured.'))
      ->willReturn($translatedMessage);

    // The DrupalHandler observer expects the setMessage method to be called.
    $observerDrupal->expects($this->once())
      ->method('setMessage')
      ->with($this->equalTo($translatedMessage), $this->equalTo('error'));

    // Instantiate MiddleWare and invoke MiddleWare::putXliff().
    $middleware = new MiddleWare($observerClient, $mockWrapper, $observerDrupal);
    $this->assertSame($expectedResponse, $middleware->putXliff(NULL, NULL, NULL));
  }

  /**
   * Tests that MiddleWare::putXliff calls Net_SFTP::put with the values that
   * the SFTP client expects.
   *
   * @test
   */
  public function putXliffTargetExists() {
    $expectedXlfData = '<xml></xml>';
    $expectedTarget = '/path/to/target';
    $expectedLangPath = 'en-US_to_ja-JP';
    $expectedFilename = 'file.xlf';
    $expectedFullPath = $expectedTarget . '/' . $expectedLangPath . '/' . $expectedFilename;
    $expectedResponse = TRUE;

    $observerClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('variableGet'));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with($this->equalTo(MiddleWare::TARGETROOTVAR), $this->equalTo(FALSE))
      ->willReturn($expectedTarget);

    // The client observer expects the put method to be called.
    $observerClient->expects($this->once())
      ->method('put')
      ->with($this->equalTo($expectedFullPath), $this->equalTo($expectedXlfData))
      ->willReturn($expectedResponse);

    // Instantiate MiddleWare and invoke MiddleWare::putXliff().
    $middleware = new MiddleWare($observerClient, $mockWrapper, $observerDrupal);
    $this->assertSame($expectedResponse, $middleware->putXliff($expectedXlfData, $expectedLangPath, $expectedFilename));
  }


  /**
   * Tests that MiddleWare::setXliffs will attempt to pull default installed
   * languages from the DrupalHandler if no languages are provided.
   *
   * @test
   */
  public function setXliffsGetsDefaultInstalledLanguages() {
    $mockWrapper = $this->getWrapperMock(array('type', 'getIdentifier'));
    $mockClient = $this->getConnectedClientMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('languageList'));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn(array('en' => (object) array()));

    // Instantiate MiddleWare and invoke MiddleWare::setXliffs()
    $middleWare = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $middleWare->setXliffs();
  }

  /**
   * Tests that MiddleWare::setXliffs will ignore English (not pull English
   * XLIFF files).
   *
   * @test
   */
  public function setXliffsIgnoresEnglish() {
    $mockWrapper = $this->getWrapperMock();
    $mockClient = $this->getConnectedClientMock();
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getProcessedXliff', 'setXliff'))
      ->setConstructorArgs(array($mockClient, $mockWrapper))
      ->getMock();

    // The getXliff and putXliff methods should never be called.
    $mockMiddleWare->expects($this->never())
      ->method('getProcessedXliff');
    $mockMiddleWare->expects($this->never())
      ->method('setXliff');

    // Invoke MiddleWare::setXliffs on our test double.
    $mockMiddleWare->setXliffs(array('en' => (object) array()));
  }

  /**
   * Tests that MiddleWare::setXliffs will Net_SFTP::get XLIFF data and run the
   * unserialization process on the returned data.
   *
   * @test
   */
  public function setXliffsGetsAndSetsXliffData() {
    $expectedXlfData = '<xml></xml>';

    $mockWrapper = $this->getWrapperMock(array('type', 'label'));
    $mockClient = $this->getConnectedClientMock();
    $mockDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getProcessedXliff', 'setXliff'))
      ->setConstructorArgs(array($mockClient, $mockWrapper, $mockDrupal))
      ->getMock();

    // The getProcessedXliff method should be called with 'fr' and 'de' in that order.
    $mockMiddleWare->expects($this->exactly(2))
      ->method('getProcessedXliff')
      ->withConsecutive(array($this->equalTo('fr')), array($this->equalTo('de')))
      ->willReturn($expectedXlfData);

    // The setXliff file should be called with expected data and params.
    $mockMiddleWare->expects($this->exactly(2))
      ->method('setXliff')
      ->withConsecutive(
        array($expectedXlfData, 'fr'),
        array($expectedXlfData, 'de')
      )
      ->willReturn(FALSE);

    // Invoke MiddleWare::putXliffs on our test double.
    $mockMiddleWare->setXliffs($this->getValidLangObjects());
  }

  /**
   * Tests that MiddleWare::setXliffs will set success messages for each
   * valid unserialization attempt.
   *
   * @test
   */
  public function setXliffsSetsFailureMessages() {
    $expectedMessage = 'Failure message.';

    $mockWrapper = $this->getWrapperMock(array('type', 'label'));
    $mockClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // The t method should be called twice with the expected values.
    $observerDrupal->expects($this->exactly(2))
      ->method('t')
      ->with($this->equalTo('Problem encountered while processing @language translation for @type %label from the remote server.'))
      ->willReturn($expectedMessage);

    // The setMessage method should be called twice with the expected values.
    $observerDrupal->expects($this->exactly(2))
      ->method('setMessage')
      ->with($this->equalTo($expectedMessage), $this->equalTo('error'));

    // Build a mock double for MiddleWare (to inject observers on itself).
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getProcessedXliff', 'setXliff'))
      ->setConstructorArgs(array($mockClient, $mockWrapper, $observerDrupal))
      ->getMock();

    // Force MiddleWare::setXliff to return TRUE so setMessage is called.
    $mockMiddleWare->expects($this->any())
      ->method('setXliff')
      ->willReturn(FALSE);

    // Invoke MiddleWare::putXliffs on our test double.
    $mockMiddleWare->setXliffs($this->getValidLangObjects());
  }


  /**
   * Tests that MiddleWare::setXliffs will set failure messages for each
   * failed unserialization attempt.
   *
   * @test
   */
  public function setXliffsSetsSuccessMessages() {
    $expectedMessage = 'Success message.';

    $mockWrapper = $this->getWrapperMock(array('type', 'label'));
    $mockClient = $this->getConnectedClientMock();
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // The t method should be called twice with the expected values.
    $observerDrupal->expects($this->exactly(2))
      ->method('t')
      ->with($this->equalTo('Successfully processed @language translation for @type %label from the remote server.'))
      ->willReturn($expectedMessage);

    // The setMessage method should be called twice with the expected values.
    $observerDrupal->expects($this->exactly(2))
      ->method('setMessage')
      ->with($this->equalTo($expectedMessage), $this->equalTo('status'));

    // Build a mock double for MiddleWare (to inject observers on itself).
    $mockMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setMethods(array('getProcessedXliff', 'setXliff'))
      ->setConstructorArgs(array($mockClient, $mockWrapper, $observerDrupal))
      ->getMock();

    // Force MiddleWare::setXliff to return TRUE so setMessage is called.
    $mockMiddleWare->expects($this->any())
      ->method('setXliff')
      ->willReturn(TRUE);

    // Invoke MiddleWare::putXliffs on our test double.
    $mockMiddleWare->setXliffs($this->getValidLangObjects());
  }

  /**
   * Tests that MiddleWare::setXliffs will not attempt unserialization if no
   * translatable is found for the given entity wrapper.
   *
   * @test
   */
  public function setXliffNoTranslatable() {
    $mockClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('entityXliffSetXliff'));
    $observerDrupal->expects($this->once())
      ->method('entityXliffSetXliff')
      ->with($this->equalTo($mockWrapper), $this->equalTo(NULL), $this->equalTo(NULL))
      ->willReturn(array());

    // Instantiate a MiddleWare instance and call setXliff().
    $middleware = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $this->assertSame(FALSE, $middleware->setXliff(NULL, NULL));
  }

  /**
   * Tests that MiddleWare::setXliff will return FALSE in the even that XLIFF
   * unserialization fails.
   *
   * @test
   */
  public function setXliffUnserializationFailure() {
    $expectedLang = 'ja';
    $expectedXlfData = '<xml></xml>';

    $mockClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('entityXliffSetXliff'));
    $observerDrupal->expects($this->once())
      ->method('entityXliffSetXliff')
      ->with($this->equalTo($mockWrapper), $this->equalTo($expectedLang), $this->equalTo($expectedXlfData))
      ->willReturn(array());

    // Instantiate a MiddleWare instance and call setXliff().
    $middleware = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $this->assertSame(FALSE, $middleware->setXliff($expectedXlfData, $expectedLang));
  }

  /**
   * Tests that MiddleWare::setXliff will unserialize data as expected and call
   * the MiddleWare::setProcessed method upon successful completion.
   *
   * @test
   */
  public function setXliff() {
    $expectedLang = 'ja';
    $expectedXlfData = '<xml></xml>';
    $expectedResponse = TRUE;

    $mockClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('entityXliffSetXliff'));
    $observerDrupal->expects($this->once())
      ->method('entityXliffSetXliff')
      ->with($this->equalTo($mockWrapper), $this->equalTo($expectedLang), $this->equalTo($expectedXlfData))
      ->willReturn(TRUE);

    // Set up an observer middleware instance.
    $observerMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setConstructorArgs(array($mockClient, $mockWrapper, $observerDrupal))
      ->setMethods(array('setProcessed'))
      ->getMock();
    $observerMiddleWare->expects($this->once())
      ->method('setProcessed')
      ->with($this->equalTo($expectedLang))
      ->willReturn($expectedResponse);

    // Instantiate a MiddleWare instance and call setXliff().
    $this->assertSame($expectedResponse, $observerMiddleWare->setXliff($expectedXlfData, $expectedLang));
  }

  /**
   * Tests that MiddleWare::getProcessedXliff will return FALSE and set an error
   * message when no source directory is configured.
   *
   * @test
   */
  public function getProcessedXliffSourceDoesNotExist() {
    $translatedMessage = 'Translated no target directory message.';
    $expectedResponse = FALSE;
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the SFTP client.
    $observerClient = $this->getConnectedClientMock();

    // The client's put method should never be called.
    $observerClient->expects($this->never())
      ->method('get');

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array(
      'variableGet',
      'setMessage',
      't',
    ));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with($this->equalTo(MiddleWare::SOURCEROOTVAR), $this->equalTo(FALSE))
      ->willReturn($expectedResponse);

    $observerDrupal->expects($this->once())
      ->method('t')
      ->with($this->equalTo('No source directory is configured.'))
      ->willReturn($translatedMessage);

    // The DrupalHandler observer expects the setMessage method to be called.
    $observerDrupal->expects($this->once())
      ->method('setMessage')
      ->with($this->equalTo($translatedMessage), $this->equalTo('error'));

    // Instantiate MiddleWare and invoke MiddleWare::putXliff().
    $middleware = new MiddleWare($observerClient, $mockWrapper, $observerDrupal);
    $this->assertSame($expectedResponse, $middleware->getProcessedXliff(NULL));
  }

  /**
   * Tests that MiddleWare::getProcessedXliff will Net_SFTP::get the XLIFF file
   * from the expected location on the FTP server.
   *
   * @test
   */
  public function getProcessedXliffSourceExists() {
    $expectedSource = '/path/to/source';
    $expectedLang = 'ja';
    $expectedLangPath = 'ja-JP';
    $expectedFilename = 'file.xlf';
    $expectedFullPath = $expectedSource . '/' . $expectedLangPath . '/' . $expectedFilename;
    $expectedResponse = TRUE;

    $observerClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('variableGet'));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with($this->equalTo(MiddleWare::SOURCEROOTVAR), $this->equalTo(FALSE))
      ->willReturn($expectedSource);

    // The client observer expects the put method to be called.
    $observerClient->expects($this->once())
      ->method('get')
      ->with($this->equalTo($expectedFullPath))
      ->willReturn($expectedResponse);

    $observerMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setConstructorArgs(array($observerClient, $mockWrapper, $observerDrupal))
      ->setMethods(array('getLanguagePathPartSource', 'getFilename'))
      ->getMock();

    $observerMiddleWare->expects($this->once())
      ->method('getLanguagePathPartSource')
      ->with($this->equalTo($expectedLang))
      ->willReturn($expectedLangPath);

    $observerMiddleWare->expects($this->any())
      ->method('getFilename')
      ->willReturn($expectedFilename);

    // Instantiate MiddleWare and invoke MiddleWare::putXliff().
    $this->assertSame($expectedResponse, $observerMiddleWare->getProcessedXliff($expectedLang));
  }

  /**
   * Tests that MiddleWare::setProcessed method moves the translated XLIFF file
   * for a given language to the /processed sub-folder in the source folder and
   * updates the access time via Net_SFTP::touch.
   *
   * @test
   */
  public function setProcessed() {
    $expectedRoot = 'path/to/source';
    $expectedLanguage = 'fr';
    $expectedLanguagePath = 'fr-FR';
    $expectedFile = 'file.xlf';
    $expectedClientResponse = TRUE;
    $expectedFrom = $expectedRoot . '/' . $expectedLanguagePath . '/' . $expectedFile;
    $expectedTo = $expectedRoot . '/' . $expectedLanguagePath . '/processed/' . $expectedFile;

    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the Client.
    $observerClient = $this->getConnectedClientMock();

    // We expect that Net_SFTP::rename will be called exactly once.
    $observerClient->expects($this->once())
      ->method('rename')
      ->with($this->equalTo($expectedFrom), $this->equalTo($expectedTo))
      ->willReturn($expectedClientResponse);

    // We expect that Net_SFTP::touch will be called exactly once.
    $observerClient->expects($this->once())
      ->method('touch')
      ->with($this->equalTo($expectedFrom));

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('variableGet'));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with($this->equalTo(MiddleWare::SOURCEROOTVAR))
      ->willReturn($expectedRoot);

    // Create an observer double for our MiddleWare class.
    $observerMiddleWare = $this->getMockBuilder('EntityXliffFtp\MiddleWare')
      ->setConstructorArgs(array($observerClient, $mockWrapper, $observerDrupal))
      ->setMethods(array('getLanguagePathPartSource', 'getFilename'))
      ->getMock();

    // We expect that the getLanguagePathPartSource method will be called once.
    $observerMiddleWare->expects($this->once())
      ->method('getLanguagePathPartSource')
      ->with($expectedLanguage)
      ->willReturn($expectedLanguagePath);

    // We expect that getFilename will be called any number of times.
    $observerMiddleWare->expects($this->any())
      ->method('getFilename')
      ->willReturn($expectedFile);

    // Call MiddleWare::setProcessed() and assert the expected response.
    $this->assertEquals($expectedClientResponse, $observerMiddleWare->setProcessed($expectedLanguage));
  }

  /**
   * Tests that MiddleWare::getXliff calls out to the Entity XLIFF API in the
   * expected way and returns that data directly.
   *
   * @test
   */
  public function getXliff() {
    $expectedLang = 'fr-fr';
    $expectedXliff = '<xml></xml>';
    $mockClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock(array('raw', 'getIdentifier', 'getPropertyInfo', 'type'));

    // Set up an observer on the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler');

    // We expect the entityXliffGetTranslatable method to be called.
    $observerDrupal->expects($this->once())
      ->method('entityXliffGetXliff')
      ->with($this->equalTo($mockWrapper), $this->equalTo($expectedLang))
      ->willReturn($expectedXliff);

    $middleware = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $this->assertEquals($expectedXliff, $middleware->getXliff($expectedLang));
  }

  /**
   * Tests that MiddleWare::getFilename returns a filename, based on the
   * encapsulated Entity wrapper, in the expected format of:
   * - [Site Prefix]-[Entity Type]-[Entity ID].xlf
   *
   * @test
   */
  public function getFilename() {
    $expectedPrefix = 'drupal-123';
    $expectedType = 'entity_type';
    $expectedId = 1234;
    $expectedFilename = implode('-', array($expectedPrefix, $expectedType, $expectedId)) . '.xlf';

    $mockClient = $this->getConnectedClientMock();
    $observerWrapper = $this->getWrapperMock(array('type', 'getIdentifier'));
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('variableGet'));

    // This method should call the entity wrapper's type method once.
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);

    // This method should call the entity wrapper's getIdentifier method once.
    $observerWrapper->expects($this->once())
      ->method('getIdentifier')
      ->willReturn($expectedId);

    // This method should call DrupalHandler::variableGet exactly once.
    $observerDrupal->expects($this->once())
      ->method('variableGet')
      ->with($this->equalTo(MiddleWare::FILEPREFIXVAR))
      ->willReturn($expectedPrefix);

    // Instantiate MiddleWare and run MiddleWare::getFilename().
    $middleware = new MiddleWare($mockClient, $observerWrapper, $observerDrupal);
    $this->assertEquals($expectedFilename, $middleware->getFilename());
  }

  /**
   * Tests that MiddleWare::getLanguagePathPartTarget returns the the target
   * language path part as expected.
   *
   * @test
   * @dataProvider languagePrefixPathPartProvider
   */
  public function getLanguagePathPartTarget($language, $response, $expectedPathPart) {
    $mockClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('languageList'));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn($response);

    // Instantiate MiddleWare and call getLanguagePathPartSource().
    $middleware = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $this->assertEquals($expectedPathPart['target'], $middleware->getLanguagePathPartTarget($language));
  }

  /**
   * Tests that MiddleWare::getLanguagePathPartSource returns the source
   * language path part as expected.
   *
   * @test
   * @dataProvider languagePrefixPathPartProvider
   */
  public function getLanguagePathPartSource($language, $response, $expectedPathPart) {
    $mockClient = $this->getConnectedClientMock();
    $mockWrapper = $this->getWrapperMock();

    // Create an observer double for the DrupalHandler.
    $observerDrupal = $this->getMock('EntityXliffFtp\Utils\DrupalHandler', array('languageList'));

    // The DrupalHandler observer expects the variableGet method to be called.
    $observerDrupal->expects($this->once())
      ->method('languageList')
      ->with($this->equalTo('language'))
      ->willReturn($response);

    // Instantiate MiddleWare and call getLanguagePathPartSource().
    $middleware = new MiddleWare($mockClient, $mockWrapper, $observerDrupal);
    $this->assertEquals($expectedPathPart['source'], $middleware->getLanguagePathPartSource($language));
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
   * Returns a simple mock EntityDrupalWrapper.
   *
   * @param array $setMethods
   *   (optional) If provided, an array of method names that will be set by
   *   the caller.
   *
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getWrapperMock(array $setMethods = NULL) {
    return $this->getMockBuilder('EntityDrupalWrapper')
      ->disableOriginalConstructor()
      ->setMethods($setMethods)
      ->getMock();
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

  /**
   * Data provider for MiddleWareTest::getLanguagePathPartSource.
   *
   * @return array
   *   An array consisting of:
   *   - 0: Target language,
   *   - 1: A mock language object,
   *   - 2: An array consisting of:
   *     - source: The expected source path for this language.
   *     - target: The expected target path for this language.
   */
  public function languagePrefixPathPartProvider() {
    return array(
      array('fr', array('fr' => (object) array('language' => 'fr')), array('source' => 'fr', 'target' => 'en_to_fr')),
      array('de', array('de' => (object) array('language' => 'de')), array('source' => 'de', 'target' => 'en_to_de'))
    );
  }

}
