<?php
FactoryDrone::define('menu', 'menu', array(
  'menu_name' => FactoryDrone::sequence(function ($n) { return "menu_name${n}"; }),
  'title' => FactoryDrone::sequence(function ($n) { return "title${n}"; }),
  'description' => FactoryDrone::sequence(function ($n) { return "description${n}"; }),
));
