<?php

/**
 * @file
 * Drujax html.tpl.php version 
 * 
 * This file should only output a json object
 */
$json = array(
  "title" => $head_title,
  "drujax_vars" => $drujax_vars,
  "content" => array(
    "$drujax_wrapper" => $page,
  ),
);

print json_encode($json);
?>
