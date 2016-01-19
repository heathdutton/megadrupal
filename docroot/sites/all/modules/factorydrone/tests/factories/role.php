<?php
FactoryDrone::define('role', 'role', array(
  'name' => FactoryDrone::sequence(function ($n) { return "role${n}"; }),
));
