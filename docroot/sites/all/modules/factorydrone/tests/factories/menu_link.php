<?php
FactoryDrone::define('menu link', 'menu link', array(
  'link_path' => '/',
  'link_title' => FactoryDrone::sequence(function ($n) { return "link_title${n}"; }),
));

FactoryDrone::define('menu link', 'menu link w menu', array(
  'link_path' => FactoryDrone::random(2, function($n) { return $n; }),
  'link_title' => FactoryDrone::sequence(function ($n) { return "link_title${n}"; }),
  'menu_name' => FactoryDrone::association('menu', array('menu_name' => 'new menu')),
));

FactoryDrone::define('menu link', 'menu link w parent item', array(
  'link_path' => FactoryDrone::random(2, function($n) { return $n; }),
  'link_title' => FactoryDrone::sequence(function ($n) { return "link_title${n}"; }),
  'plid' => FactoryDrone::association('menu link', array('link_title' => 'new parent menu item')),
));
