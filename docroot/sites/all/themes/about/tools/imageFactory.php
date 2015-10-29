<?php

$img = $_GET['i'];

if (! preg_match("/^[^\.\/]/", $img)) {
  header("Content-type: image/png");
  readfile("images/404.png");
  exit(0); 
}


require_once('simpleImage.php');

$image = new SimpleImage();
$image->load($img);

switch ($image->image_type) {
  case IMAGETYPE_GIF :
    header("Content-type: image/gif");
    break;
  case IMAGETYPE_PNG :
    header("Content-type: image/png");
    break;
  case IMAGETYPE_JPEG : 
  default : 
    header("Content-type: image/jpeg");
    break;
}

$image->resizeToWidth($_GET['w']);
$image->output();

exit(0);
