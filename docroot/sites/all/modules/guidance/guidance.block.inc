<?php


/**
 * Define a block.
 * @param int $nid
 * @return array
 */
function guidance_create_block($nid) {
	if (!guidance_is_run($nid)) return;
	
  $guide = guidance_get_guide($nid);
  $links = guidance_get_guide_links($nid);
  if (empty($links)) return;
  
  cache_clear_all(NULL, 'cache_page');

  $content = '';

  $rows = array();
  foreach ((array)$links as $link) {
    $rows[] = array(l(
      _guidance_title_format($link->title, $link->url),
      ($link->lid) ? 'guide/'.$nid.'/'.$link->lid : $link->url,
      array('attributes' => array('class' => 'guide'.($link->visited ? ' visited' : '')))
    ));
  }
  $content = guidance_block_get_guide_body($guide);
  $content .= theme('table', array('rows' => $rows));
  $form = drupal_get_form('guidance_block_closer_form_' . $nid, $nid);
  $content .= drupal_render($form);

  $block['subject'] = _guidance_title_format($guide->title, t('Guide'));
  $block['content'] = $content;

	drupal_alter('guidance_block', $block, $nid);
  
  return $block;
}


/**
 * Define a guide runner block
 */
function guidance_create_runner_block($nid) {
	if (guidance_is_run($nid)) return;
	
	cache_clear_all(NULL, 'cache_page');
	
	$guide = guidance_get_guide($nid);
  $content = guidance_block_get_guide_body($guide);
	$form = drupal_get_form('guidance_block_runner_form_'.$nid, $nid);
  $content .= drupal_render($form);
	$block['subject'] = _guidance_title_format($guide->title, t('Guide'));
	$block['content'] = $content;
	
	drupal_alter('guidance_block', $block, $nid);
	
	return $block;
}


/**
 * Form for the START button on the runner form.
 */
function guidance_block_runner_form($form, $form_status, $nid) {
	return array(
		'nid' => array(
			'#type' => 'hidden',
			'#value' => $nid,
		),
		'submit' => array(
			'#type' => 'submit',
			'#value' => 'Start guide',
		),
    '#submit' => array('guidance_block_runner_form_submit'),
	);
}


/**
 * Submission for the start button on the runner form.
 */
function guidance_block_runner_form_submit($form, $form_state) {
	guidance_save_state($form_state['values']['nid'], GUIDANCE_GUIDE_ON);
	cache_clear_all(NULL, 'cache_page');
}


/**
 * Form for closing a guide.
 */
function guidance_block_closer_form($form, $form_status, $nid) {
	return array(
		'nid' => array(
			'#type' => 'hidden',
			'#value' => $nid,
		),
		'submit' => array(
			'#type' => 'submit',
			'#value' => 'Close guide',
		),
    '#submit' => array('guidance_block_closer_form_submit'),
	);
}


/**
 * Submission for the closer form.
 */
function guidance_block_closer_form_submit($form, $form_state) {
	guidance_save_state($form_state['values']['nid'], GUIDANCE_GUIDE_OFF);
	cache_clear_all(NULL, 'cache_page');
}


/**
 * Renders the guide-link-help block.
 *
 * @param object $node
 * @return string
 */
function guidance_block_get_guide_body($node) {
  cache_clear_all(NULL, 'cache_page');
  $output = '';

  $body = $node->body['und'][0]['safe_value'];
  $summary = $node->body['und'][0]['summary'];
  if (strlen(strstr($body, '<!--break-->')) > 14) {
  	$output .= !empty($summary) ? $summary . ' ' . l(t('More information...'), 'node/' . $node->nid) : '';
  } else {
    $output .= $body;
  }
  return '<p>' . $output . '</p>';
}
