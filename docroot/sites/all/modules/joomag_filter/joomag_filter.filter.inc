<?php

/**
 * @file
 * Include file for joomag_filter.
 */

/**
 * Generates the proper HTML markup code.
 *
 * Given a $text with a joomag embed code, it will generate the proper HTML
 * markup code.
 *
 * @param string $text
 *   Text written by the author containing joomag embed code.
 *
 * @return string
 *   A copy of $text with joomag embed code replaced by the proper
 *   markup.
 *
 * @see http://goo.gl/DKoQOH
 */
function joomag_filter_parser($text) {
  // Magazine code replacement. See http://goo.gl/Txi0vO
  $regex = '/\[joomag ([^]]*)\]/i';
  preg_match_all($regex, $text, $matches);
  for ($x = 0; $x < count($matches[0]); $x++) {
    $replace = joomag_filter_get_magazine($matches[0][$x]);
    $text = str_replace($matches[0][$x], $replace, $text);
  }

  // Bookshelf code replacement. See http://goo.gl/q8hVjk
  $regex = '/\[joomag_bookshelf ([^]]*)\]/i';
  preg_match_all($regex, $text, $matches);
  for ($x = 0; $x < count($matches[0]); $x++) {
    $replace = joomag_filter_get_bookshelf($matches[0][$x]);
    $text = str_replace($matches[0][$x], $replace, $text);
  }

  return $text;
}

/**
 * Get bookshelf IFRAME code.
 *
 * Given a $match with a joomag embed code, it first performs a find/replace
 * of the following params:
 * height (defaults to 460px),
 * width (defaults to 450px),
 * magazineId (required),
 * cols (default to 3),
 * rows (defaults to 2)
 * and then builds the IFRAME markup code for a bookshelf.
 *
 * @param string $match
 *   Joomag bookshelf embed code. See http://goo.gl/q8hVjk.
 *
 * @return string
 *   IFRAME markup code of the Joomag bookshelf according to the
 *   parameters specified in $match.
 */
function joomag_filter_get_bookshelf($match) {
  $height = _joomag_filter_get_value_with_default('/(?:^|[\s]+)height=([\S]*)/i', $match, 460);
  $width = _joomag_filter_get_value_with_default('/(?:^|[\s]+)width=([\S]*)/i', $match, 450);
  $magazine_id = _joomag_filter_get_value_with_default('/(?:^|[\s]+)magazineId=([\S]*)/i', $match, '');
  $cols = _joomag_filter_get_value_with_default('/(?:^|[\s]+)cols=([\S]*)/i', $match, 3);
  $rows = _joomag_filter_get_value_with_default('/(?:^|[\s]+)rows=([\S]*)/i', $match, 2);

  $embed_code_str = '<iframe name="Joomag_embed_${UUID}"' . ' style="width:${width};height:${height}" width="${width}" height="${height}" hspace="0" vspace="0" frameborder="0" ' . ' src="${bookshelfURL}&cols=${cols}&rows=${rows}"></iframe>';

  $domain = 'www.joomag.com';

  $bookshelf_url = '//' . $domain . '/Frontend/embed/bookshelf/index.php?UID=' . $magazine_id;

  $uuid = sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x', mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0x0fff) | 0x4000, mt_rand(0, 0x3fff) | 0x8000, mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff));

  $embed_code_str = str_replace('${bookshelfURL}', $bookshelf_url, $embed_code_str);
  $embed_code_str = str_replace('${width}', $width . 'px', $embed_code_str);
  $embed_code_str = str_replace('${height}', $height . 'px', $embed_code_str);
  $embed_code_str = str_replace('${cols}', $cols, $embed_code_str);
  $embed_code_str = str_replace('${rows}', $rows, $embed_code_str);
  $embed_code_str = str_replace('${UUID}', $uuid, $embed_code_str);

  return $embed_code_str;
}

/**
 * Get magazine IFRAME code.
 *
 * Given a $match with a joomag embed code, it first performs a find/replace
 * of the following params:
 * height (defaults to 460px),
 * width (defaults to 450px),
 * pageNumber (defaults to 1),
 * magazineId (required),
 * backgroundColor,
 * backgroundImage,
 * toolbar (none, transparent, or custom),
 * autoflip,
 * autoFit (defaults to FALSE)
 * and then builds the IFRAME markup code for a magazine.
 *
 * @param string $match
 *   Joomag magazine embed code. See http://goo.gl/Txi0vO.
 *
 * @return string
 *   IFRAME markup code of the Joomag magazine according to the
 *   parameters specified in $match.
 */
