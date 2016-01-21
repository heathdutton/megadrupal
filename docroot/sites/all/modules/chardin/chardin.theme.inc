<?php

function chardin_preprocess_chardin_link(&$vars) {
  if(empty($vars['options'])) $vars['options'] = array();

  if(!isset($vars['options']['attributes']['class'])) $vars['options']['attributes']['class'] = array();
  $vars['options']['attributes']['class'][] = 'chardin-link';

  if(!isset($vars['options']['attributes']['data-toggle'])) $vars['options']['attributes']['data-toggle'] = array();
	$vars['options']['attributes']['data-toggle'] = 'chardinjs';

  $vars['options']['external'] = TRUE;

  if(empty($vars['link_title'])) $vars['link_title'] = t('Start the tour!');
}