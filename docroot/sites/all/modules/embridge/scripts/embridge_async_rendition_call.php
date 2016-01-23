<?php
/**
 * @file
 * EMBridge Aysnc Rendition Call.
 */

$ch = curl_init();

curl_setopt($ch, CURLOPT_URL, $argv[1]);
curl_setopt($ch, CURLOPT_HEADER, 0);
curl_setopt($ch, CURLOPT_TIMEOUT, 0);
curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
curl_exec($ch);
curl_close($ch);
