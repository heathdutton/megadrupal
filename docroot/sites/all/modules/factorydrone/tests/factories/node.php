<?php
FactoryDrone::define('node', 'node', array(
  'type' => FactoryDrone::create('content type')->type,
  'title' => FactoryDrone::sequence(function ($n) { return "node_title{$n}"; }),
  'status' => 1,
));

FactoryDrone::define('node', 'node w content type', array(
  'type' => FactoryDrone::association('content type'),
  'title' => FactoryDrone::sequence(function ($n) { return "node_title{$n}"; }),
  'status' => 1,
));
