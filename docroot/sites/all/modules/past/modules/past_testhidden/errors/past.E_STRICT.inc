<?php
// Define a class with method that accepts
// one argument.
class ParentClass {
  function myMethod($args) {

  }
}

// Extend the parent class and override
// myMethod() in a way that changes the
// method signature.
class ChildClass extends ParentClass {
  function myMethod() {

  }
}
