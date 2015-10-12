<?php
FactoryDrone::define('user', 'user', array(
  'name' => FactoryDrone::sequence(function ($n) { return "name${n}"; }),
  'mail' => FactoryDrone::sequence(function ($n) { return "foo${n}@example.com"; }),
));

FactoryDrone::define('user', 'user w role', array(
  'name' => FactoryDrone::random(5, function ($n) { return $n; }),
  'mail' => FactoryDrone::sequence(function ($n) { return "foo{$n}@example.com"; }),
  'roles' => FactoryDrone::association('role', array('name' => 'new_user_role')),
));
