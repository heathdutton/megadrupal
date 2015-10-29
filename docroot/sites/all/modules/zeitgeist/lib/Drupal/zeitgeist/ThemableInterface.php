<?php

/**
 * @file
 * The interface for any class defining themable artefacts.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * The interface for declaring theme artefacts.
 *
 * @see zeitgeist_theme()
 */
interface ThemableInterface {
  /**
   * Delegated implementation of the theme hook.
   */
  public function hookTheme($existing, $type, $theme, $path);
}
