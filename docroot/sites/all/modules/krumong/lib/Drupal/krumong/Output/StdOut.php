<?php

namespace Drupal\krumong\Output;


/**
 * Ouch, that's a bit too much abstraction.
 * Let's see how the rest shapes up, then we change this.
 */
class StdOut {

  protected $cssJs;

  function __construct($cssJs) {
    $this->cssJs = $cssJs;
  }

  function html($html) {
    $this->cssJs->printCssJsOnce();
    print $html;
  }
}
