<?php
/*
	This file is part of myTinyTodo.
	(C) Copyright 2009-2010 Max Pozdeev <maxpozdeev@gmail.com>
	Licensed under the GNU GPL v2+ license. See file COPYRIGHT for details.
*/

if(!defined('MTTPATH')) define('MTTPATH', dirname(__FILE__) .'/');

require_once(MTTPATH. 'common.php');
require_once(MTTPATH. 'class.db.drupal.php');

Config::loadConfig();

$db = DBConnection::init(new Database_Drupal);
$db->dq("SET NAMES utf8");

require_once(MTTPATH. 'lang/class.default.php');
require_once(MTTPATH. 'lang/'.Config::get('lang').'.php');

function is_readonly()
{
	return false;
}

function timestampToDatetime($timestamp)
{
	$format = Config::get('dateformat') .' '. (Config::get('clock') == 12 ? 'g:i A' : 'H:i');
	return formatTime($format, $timestamp);
}

function formatTime($format, $timestamp=0)
{
	$lang = Lang::instance();
	if($timestamp == 0) $timestamp = time();
	$newformat = strtr($format, array('F'=>'%1', 'M'=>'%2'));
	$adate = explode(',', date('n,'.$newformat, $timestamp), 2);
	$s = $adate[1];
	if($newformat != $format)
	{
		$am = (int)$adate[0];
		$ml = $lang->get('months_long');
		$ms = $lang->get('months_short');
		$F = $ml[$am-1];
		$M = $ms[$am-1];
		$s = strtr($s, array('%1'=>$F, '%2'=>$M));
	}
	return $s;
}

function _e($s)
{
	echo Lang::instance()->get($s);
}

function __($s)
{
	return Lang::instance()->get($s);
}

function mttinfo($v)
{
	global $_mttinfo;
	if(!isset($_mttinfo[$v])) {
		echo get_mttinfo($v);
	} else {
		echo $_mttinfo[$v];
	}
}

function get_mttinfo($v)
{
	global $_mttinfo;
	if(isset($_mttinfo[$v])) return $_mttinfo[$v];
	switch($v)
	{
		case 'template_url':
			$_mttinfo['template_url'] = get_mttinfo('mtt_url'). 'themes/'. Config::get('template') . '/';
			return $_mttinfo['template_url'];
		case 'url':
			$_mttinfo['url'] = Config::get('url');
			if($_mttinfo['url'] == '')
				$_mttinfo['url'] = 'http://'.$_SERVER['HTTP_HOST'] .($_SERVER['SERVER_PORT'] != 80 ? ':'.$_SERVER['SERVER_PORT'] : '').
									url_dir(isset($_SERVER['REQUEST_URI']) ? $_SERVER['REQUEST_URI'] : $_SERVER['SCRIPT_NAME']);
			return $_mttinfo['url'];
		case 'mtt_url':
			$_mttinfo['mtt_url'] = Config::get('mtt_url');
			if($_mttinfo['mtt_url'] == '') $_mttinfo['mtt_url'] = url_dir(isset($_SERVER['REQUEST_URI']) ? $_SERVER['REQUEST_URI'] : $_SERVER['SCRIPT_NAME']);
			return $_mttinfo['mtt_url'];
		case 'title':
			$_mttinfo['title'] = (Config::get('title') != '') ? htmlarray(Config::get('title')) : __('My Tiny Todolist');
			return $_mttinfo['title'];
	}
}

function jsonExit($data)
{
	header('Content-type: application/json; charset=utf-8');
	echo json_encode($data);
	exit;
}

?>
