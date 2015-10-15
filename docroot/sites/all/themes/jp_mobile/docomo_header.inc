<?php

/**
 * @file
 * Specifies header and head element for DoCoMo
 */

header('Content-Type: application/xhtml+xml');
$mobile_head = '<?xml version="1.0" encoding="shift_jis"?>
<!DOCTYPE html PUBLIC "-//i-mode group (ja)//DTD XHTML i-XHTML(Locale/Ver.=ja/1.0) 1.0//EN" "i-xhtml_4ja_10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja">
  <head>
    <title>'. $head_title .'</title>
  </head>';
