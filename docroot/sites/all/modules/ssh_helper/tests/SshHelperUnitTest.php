<?php

class SshHelperUnitTest extends PHPUnit_Framework_TestCase {

  function testSshHelperLoginSsh() {
    // Get the Net_SSH2 class and mock it so we can use the login class
    $ssh = $this->getMock('Net_SSH2');
    // Expect the login method and let it return true
    $ssh->expects($this->once())
      ->method("login")
      ->will($this->returnValue(true));

    // check if the ssh_helper_login_ssh works as expected
    $ssh_return = ssh_helper_login_ssh('somehostname', 'user', 'key', $ssh);
    $this->assertNotEmpty($ssh_return, 'Login succeeded');

    // Create a new mock object where the login method does not return anything. Eg.: it'll fail
    $ssh = $this->getMock('Net_SSH2');
    $ssh->expects($this->once())
      ->method("login");

    // Expect the login method and let it return true
    $exception_thrown = false;
    try {
      // check if the ssh_helper_login_ssh works as expected
      ssh_helper_login_ssh('somehostname', 'user', 'key', $ssh);
    } catch (Exception $e) {
      $exception_thrown = true;
    }
    $this->assertTrue($exception_thrown, 'Login was unsuccessful and exception was thrown');
  }

  /**
   * Tests the getter and setter of the KeyPath property
   */
  function testSshHelperKeyPath() {
    $ssh_helper = new SshHelper();
    $keyPath = $ssh_helper->getKeyPath();
    $this->assertNotEmpty($keyPath, 'KeyPath was not empty ');

    $ssh_helper->setKeyPath('test');
    $keyPath = $ssh_helper->getKeyPath();
    $this->assertEquals('test', $keyPath, 'KeyPath is equal to test');
  }

  /**
   * Tests the getter and setter of the Mail property
   */
  function testSshHelperMail() {
    $ssh_helper = new SshHelper();
    $mail = $ssh_helper->getMail();
    $this->assertNotEmpty($mail, 'Mail was not empty ');

    $ssh_helper->setMail('test');
    $mail = $ssh_helper->getMail();
    $this->assertEquals('test', $mail, 'Mail is equal to test');
  }

  /**
   * Tests the getter and setter of the hostname property
   */
  function testSshHelperHostname() {
    // Get the Net_SSH2 class and mock it so we can use the login class
    $ssh = $this->getMock('Net_SSH2');

    // Set a hostname to test
    $hostname1 = 'www.example.com';

    // Set our fake ssh object instantly
    $ssh_helper = new SshHelper($hostname1, $ssh);

    $hostname = $ssh_helper->getHostname();
    $this->assertEquals($hostname1, $hostname, 'Hostname was equal to example1');

    $hostname2 = 'www.example2.com';
    $ssh_helper->setHostname($hostname2);
    $hostname = $ssh_helper->getHostname();
    $this->assertEquals($hostname2, $hostname, 'Hostname is equal to example2');
  }

  /**
   * Tests the getter and setter of the key property
   */
  function testSshHelperKey() {
    // Get the Net_SSH2 class and mock it so we can use the login class
    $ssh = $this->getMock('Net_SSH2');

    $ssh_helper = new SshHelper();
    $ssh_helper->setSsh($ssh);

    // See if we can retrieve our ssh object
    $sshGet = $ssh_helper->getSsh();

    // Check if it is equal
    $this->assertEquals($sshGet, $ssh, 'SSH Mock object that was set is equal to the one we retrieve');

    // Generate our key
    $ssh_helper->generateKeypair();

    $key = $ssh_helper->loadKey();
    $this->assertNotEmpty($key, 'Key is not empty');

    $ssh_helper->generateKeypair();
    variable_set('ssh_helper_private_key_pass', 'test');
    $key = $ssh_helper->loadKey();
    $this->assertNotEmpty($key, 'Key is not empty with a password');
    $getKey = $ssh_helper->getKey();

    $this->assertEquals($getKey, $key, 'Key from both methods are equal');
    // Unset the variable
    variable_set('ssh_helper_private_key_pass', '');
  }

  /**
   * Tests the getter and setter of the Mail property
   */
  function testSshHelperUser() {
    variable_set('ssh_helper_username', 'NotEmpty');
    $ssh_helper = new SshHelper();
    $user = $ssh_helper->getUser();
    $this->assertNotEmpty($user, 'User was not empty ');

    $ssh_helper->setUser('test');
    $user = $ssh_helper->getUser();
    $this->assertEquals('test', $user, 'User is equal to test');
  }

}