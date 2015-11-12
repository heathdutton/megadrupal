<?php


/**
 * Lists all the links of a guide.
 * This page for adding new links also.
 *
 * @param int $nid
 * @return string
 */
function guidance_guide_page($nid) {
  $output = '';
  $guide = guidance_get_guide($nid);

  drupal_set_title(t('Guide: @title', array('@title' => _guidance_title_format($guide->title, '#'.$guide->nid))));

  $form = drupal_get_form('guidance_guide_links_form', $nid);
  $output .= drupal_render($form);

  $output .= l(t('Add new link'), 'node/'.$nid.'/guidance/add');

  guidance_set_breadcrumb($nid);

  return $output;
}


/**
 * Form for listing links
 */
function guidance_guide_links_form($form, &$form_state, $nid) {
  $links = guidance_get_guide_links($nid);
	$callback_array = module_invoke_all('guidance_callback');
	
  foreach((array)$links as $link) {
    $form['row'][$link->lid] = array(
      'title' => array(
        '#markup' => l(_guidance_title_format($link->title, $link->url), $link->url),
      ),
      'description' => array(
        '#markup' => check_plain($link->description),
      ),
			'callback' => array(
				'#markup' => $callback_array[$link->callback]['info'],
			),
      'edit' => array(
        '#markup' => l(t('Edit'), 'node/'.$nid.'/guidance/'.$link->lid.'/edit'),
      ),
      'delete' => array(
        '#markup' => l(t('Delete'), 'node/'.$nid.'/guidance/'.$link->lid.'/delete'),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => 0,
        '#delta' => 10,
        '#attributes' => array(
          'class' => array('links-weight'),
        ),
      ),
    );
  }

  $form['other']['nid'] = array(
    '#type' => 'hidden',
    '#value' => $nid,
  );

  $form['other']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
  );
  $form['#theme'] = 'guidance_guide_links_form';
  $form['#tree'] = TRUE;

  return $form;
}


/**
 * Submit for link form
 */
function guidance_guide_links_form_submit($form, $form_state) {
  foreach ((array)$form_state['values']['row'] as $lid => $link) {
    db_query('UPDATE {guidance_links} SET weight = :weight WHERE lid = :lid;', array(':weight' => $link['weight'], ':lid' => $lid));
  }
}


/**
 * Theme for link listing on a guide
 */
function theme_guidance_guide_links_form($variables) {
  $header = array(t('Link'), t('Description'), t('Callback'), t('Edit'), t('Delete'), NULL);
  $rows = array();

  $output = '';

  if (isset($variables['form']['row'])) {
    foreach ((array)$variables['form']['row'] as $lid => $row) {
      //if (!is_numeric($lid)) continue;
      if (substr($lid, 0, 1) == '#') continue;
      $rows[] = array(
        'data' => array(
          drupal_render($variables['form']['row'][$lid]['title']),
          drupal_render($variables['form']['row'][$lid]['description']),
          drupal_render($variables['form']['row'][$lid]['callback']),
          drupal_render($variables['form']['row'][$lid]['edit']),
          drupal_render($variables['form']['row'][$lid]['delete']),
          drupal_render($variables['form']['row'][$lid]['weight']),
        ),
        'class' => array('draggable'),
      );
    }
    drupal_add_tabledrag('links_table', 'order', 'sibling', 'links-weight');
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'links_table')));
  } else {
    $output .= t('No items yet.');
  }

  return $output . drupal_render_children($variables['form']);
}


/**
 * Editing/adding 1 link
 */
