#!/usr/bin/php
<?php

// Security restriction.
if (isset($_SERVER['REQUEST_METHOD'])) {
  return;
}

function curl_get($url) {
  $ch = curl_init($url);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array('User-Agent: smalot'));
  $content = curl_exec($ch);
  curl_close($ch);

  return $content;
}

// Identify current release.
$github = 'https://api.github.com/repos/smalot/magento-client/releases';
$content = curl_get($github);

// Extract tarball url.
$history = json_decode($content, TRUE);
$tarfile = $history[0]['tarball_url'];

// Download tarball.
file_put_contents('magento-client.tgz', curl_get($tarfile));

if (is_dir('lib')) {
  exec('rm -Rf lib');
}

mkdir('lib');
exec('tar zxvf magento-client.tgz --strip 1 --directory=lib');
unlink('magento-client.tgz');
unlink('lib/composer.json');
unlink('lib/composer.lock');

echo '-----------------------------------------------------' . "\n";
echo 'Library "smalot/magento-client" updated to ' . $history[0]['tag_name'] . "\n";
echo '-----------------------------------------------------' . "\n";
echo $history[0]['body'] . "\n";
echo '-----------------------------------------------------' . "\n";
