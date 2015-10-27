<?php
/**
 * @file
 * Theme and preprocess functions for menus
 */
/**
 * Implements theme_menu_link__management.
 * Radix does not theme navbar well,
 * So we need to beat it into shape
 */
function admiral_menu_link__management($variables) {
	$element = $variables['element'];
	$sub_menu = '';

	if (!empty($element['#below'])) {
    // Wrap in dropdown-menu.
		unset($element['#below']['#theme_wrappers']);
		$sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';
		$element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
		$element['#localized_options']['attributes']['data-toggle'] = 'dropdown';

    // Check if element is nested.
		if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] > 2)) {
			$element['#attributes']['class'][] = 'dropdown-submenu';
		}
		else {
			$element['#attributes']['class'][] = 'dropdown';
			$element['#localized_options']['html'] = TRUE;
			$element['#title'] .= '<span class="caret"></span>';
		}

		$element['#localized_options']['attributes']['data-target'] = '#';
	}

  // Fix for active class.
	if (($element['#href'] == current_path() || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']) || $element['#localized_options']['language']->language == $language_url->language)) {
		$element['#attributes']['class'][] = 'active';
	}

  // Add active class to li if active trail.
	if (in_array('active-trail', $element['#attributes']['class'])) {
		$element['#attributes']['class'][] = 'active';
	}

	$output = l($element['#title'], $element['#href'], $element['#localized_options']);
	return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}