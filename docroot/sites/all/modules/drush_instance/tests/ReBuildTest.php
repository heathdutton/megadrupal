<?php

/**
 * @file ReBuildTest.php
 *
 * Extends ReBuildAbstract class to test a makefile-based build.
 */

// Ideally autoload would pick up on this.
if (!class_exists("ReBuildAbstract")) {
  require_once(__DIR__ . "/ReBuildAbstract.php");
}

/**
 * @class ReBuildTest
 */
class ReBuildTest extends ReBuildAbstract {
}
