<?php


// jQuery Masonry
$libraries['masonry'] = array(

  'name' => 'jQuery Masonry',
  'vendor url' => 'https://github.com/desandro/masonry',
  'download url' => 'https://github.com/desandro/masonry/zipball/master',
  'version arguments' => array(
    'file' => 'dist/masonry.pkgd.js',
    'pattern' => '/PACKAGED v([0-9a-zA-Z\.-]+)/',
  ),
  'files' => array(
    'js' => array(
      'dist/masonry.pkgd.js',
    ),
  )
);
