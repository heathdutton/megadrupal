<?php
/**
 * @file
 * PHPUnit tests for DHL Quote.
 */

class DHLQuoteTest extends PHPUnit_Framework_TestCase {
  function __construct() {
    require_once dirname(__FILE__) . '/../lib/DHLQuote.php';
  }

  public function testQuote() {
    $dhl = DHLQuote::Factory();
    $this->assertEquals($dhl->calculate(1.3, 'VN'), 193.38*1.2);
    $this->assertEquals($dhl->calculate(2.7, 'PT'), 147.73*1.2*1.2);
    $this->assertEquals($dhl->calculate(5, 'FR'), 152.36*1.2*1.2);

    // Test with package sizes.
    $dhl->setAllowedPackageSizes(array(1, 2, 3, 4, 7, 9, 10));
    // 7 kg.
    $price = 9.40*1.2*1.2;
    $this->assertEquals($dhl->calculate(4.5, 'FR'), $price);
    $this->assertEquals($dhl->calculate(5, 'FR'), $price);
    $this->assertEquals($dhl->calculate(7, 'FR'), $price);

    // Test chaining.
    $this->assertEquals($dhl->setAllowedPackagesizes(array(1, 7, 9))->calculate(2, 'FR'), $price);
  }
}

