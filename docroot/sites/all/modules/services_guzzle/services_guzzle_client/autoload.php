<?php

$loader = new \Composer\Autoload\ClassLoader();

// register classes with namespaces
$loader->add('DrupalServices', __DIR__);

// activate the autoloader
$loader->register();