function joomag_filter_get_magazine($match) {
  $height = _joomag_filter_get_value_with_default('/(?:^|[\s]+)height=([\S]*)/i', $match, 272);
  $width = _joomag_filter_get_value_with_default('/(?:^|[\s]+)width=([\S]*)/i', $match, 420);
  $page_number = _joomag_filter_get_value_with_default('/(?:^|[\s]+)pageNumber=([\S]*)/i', $match, 1);
  $magazine_id = _joomag_filter_get_value_with_default('/(?:^|[\s]+)magazineId=([\S]*)/i', $match, '');
  $title = _joomag_filter_get_value_with_default('/(?:^|[\s]+)title=([\S]*)/i', $match, '');
  $background_color = _joomag_filter_get_value_with_default('/(?:^|[\s]+)backgroundColor=([\S]*)/i', $match, '');
  $background_img = _joomag_filter_get_value_with_default('/(?:^|[\s]+)backgroundImage=([\S]*)/i', $match, '');
  $toolbar = _joomag_filter_get_value_with_default('/(?:^|[\s]+)toolbar=([\S]*)/i', $match, '');
  $auto_flip = _joomag_filter_get_value_with_default('/(?:^|[\s]+)autoFlip=([\S]*)/i', $match, '');
  $auto_fit = _joomag_filter_get_value_with_default('/(?:^|[\s]+)autoFit=([\S]*)/i', $match, 'FALSE') == 'TRUE' ? TRUE : FALSE;

  $embed_code_str = '<IFRAME NAME="Joomag_embed_${UUID}"' . ' style="width:${width};height:${height}" WIDTH="${width}" HEIGHT="${height}" HSPACE="0" VSPACE="0" frameborder="0" ' . ' SRC="${magURL}?page=${startPage}&e=1${otherOptions}"></IFRAME>';

  $domain = 'www.joomag.com';

  $viewer_url = '//' . $domain . '/magazine/' . $title . '/' . $magazine_id;

  $embed_opts = array();
  _joomag_filter_get_embed_opts_toolbar($embed_opts, $toolbar);
  _joomag_filter_get_embed_opts_bkg_color($embed_opts, $background_img, $background_color);

  $embed_opts_str = '&embedInfo=' . implode(';', $embed_opts);
  if (is_numeric($auto_flip)) {
    $embed_opts_str .= "&autoFlipDelay={$auto_flip}";
  }

  if ($auto_fit == TRUE) {
    $embed_code_str = str_replace('${width}', '100%', $embed_code_str);
    $embed_code_str = str_replace('${height}', '100%', $embed_code_str);
  }
  else {
    $embed_code_str = str_replace('${width}', $width . 'px', $embed_code_str);
    $embed_code_str = str_replace('${height}', $height . 'px', $embed_code_str);
  }

  $embed_code_str = str_replace('${startPage}', $page_number, $embed_code_str);

  $embed_code_str = str_replace('${otherOptions}', $embed_opts_str, $embed_code_str);

  $uuid = sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x', mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0x0fff) | 0x4000, mt_rand(0, 0x3fff) | 0x8000, mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff));

  $embed_code_str = str_replace('${magURL}', $viewer_url, $embed_code_str);
  $embed_code_str = str_replace('${UUID}', $uuid, $embed_code_str);

  return $embed_code_str;
}

/**
 * A wrapper to preg_match_all().
 *
 * Wraps in order to return a default value if no match is found.
 *
 * @param string $regex
 *   A regexp to extract the needed parameter value out of $params.
 * @param string $params
 *   In this context, Joomag magazine embed code. See http://goo.gl/Txi0vO.
 * @param string $default
 *   A default value if there's no match of $regex in $params.
 *
 * @return string
 *   Matched $regex in $params, otherwise $default.
 */
function _joomag_filter_get_value_with_default($regex, $params, $default) {
  $match_count = preg_match_all($regex, $params, $matches);
  if ($match_count) {
    return $matches[1][0];
  }
  else {
    return $default;
  }
}

/**
 * Joomag embed options, according to toolbar property.
 *
 * @param array $embed_opts
 *   The array of embed options passed by reference to be added/modified.
 * @param string $toolbar
 *   The toolbar property in the editor's code block ($match): empty, 'none',
 *   or 'transparent'.
 */
function _joomag_filter_get_embed_opts_toolbar(array &$embed_opts, $toolbar) {
  if ($toolbar != '') {
    switch ($toolbar) {
      case 'none':
        array_push($embed_opts, 'noToolbar');
        break;

      case 'transparent':
        array_push($embed_opts, 'none');
        break;

      default:
        array_push($embed_opts, "solid,{$toolbar}");
        break;
    }
  }
  else {
    array_push($embed_opts, '');
  }
}

/**
 * Joomag embed options, according to backgroundColor.
 *
 * @param array $embed_opts
 *   The array of embed options passed by reference to be added/modified.
 * @param string $background_color
 *   The backgroundColor property in the editor's code block ($match).
 */
function _joomag_filter_get_embed_opts_bkg_color(array &$embed_opts, $background_img, $background_color) {
  if ($background_color != '') {
    $bg_colors = explode(',', $background_color);

    if ($background_color == 'transparent') {
      array_push($embed_opts, 'none');
    }
    elseif (is_array($bg_colors) && count($bg_colors) == 2) {
      array_push($embed_opts, "gradient,{$background_color}");
    }
    else {
      array_push($embed_opts, "solid,{$background_color}");
    }
  }
  else {
    if ($background_img != '') {
      array_push($embed_opts, "image,{$background_img},fill");
    }
    else {
      array_push($embed_opts, '');
    }
  }
}
