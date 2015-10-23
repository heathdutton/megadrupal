#!/usr/bin/php
<?php

require 'vendor/autoload.php';

$dir = new \fizk\DirectoryHash\Standard();
echo $dir->hash() . "\n";
