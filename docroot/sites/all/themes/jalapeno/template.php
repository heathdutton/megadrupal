<?php 

//function jalapeno_html_head_alter(&$head_elements) {
//  $head_elements['system_meta_content_type']['#attributes'] = array(
//    'charset' => 'utf-8',
//  );
//}

function jalapeno_preprocess_html(&$variables, $hook) {
	  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  if (!$variables['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $variables['classes_array'][] = drupal_html_class('section-' . $section);
  }

  // Store the menu item since it has some useful information.
  $variables['menu_item'] = menu_get_item();
  switch ($variables['menu_item']['page_callback']) {
    case 'views_page':
      // Is this a Views page?
      $variables['classes_array'][] = 'page-views';
      break;
    case 'page_manager_page_execute':
    case 'page_manager_node_view':
    case 'page_manager_contact_site':
      // Is this a Panels page?
      $variables['classes_array'][] = 'page-panels';
      break;
  }
}

// Preprocess variables for node.tpl.php.
function jalapeno_preprocess_node(&$variables) {
  if (variable_get('node_submitted_' . $variables['node']->type, TRUE)) {
    $variables['submitted'] = t('<div class="submitted-info">Written by !user</div><div class="submitted-info"> on !date</div>',
      array(
        '!user' => $variables['name'],
        '!date' => $variables['date'],
      )
    );
  }
  else {
    $variables['submitted'] = '';
  }
  $variables['unpublished'] = '';
  if (!$variables['status']) {
    $variables['unpublished'] = '<div class="unpublished">' . t('Unpublished') . '</div>';
  }


// Let's get that read more link out of the generated links variable!
  unset($variables['content']['links']['node']['#links']['node-readmore']);
	
// Now let's put it back as it's own variable! So it's actually versatile!
    $variables['newreadmore'] = t('<span class="newreadmore"> <a href="!title">Read More</a> </span>',
      array(
        '!title' => $variables['node_url'],
      )
);

  $variables['title_attributes_array']['class'][] = 'node-title';
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function jalapeno_preprocess_block(&$variables, $hook) {
  // Classes describing the position of the block within the region.
  if ($variables['block_id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  // The last_in_region property is set in zen_page_alter().
  if (isset($variables['block']->last_in_region)) {
    $variables['classes_array'][] = 'last';
  }
  $variables['classes_array'][] = $variables['block_zebra'];

  $variables['title_attributes_array']['class'][] = 'block-title';
}

function jalapeno_page_alter(&$page) {
  // Look in each visible region for blocks.
  foreach (system_region_list($GLOBALS['theme'], REGIONS_VISIBLE) as $region => $name) {
    if (!empty($page[$region])) {
      // Find the last block in the region.
      $blocks = array_reverse(element_children($page[$region]));
      while ($blocks && !isset($page[$region][$blocks[0]]['#block'])) {
        array_shift($blocks);
      }
      if ($blocks) {
        $page[$region][$blocks[0]]['#block']->last_in_region = TRUE;
      }
    }
  }
}

function jalapeno_preprocess_comment(&$variables, $hook) {
  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $variables['node']->type, 1) == 0) {
    $variables['title'] = '';
  }

  // Anonymous class is broken in core. See #1110650
  if ($variables['comment']->uid == 0) {
    $variables['classes_array'][] = 'comment-by-anonymous';
  }
  // Zebra striping.
  if ($variables['id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  if ($variables['id'] == $variables['node']->comment_count) {
    $variables['classes_array'][] = 'last';
  }
  $variables['classes_array'][] = $variables['zebra'];

  $variables['title_attributes_array']['class'][] = 'comment-title';
}

// Changes the search form to use the "search" input element of HTML5
function jalapeno_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/**
 * Set a class on the iframe body element for WYSIWYG editors. This allows you
 * to easily override the background for the iframe body element.
 * This only works for the WYSIWYG module: http://drupal.org/project/wysiwyg
 */
function jalapeno_wysiwyg_editor_settings_alter(&$settings, &$context) {
  $settings['bodyClass'] = 'wysiwygeditor';
}

/*
 * An example page level function override for a later date
 */
//function jalapeno_preprocess_page(&$variables) {
//  $output = '<span class="newreadmore">';
//	$output .= 'NEW READ MORE';
//	$output .= '</span>';
//
//	$variables['newreadmore'] = $output;
//}