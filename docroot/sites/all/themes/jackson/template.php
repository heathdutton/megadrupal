<?php

/**
 * Set count variables so column numbers can be dynamic.
 */
function jackson_preprocess_page (&$vars) {
  $vars['preface'] = count(array_filter(array($vars['page']['preface_one'], $vars['page']['preface_two'], $vars['page']['preface_three'])));
  $vars['bottom'] = count(array_filter(array($vars['page']['bottom_one'], $vars['page']['bottom_two'], $vars['page']['bottom_three'], $vars['page']['bottom_four'])));
  // Display user account links.
}

