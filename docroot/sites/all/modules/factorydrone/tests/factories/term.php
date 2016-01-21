<?php
FactoryDrone::define('term', 'term', array(
  'name' => FactoryDrone::sequence(function ($n) { return "term_name{$n}"; }),
));
