<?php

/**
 * @file
 * Theme file for the wow character module.
 */

/**
 * Returns HTML for a character.
 *
 * @param array $variables
 *   An associative array containing:
 *   - character: The user's character.
 *
 * @ingroup themeable
 */
function theme_wow_character($variables) {
  $output = '';
  $character = $variables['character'];
  $uri = wow_character_uri($character);

  $class = '<a class="class">' . check_plain(wow_character_classes($character->class)->name) . '</a>';
  $race = '<a class="race">' . check_plain(wow_character_races($character->race)->name) . '</a>';
  $level = '<span class="level"><strong>' . check_plain($character->level) . '</strong></span>';

  $output .= '<div class="profile-info">';
  $output .= '<div class="name">';
  $output .= l($character->name, $uri['path']);
  $output .= '</div>';
  $output .= '<div class="under-name color-c' . $character->class . '">';
  $output .= t("!race !class of level !level", array('!race' => $race, '!class' => $class, '!level' => $level));
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for a character description.
 *
 * @param array $variables
 *   An associative array containing:
 *   - character: The user's character.
 *
 * @ingroup themeable
 */
function theme_wow_character_description($variables) {
  $character = $variables['character'];
  $args = array(
    '@class' => wow_character_class($character)->name,
    '@race' => wow_character_race($character)->name,
    '@level' => $character->level,
  );

  return '<span class="color-c' . $character->class . '">' . t('@level @race @class', $args) . '</span>';
}

/**
 * Returns HTML for a character avatar.
 *
 * @param array $variables
 *   An associative array containing:
 *   - character: The user's character.
 *
 * @ingroup themeable
 */
function theme_wow_character_avatar($variables) {
  global $language;
  $character = $variables['character'];
  $region = variable_get('wow_region', 'us');
  $host = wow_api_info($region, 'host');

  $output = '';

  $url = "http://$host/wow/$language->language/character/$character->realm/$character->name/";

  $path = "http://$host/static-render/$region/$character->thumbnail";
  $options = array(
    'query' => array('alt' => '/wow/static/images/2d/avatar/' . $character->race . '-' . $character->gender . '.jpg'),
  );
  $avatar = theme('image', array('path' => url($path, $options)));

  // TODO: tpl file !?
  $output .= '<div class="avatar">';
  $output .= '<div class="avatar-interior">';
  $output .= l($avatar, $url, array('html' => TRUE));
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for a character name.
 *
 * @param array $variables
 *   An associative array containing:
 *   - character: The user's character.
 *   - path: The path to link. Default to character entity.
 *
 * @ingroup themeable
 */
function theme_wow_character_name($variables) {
  // Do not redefine 'path' if provided.
  $variables += wow_character_uri($variables['character']);

  $options = array(
    'attributes' => array(
        // Adds a css class for character coloring.
      'class' => array('color-c' . $variables['character']->class)
    )
  );

  return l($variables['character']->name, $variables['path'], $options);
}

/**
 * Returns the HTML for a race icon.
 *
 * @param array $variables
 *   An associative array containing:
 *   - character: The user's character.
 *
 * @ingroup themeable
 */
function theme_wow_character_race($variables) {
  $character = $variables['character'];
  $content = wow_api_info($character->region, 'content');

  if ($variables['size'] <= 18) {
    $size_path = 18;
  }
  else {
    $size_path = 36;
  }

  $race = $variables['character']->race;
  $gender = $variables['character']->gender;

  $icon['#theme'] = 'image';
  $icon['#path'] = "http://$content/wow/icons/$size_path/race_{$race}_{$gender}.jpg";
  $icon['#alt'] = wow_character_race($character)->name;
  $icon['#width'] = $variables['size'];
  $icon['#height'] = $variables['size'];

  return drupal_render($icon);
}

/**
 * Returns the HTML for a class icon.
 *
 * @param array $variables
 *   An associative array containing:
 *   - character: The user's character.
 *
 * @ingroup themeable
 */
function theme_wow_character_class($variables) {
  $character = $variables['character'];
  $content = wow_api_info($character->region, 'content');

  if ($variables['size'] <= 18) {
    $size_path = 18;
  }
  else {
    $size_path = 36;
  }

  $class = $variables['character']->class;

  $icon['#theme'] = 'image';
  $icon['#path'] = "http://$content/wow/icons/$size_path/class_{$class}.jpg";
  $icon['#alt'] = wow_character_class($character)->name;
  $icon['#width'] = $variables['size'];
  $icon['#height'] = $variables['size'];

  return drupal_render($icon);
}
