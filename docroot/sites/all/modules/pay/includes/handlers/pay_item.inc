<?php

/**
 * @file
 * An item type for payment activities.
 */

class pay_item extends pay {

  var $piid;
  var $title;
  var $description;
  var $status;
  var $cost;

  var $table = 'pay_item';
  var $key = 'piid';

  function set_title($val = NULL) {
    $this->title = check_plain($val);
  }

  function title() {
    return check_plain($this->title);
  }

  function set_description($val = NULL) {
    $this->description = filter_xss($val);
  }

  function description() {
    return filter_xss($this->description);
  }

  function set_cost($val = 0) {
    $this->cost = (float) $val;
  }

  function cost() {
    return (float) $this->cost;
  }
}
