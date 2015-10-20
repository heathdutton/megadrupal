<?php
/*
 * 
 * @file hotel_booking_handler_field_from_price.inc
 * Provides from price field for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 * 
 */


class hotel_booking_handler_field_from_price extends views_handler_field_numeric {

  function render($values) {
    $price = $this->get_value($values);
    if ($price == 0) {
      return 0;
    }
    return uc_currency_format($price);
  }

}