function guidance_link_form($form_original, $form_status_original, $nid, $lid = NULL) {
  if (empty($lid)) {
    drupal_set_title(t('Add new link'));
  } else {
    drupal_set_title(t('Edit link'));
    $link = guidance_get_guide_link($lid);
  }

	$callback_array = module_invoke_all('guidance_callback');
	$callbacks = array();
	$callback_help = '';
	foreach ((array)$callback_array as $callback_key => $callback_info) {
		$callbacks[$callback_key] = $callback_info['info'];
		$callback_help .= '<strong>'.$callback_info['info'].':</strong><br/>'.$callback_info['help'].'<br/><br/>';
	}

  $form = array(
    'title' => array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => isset($link->title) ? check_plain($link->title) : '',
      '#description' => t('Title of the guide.'),
    ),
    'url' => array(
      '#type' => 'textfield',
      '#title' => t('Url'),
      '#description' => t('The path this guide item links to. This can be an internal Drupal path such as node/add or an external URL such as http://drupal.org. Enter <front> to link to the front page.'),
      '#default_value' => isset($link->url) ? $link->url : '',
    ),
		'callback' => array(
			'#type' => 'select',
			'#title' => t('Callback function to verify if a link is done.'),
			'#options' => $callbacks,
			'#default_value' => isset($link->callback) ? check_plain($link->callback) : 'guidance_is_page_visited',
		),
		'callback_help' => array(
			'#type' => 'fieldset',
			'#title' => t('Help for callback functions'),
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
			'help_description' => array(
				'#prefix' => '<div>',
				'#suffix' => '</div>',
				'#markup' => $callback_help,
			),
		),
    'description' => array(
      '#type' => 'textarea',
      '#title' => t('Detailed description about the current link'),
      '#description' => t('This description will be displayed in a separate block you have to enable.'),
      '#default_value' => isset($link->description) ? check_plain($link->description) : '',
    ),
    'nid' => array(
      '#type' => 'hidden',
      '#value' => $nid,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => empty($lid) ? t('Add link') : t('Edit link'),
    ),
  );

  if (!empty($lid)) {
    $form['lid'] = array(
      '#type' => 'hidden',
      '#value' => check_plain($lid),
    );
  }

  guidance_set_breadcrumb($nid, $lid);

  return $form;
}


/**
 * Validation for the link form
 */
function guidance_link_form_validate($form, $form_state) {
  if (strlen($form_state['values']['url']) == 0) {
    form_set_error('url', t('Missing url.'));
  }
}


/**
 * Submit for the link form
 */
function guidance_link_form_submit($form, $form_submit) {
  $link = new stdClass();
  $link->title = check_plain($form_submit['values']['title']);
  $link->url = $form_submit['values']['url'];
  $link->nid = check_plain($form_submit['values']['nid']);
  $link->description = check_plain($form_submit['values']['description']);
  $link->callback = check_plain($form_submit['values']['callback']);

  if (!empty($form_submit['values']['lid'])) {
    $link->lid = $form_submit['values']['lid'];
    drupal_write_record('guidance_links', $link, array('lid'));
  } else {
    drupal_write_record('guidance_links', $link);
  }

  drupal_set_message(t('Link saved.'));
  drupal_goto('node/'.$form_submit['values']['nid'].'/guidance');
}


/**
 * Form for deleting a link.
 */
function guidance_link_delete_form($form, $form_status, $lid) {
  $link = guidance_get_guide_link($lid);
  drupal_set_title(t('Deleting link: @title', array('@title' => _guidance_title_format($link->title, $link->url))));
  $form = array(
    'lid' => array(
      '#type' => 'hidden',
      '#value' => $lid,
    ),
  );

  return confirm_form($form, t('Are you sure about deleting this link?'), 'node/' . $link->nid . '/guidance', t('This action cannot be undone.'));
}


/**
 * Submission for the link delete form.
 */
function guidance_link_delete_form_submit($form, $form_state) {
  $link = guidance_get_guide_link($form_state['values']['lid']);
  db_query('DELETE FROM {guidance_links} WHERE lid = :lid;', array(':lid' => $form_state['values']['lid']));
  drupal_set_message(t('Link deleted.'));
  drupal_goto('node/'.$link->nid.'/guidance');
}

