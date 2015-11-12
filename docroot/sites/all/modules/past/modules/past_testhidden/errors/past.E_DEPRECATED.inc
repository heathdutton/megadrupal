<?php

// Calling a PHP deprecated method. See
// http://php.net/manual/en/migration53.deprecated.php
// For “current” list of deprecated PHP functions.
class TestDeprecated {
  function myMethod() {

  }
}
$obj = new TestDeprecated();
call_user_method('myMethod', $obj);
