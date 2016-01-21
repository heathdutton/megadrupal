<?php

if(!defined('MTTPATH')) define('MTTPATH', dirname(__FILE__) .'/');

require_once(MTTPATH. 'common.php');

Config::loadConfig();
require_once(MTTPATH. 'lang/class.default.php');
require_once(MTTPATH. 'lang/'.Config::get('lang').'.php');

header('Content-type: text/javascript; charset=utf-8');
echo "mytinytodo.lang.init(". Lang::instance()->makeJS() .");";

?>
