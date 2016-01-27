<?php
FactoryDrone::define('', 'factory', array(
  'foo' => 'bar',
  'sequence' => FactoryDrone::sequence(function ($n) { return "sequence {$n}"; }),
));
