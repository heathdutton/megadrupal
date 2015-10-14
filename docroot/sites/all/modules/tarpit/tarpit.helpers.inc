<?php

function _tarpit_generate_render_array($words = NULL, $links = NULL, $file = NULL) {
  $config = array(
    'size' => $words ? $words : \Drupal\tarpit\Config::get('tarpit.size'),
    'links' => $links ? $links : \Drupal\tarpit\Config::get('tarpit.links'),
    'wordlist'  => $file ? $file : \Drupal\tarpit\Config::get('tarpit.wordlist'),
  );

  $markup = array(
    'default' => array(
      '#markup' => ''
    )
  );

  drupal_alter('tarpit_content', $markup, $config);

  return $markup;
}

function _tarpit_generate_content($path, $words, $links, $file) {
  $f_contents = file($file);

  for($i=0; $i < $links; $i++) {
    $random = trim($f_contents[array_rand($f_contents)]);
    $text[] = l($random, $path . '/' . $random);
  }

  for($i=0; $i < $words; $i++) {
    $text[] = trim($f_contents[array_rand($f_contents)]);
  }

  shuffle($text);
  return implode($text, ' ');
}

function _tarpit_sleep($sleep_min = 0, $sleep_max = 0) {
  if (!is_numeric($sleep_min) || !is_numeric($sleep_max)) {
    return;
  }

  if ($sleep_min < 0 || $sleep_max < 0) {
    return;
  }

  if ($sleep_min == 0 && $sleep_max == 0) {
    return;
  }

  if ($sleep_min >= $sleep_max) {
    return;
  }

  sleep(mt_rand($sleep_min, $sleep_max));
}
