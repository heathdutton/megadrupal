<?php
/**
 * @file
 * Inline devel test class.
 */
abstract class inlineDvelTesting extends DrupalWebTestCase {

  function setUp() {
    parent::setUp($this->modules);

    $this->user = $this->drupalCreateUser(array(
      'access devel information',
      'execute php code'
    ));

    $function = "
      function hook_test_message_invoke() {
        return 'Message Teseting';
      }
    ";

    $functions = inline_devel_php_parsing($function);

    // Change the hook prefix name.
    inline_devel_change_hook_prefix($functions[1]);

    $_SESSION['function'][$functions[1]] = array(
      'function' => $functions[1],
      'arguments' => $functions[2],
      'content' => $functions[3],
    );
  }

  function tearDown() {
    parent::tearDown();
    unset($_SESSION['function']);
  }
}
