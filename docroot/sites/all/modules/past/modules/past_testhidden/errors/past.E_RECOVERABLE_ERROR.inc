<?php
// Define function that expect an argument of type array.
function my_func(array $arg) {
  var_dump($arg);
}

// Call the function with string argument.
my_func(NULL);
