<?php

/**
 * @file
 * Administration pages for widget settings.
 */

/**
 * Menu callback; Listing of all current widget sets.
 */
function enterprise_base_apps_structures_list() {
  $page = array();

  $apps_structures = enterprise_base_apps_structures();
  $page['widgets_set_list'] = array(
    '#markup' => theme('enterprise_base_apps_structures_list', array('apps_structures' => $apps_structures)),
    //'#attached' => array(
    //  'css' => array(drupal_get_path('module', 'widgets') . '/widgets.admin.css' => array()),
    //),
  );

  return $page;
}

/**
 * Returns HTML for the page containing the list of widget sets.
 *
 * @param $variables
 *   An associative array containing:
 *   - sets: An array of all the widget sets returned by widgets_get_sets().
 *
 * @see widgets_get_sets()
 * @ingroup themeable
 */
function theme_enterprise_base_apps_structures_list($variables) {  
  $apps_structures = $variables['apps_structures'];

  $header = array(t('App'), array('data' => t('Operations'), 'colspan' => 1));
  $rows = array();
  foreach ($apps_structures as $apps_structure) {
    $row = array();
    $row[] = $apps_structure['title'];
    $row[] = l(t('configure blocks'), 'admin/structure/enterprise_base/blocks/edit/' . $apps_structure['name']);
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 1,
      'data' => t('There are currently no apps to configure.'),
    ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

function enterprise_base_apps_structures_blocks_form($form, &$form_state, $app_structures) {
	drupal_add_css(drupal_get_path('module', 'enterprise_base') . '/css/enterprise_base.admin.css');
  $form = array();
  $theme = variable_get('theme_default', 'bartik');

  // Fetch and sort blocks.
  require_once(drupal_get_path('module', 'block') . '/block.admin.inc');
  $app_block_deltas = array();
  foreach ($app_structures['blocks'] AS $key => $block_struc) {
    list($module, $delta) = explode(':', $key);
    // check if views is using a hash
    if (($module == 'views') && (strlen($delta) >= 32)) {
      $hash = md5($delta);
      $app_structures['blocks']['views:' . $hash] = $app_structures['blocks'][$key];
      $delta = $hash;
      unset($app_structures['blocks'][$key]);
    }
    $app_block_deltas[$delta] = 1;
  }
  $blocks = block_admin_display_prepare_blocks($theme);
  $app_blocks = array();
  foreach($blocks as $block) {
    if (isset($app_block_deltas[$block['delta']])) {
      $app_blocks[] = $block;
    }
  }
//dsm($blocks);
//dsm($app_structures);
//dsm($app_blocks);

  $block_regions = system_region_list($theme);

  $form['app_structures'] = array(
    '#type' => 'value',
    '#value' => $app_structures,
  );
  $form['#tree'] = TRUE;

  foreach ($app_blocks as $i => $block) {

    $key = $block['module'] . '_' . $block['delta'];

    $app_blocks_struc = $app_structures['blocks'][$block['module'] . ':' . $block['delta']];
    $block['node_types'] = db_query("SELECT type FROM {block_node_type} WHERE module = :module AND delta = :delta", array(
      ':module' => $block['module'],
      ':delta' => $block['delta'],
    ))->fetchCol();

    $result = db_query("SELECT * FROM {block_views} WHERE module = :module AND delta = :delta", array(
      ':module' => $block['module'],
      ':delta' => $block['delta'],
	  ))->fetchAll();
	  $block['views'] = array();
	  if (is_array($result)) {
	    foreach ($result AS $row) {
	      $block['views'][] = "$row->view:$row->display";
	    }
	  } 
	  
	  $block['pages_array'] = array();
	  if (trim($block['pages'])) {
	    $block['pages_array'] = explode("\n", $block['pages']); 
	  }  
    
    $form['blocks'][$key] = array(
      '#type' => 'fieldset',
      '#title' => t('@block', array('@block' => $block['info'])),
      '#description' =>  isset($app_blocks_struc['description']) ? $app_blocks_struc['description'] : '',
    );
    $form['blocks'][$key]['module'] = array(
      '#type' => 'value',
      '#value' => $block['module'],
    );
    $form['blocks'][$key]['delta'] = array(
      '#type' => 'value',
      '#value' => $block['delta'],
    );
    $form['blocks'][$key]['theme'] = array(
      '#type' => 'value',
      '#value' => $theme,
    );
    $form['blocks'][$key]['weight'] = array(
      '#type' => 'value',
      '#value' => $block['weight'],
    );
    $suffix = '';
    $default = '';
    if (($block['region'] == BLOCK_REGION_NONE) && isset($app_blocks_struc['defaults']['regions'])) {
    	foreach ($app_blocks_struc['defaults']['regions'] AS $region) {
    		if (isset($block_regions[$region])) {
    			$default = $region;
    			$suffix = ' ' . t('(currently hidden)');
    			break;
    		}
    	}
    }
    else {
    	$default = $block['region'] != BLOCK_REGION_NONE ? $block['region'] : NULL;
    }
    $form['blocks'][$key]['region'] = array(
      '#type' => 'select',
      '#default_value' => $default,
      '#empty_value' => BLOCK_REGION_NONE,
      '#title' => t('Region'),
      '#options' => $block_regions,
      '#field_suffix' => $suffix,
    );
    $options = array();
    $defaults = array();
    $checks = array('path', 'node_type', 'views');
    $custom = $block;
    if (isset($app_blocks_struc['visibility']) && !empty($app_blocks_struc['visibility'])) {
	    foreach ($app_blocks_struc['visibility'] AS $k => $b) {
	      $options[$k] = $b['title'];
	      $b += array(
	        'path' => array(
	          'pages' => array(),
	          'visibility' => 0,
	        ),
	        'node_type' => array(
	          'types' => array(),
	        ),
	        'views' => array(
	          'views' => array(),
	        ),
	      );
	      $bc = $b;
	//dsm($b);
	//dsm($block);     
	      
	      if (($b['path']['visibility'] == $block['visibility'])) {
	      	foreach ($block['pages_array'] AS $i => $page) {
	      		if ($bci = array_search($page, $b['path']['pages'])) {
	      			unset($bc['path']['pages'][$bci]);
	      			unset($custom['pages'][$i]);
	      		}
	      	}
	      }
	      foreach ($block['node_types'] AS $i => $type) {
	//dsm($type);
	        $bci = array_search($type, $b['node_type']['types']);
	        if ($bci !== FALSE) {
	//dsm($bci);
	          unset($bc['node_type']['types'][$bci]);
	          unset($custom['node_types'][$i]);
	        }
	      }
	      foreach ($block['views'] AS $i => $view) {
	      	$bci = array_search($view, $b['views']['views']);
	        if ($bci !== FALSE) {
	          unset($bc['views']['views'][$bci]);
	          unset($custom['views'][$i]);
	        }
	      }
	
	//dsm($bc);
	      if (empty($bc['path']['pages']) && empty($bc['node_type']['types']) && empty($bc['views']['views'])) {
	        $defaults[] = $k;
	      }
	    }
	//dsm($custom);
	//dsm($options);
	   $description_extra = '';
	   if (!empty($custom['pages_array'])|| !empty($custom['node_types']) || !empty($custom['views'])) {
	      $options['custom'] = t('Keep existing custom settings');
	      $defaults[] = 'custom';
	      $custom_preset = array(
	        'path' => array(
	          'pages' => $custom['pages_array'],
	          'visibility' => 0,
	        ),
	        'node_type' => array(
	          'types' => $custom['node_types'],
	        ),
	        'views' => array(
	          'views' => $custom['views'],
	        ),      
	      );
	      
	      $form['blocks'][$key]['custom_preset'] = array(
	        '#type' => 'value',
	        '#value' => $custom_preset,
	      ); 
	      $description_extra = ' ' . t('There are some existing custom visibility settings. These settings may conflict with presets. To keep the existing settings leave "Keep existing..." option selected.'); 
	    }
	    if ($block['region'] == -1 && isset($app_blocks_struc['defaults']['visibility'])) {
	      $defaults = $app_blocks_struc['defaults']['visibility'];
	    }
	    $description_extra .= ' ' . t('For advanced settings see the !link.',
	      array(
	        '!link' => l(t('block configure form'), 'admin/structure/block/manage/' . $block['module'] . '/' . $block['delta'] . '/configure', array('attributes' => array('target' =>  '_blank'))),
	      )
	    );
	    $form['blocks'][$key]['visiblity_presets'] = array(
	      '#type' => 'checkboxes',
	      '#default_value' => $defaults,
	      '#title' => t('Visiblity'),
	      '#options' => $options,
	      '#description' => t('Use the check boxes to display the block only on specific pages within your site. Leave all selections unchecked to show on all pages.') . $description_extra,
	      '#attributes' => array(
	        'class' => array('scrollable'),
	      )
	    );
	    $form['blocks'][$key]['visiblity_presets_default'] = array(
	      '#type' => 'value',
	      '#value' => $defaults,
	    );    
    }
    
  }
  $form['blocks']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  // Use the default block admin form submit.
  //$form['#submit'][] = 'enterprise_base_apps_structures_blocks_form_submit';
  //$form['#submit'][] = 'block_admin_display_form_submit';

  return $form;
}

function enterprise_base_apps_structures_blocks_form_validate($form, &$form_state) {
	
}

function enterprise_base_apps_structures_blocks_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  // check if tree from apps config or standard config page  
  if (!isset($values['app_structures']) && isset($values['blocks']['app_structures'])) {
    $app_structures = $values['blocks']['app_structures'];
    $blocks = $values['blocks']['blocks'];
  } 
  else {
  	$app_structures = $values['app_structures'];
    $blocks = $values['blocks'];
  }
  $base_weights = array();
  $default_theme = variable_get('theme_default', NULL);
  foreach ($blocks AS $name => $block) {
    // skip if not a real block
  	if (!is_array($block) || !isset($block['module']) || !$block['module']) {
      continue;
    }
  	$region = $block['region'];
  	// determine the highest weight of blocks in each region
  	if (($region != -1) && !isset($base_weights[$region])) {
  		$query = db_select('block')
  		  ->fields('block', array('weight'))
        ->condition('theme', $default_theme)
        ->condition('module', $block['module'])
        ->condition('delta', $block['delta']);
      $base_weights[$region] = $query->execute()->fetchField();
      if (!$base_weights[$region]) {
      	$base_weights[$region] = -10;
      }
  	}
  	$visiblity = 0;
  	$pages = array();
  	$types = array();
  	$views = array();
  	if (isset($block['visiblity_presets']) && is_array($block['visiblity_presets'])) {
      $app_blocks_struc = $app_structures['blocks'][$block['module'] . ':' . $block['delta']];
      foreach ($block['visiblity_presets'] AS $preset_name) {
        if (!$preset_name) {
        	continue;
        }
      	if ($preset_name == 'custom') {
      		$preset = $block['custom_preset'];
      	}
      	elseif (isset($app_blocks_struc['visibility'][$preset_name])) {
	        $preset = $app_blocks_struc['visibility'][$preset_name];
      	}
      	else {
      		$preset = array();
      	}

	      $preset += array(
	        'path' => array(
	          'pages' => array(),
	          'visibility' => 0,
	        ),
	        'node_type' => array(
	          'types' => array(),
	        ),
	        'views' => array(
	          'views' => array(),
	        ),
	      );
	      $visibility = $preset['path']['visibility'];
	      foreach ($preset['path']['pages'] AS $page) {
	        $pages[$page] = $page;
	      }
        foreach ($preset['node_type']['types'] AS $type) {
          $types[$type] = $type;
        }
        foreach ($preset['views']['views'] AS $view) {
          $views[$view] = $view;
        }      	
      }
  	}
//dsm($block);
//dsm($region);
//dsm($pages);
//dsm($types);
//dsm($views);
    $fields = array(
      'region' => ($region) ? $region : -1,
      'status' => ($region) ? 1 : 0,
      'visibility' => (int) $visiblity,
      'pages' => implode("\n", $pages),
    );
    if (isset($base_weights[$region])) {
      $fields['weight'] = $base_weights[$region] + isset($app_blocks_struc['weight']) ? $app_blocks_struc['weight'] : 0;
    }
    $query = db_update('block')
      ->fields($fields)
      ->condition('theme', $default_theme)
      ->condition('module', $block['module'])
      ->condition('delta', $block['delta']);
//dsm(" " . $query);
    $query->execute();
      
    db_delete('block_node_type')
      ->condition('module', $block['module'])
      ->condition('delta', $block['delta'])
      ->execute();
    foreach ($types AS $i => $type) {
      $fields = array(
        'module' => $block['module'],
        'delta' => $block['delta'],
        'type' => $type,
      );
      db_insert('block_node_type')
        ->fields($fields)
        ->execute();
    }
      
    db_delete('block_views')
      ->condition('module', $block['module'])
      ->condition('delta', $block['delta'])
      ->execute();      
    foreach ($views AS $i => $viewdis) {
      list($view, $display) = explode(':', $viewdis);
      $fields = array(
        'module' => $block['module'],
        'delta' => $block['delta'],
        'view' => $view,
        'display' => $display,
      );
      db_insert('block_views')
        ->fields($fields)
        ->execute();
    }
  }
  
  //dsm($form);
  //dsm($form_state['values']);
}