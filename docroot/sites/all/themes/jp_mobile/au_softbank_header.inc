<?php

/**
 * @file
 * Specifies header and head element for au and Softbank
 */

header('Content-type: text/html; charset=shift_jis');
$mobile_head = '<?xml version="1.0" encoding="shift_jis"?>
<!DOCTYPE html PUBLIC "-//OPENWAVE//DTD XHTML 1.0//EN" "http://www.openwave.com/DTD/xhtml-basic.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja">
  <head>
    <title>'. $head_title .'</title>
    <meta http-equiv="Pragma" content="no-cache" />
    <meta http-equiv="Cache-Control" content="no-cache" />
    <meta http-equiv="Expires" content="-1" />
  </head>';
