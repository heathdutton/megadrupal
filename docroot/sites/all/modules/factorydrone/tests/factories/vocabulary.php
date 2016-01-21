<?php
FactoryDrone::define('vocabulary', 'vocab', array(
  'name' => FactoryDrone::sequence(function ($n) { return "vocabulary_name{$n}"; }),
  'machine_name' => FactoryDrone::sequence(function ($n) { return "vocabulary_machine_name{$n}"; }),
  'heirarchy' => 0,
));
