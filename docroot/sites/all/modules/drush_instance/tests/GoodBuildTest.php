<?php

/**
 * @file GoodBuildTest.php
 *
 * Extends GoodbuildAbstract class for makefile builds.
 */

// Ideally autoload would pick up on this.
if (!class_exists("GoodBuildAbstract")) {
  require_once(__DIR__ . "/GoodBuildAbstract.php");
}

/**
 * @class GoodBuildTest
 */
class GoodBuildTest extends GoodBuildAbstract {
}
