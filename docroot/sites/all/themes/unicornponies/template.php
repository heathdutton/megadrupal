<?php

// $Id$

function unicornponies_preprocess_html(&$variables) {
  drupal_add_js('http://www.cornify.com/js/cornify.js', 'external');
}