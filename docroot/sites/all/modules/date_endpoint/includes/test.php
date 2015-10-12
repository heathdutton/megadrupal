<?php

include_once('DateEndpointGenerator.inc');

date_default_timezone_set('America/Chicago');

$endpoints = new DateEndpointGenerator;
$options = $endpoints->getOptions();
$objects = $endpoints->getObjects();

print_r($objects['Y0']);
