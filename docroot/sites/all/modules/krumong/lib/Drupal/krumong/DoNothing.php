<?php

namespace Drupal\krumong;


class DoNothing {

  function __call($method, $args) {
    return;
  }
}
