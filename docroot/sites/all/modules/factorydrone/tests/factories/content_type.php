<?php
FactoryDrone::define('content type', 'content type', array(
  'type' => FactoryDrone::sequence(function ($n) { return "page${n}"; }),
  'name' => FactoryDrone::sequence(function ($n) { return "Page Type ${n}"; }),
  'base' => 'node_content',
  'description' => 'A simple page',
));
