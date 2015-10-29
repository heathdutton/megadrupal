<?php

/**
 * @file
 * Provide environments to render maths and/or arbitrary TeX.
 */

/**
 * Implements hook_info().
 */
function drutex_render_info($filter) {
  return (object) array(
    'title' => t('LaTeX Renderer'),
    'description' => t('Provides different environments to create rendered images (especially maths).'),
    'toggle' => TRUE,
    'weight' => 0,
  );
}

/**
 * Implementation of subhook_filter_tips().
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drutex_render_filter_tips($delta, $filter, $long = FALSE) {
  return t('Provides different environments to create rendered images (especially maths).');
}

/**
 * Implements hook_defaults().
 */
function drutex_render_defaults() {
  $D['drutex_render_active'] = TRUE;

  /* relative to drutex/templates/render */
  $D['drutex_template'] = 'default.tex';

  $D['drutex_pattern_dvipng']           = 'cd [TMP_DIR]; TEXINPUTS="[DRUTEX_DIR]//:" latex -interaction=batchmode [HASH].tex' . "\n" .
                                      'dvipng -o [IMG_FILE] -D [DPI] -T tight [TMP_DIR]/[HASH].dvi';

  $D['drutex_pattern_imagemagick']      = 'cd [TMP_DIR]; TEXINPUTS="[DRUTEX_DIR]//:" latex -interaction=batchmode [HASH].tex' . "\n" .
                                      'convert -density [DPI] -trim [TMP_DIR]/[HASH].dvi [IMG_FILE]';

  $D['drutex_pattern_dvipng_trans']      = 'cd [TMP_DIR]; TEXINPUTS="[DRUTEX_DIR]//:" latex -interaction=batchmode [HASH].tex' . "\n" .
                                      'dvipng -o [IMG_FILE] -D [DPI] -T tight -bg Transparent [TMP_DIR]/[HASH].dvi';

  $D['drutex_pattern_imagemagick_trans'] = 'cd [TMP_DIR]; TEXINPUTS="[DRUTEX_DIR]//:" latex -interaction=batchmode [HASH].tex' . "\n" .
                                      'convert -density [DPI] -trim -transparent "#FFFFFF" -antialias +repage [TMP_DIR]/[HASH].dvi [IMG_FILE]';

  $D['drutex_pattern_dvipng_win32']      = 'cd [TMP_DIR] & latex -interaction=batchmode [HASH].tex' . "\n" .
                                      'dvipng -o [IMG_FILE] -D [DPI] -T tight [TMP_DIR]/[HASH].dvi';

  $D['drutex_pattern_imagemagick_win32'] = 'cd [TMP_DIR] & latex -interaction=batchmode [HASH].tex' . "\n" .
                                      'convert -density [DPI] -trim [TMP_DIR]/[HASH].dvi [IMG_FILE]';

  return $D;
}

/**
 * Implements hook_node2html().
 */
function drutex_render_node2html() {
  $E = array();

  /* Plain TeX environment */
  $E[] = (object) array(
    'pattern' => '@<tex( [^>]*)?>(.+?)</tex>@se',
    'replacement' => "drutex_render('$2', $'<var>filter</var>', '$1', 'tag=\"span\" class=\"plaintex\"')",
    'weight' => -10,
  );

  /* <equation> environment */
  $E[] = (object) array(
    'pattern' => '@<equation( [^>]*)?>(.+?)</equation>@se',
    'replacement' => "drutex_render('\[ $2 \]', $'<var>filter</var>', '$1', 'tag=\"table\" class=\"displaymath\"')",
    'weight' => -7,
  );

  /* <equations> environment */
  $E[] = (object) array(
    'pattern' => '@<equations( [^>]*)?>(.+?)</equations>@se',
    'replacement' => "drutex_render('\begin{align*} $2 \end{align*}', $'<var>filter</var>', '$1', 'tag=\"table\" class=\"displaymath\"')",
    'weight' => -6,
  );

  /* Paragraph math between $$ and $$ */
  $E[] = (object) array(
    'pattern' => '@(^|[^\\\\])(\$\$.*?[^\\\\]\$\$)@se',
    'replacement' => "'$1' . drutex_render('$2', $'<var>filter</var>', '', 'tag=\"table\" class=\"displaymath\"')",
    'weight' => -5,
  );

  /* Paragraph math between \[ and \] */
  $E[] = (object) array(
    'pattern' => '@\\\\\\[(.+?)\\\\\\]@se',
    'replacement' => "drutex_render('$0', $'<var>filter</var>', '', 'tag=\"table\" class=\"displaymath\"')",
    'weight' => -5,
  );

  /* Inline math (display style) between $! and $ */
  $E[] = (object) array(
    'pattern' => '@(^|[^\\\\])\$!(.*?[^\\\\])\$@se',
    'replacement' => "'$1' . drutex_render('$\displaystyle $2 $', $'<var>filter</var>')",
    'weight' => -4,
  );

  /* Inline math between $ and $ */
  $E[] = (object) array(
    'pattern' => '@(^|[^\\\\])\$(.*?[^\\\\])\$@se',
    'replacement' => "'$1' . drutex_render('$ $2 $', $'<var>filter</var>')",
    'weight' => -3,
  );

  /* unescape escaped dollar signs */
  $E[] = (object) array(
    'pattern' => '@\\\\\\$@s',
    'replacement' => '$',
    'weight' => 1000 /*  very late */,
  );

  return $E;
}

/**
 * Implements hook_filter_settings().
 */
function drutex_render_filter_settings($filter) {
  /* conversion methods */
  $conversions = array(
    'dvipng' => 'dvipng',
    'imagemagick' => 'ImageMagick (convert)',
    'dvipng_trans' => 'dvipng - Transparent',
    'imagemagick_trans' => 'ImageMagick (convert) - Transparent',
    'dvipng_win32' => 'dvipng - Windows',
    'imagemagick_win32' => 'ImageMagick (convert) - Windows',
    'custom' => 'custom',
  );

  $form["drutex_template"] = array(
    '#type' => 'select',
    '#title' => t('Template'),
    '#options' => _drutex_get_templates(),
    '#default_value' => $filter->settings['drutex_render']['drutex_template'],
    
    
  );

  $form["drutex_dpi"] = array(
    '#type' => 'textfield',
    '#title' => t('Resolution (DPI)'),
    '#size' => 15,
    '#maxlength' => 3,
    '#default_value' => $filter->settings['drutex_render']['drutex_dpi'],
    '#description' => t('100 is small, 107 is default, 115 is big'),
  );

  $form["drutex_conversion"] = array(
    '#type' => 'select',
    '#title' => t('Conversion Method'),
    '#options' => $conversions,
    '#default_value' => $filter->settings['drutex_render']['drutex_conversion'],
  );


  /* Custom Conversion Method */

  /* determine default value for <custom conversion method> */
  $default_value =  $filter->settings['drutex_render']['drutex_pattern_render_custom'];
  if (empty($default_value)) {
    /* get selected conversion method */
    $conversion = $filter->settings['drutex_render']['drutex_conversion'];
    $default_value = $filter->settings['drutex_render']["drutex_pattern_{$conversion}"];
  }

  /* description for <custom conversion method> */
  $description = t('* Leave blank and select dvipng or ImageMagick, to display a template for one of these commands in this box.') . '<br />' . t('* Every line is executed in sequence, but in its own shell (so path changes aren\'t inherited).') . '<br />' . t('Placeholders available:<br />
        [HASH] - hash (name) for the image<br />
        [TMP_DIR] - temporary dir (automatically cleaned)<br />
        [IMG_DIR] - directory where the images are saved<br />
        [IMG_TYPE] - png in most cases<br />
        [DPI] - dots per inch (see above)<br />
        [IMG_FILE] - abbreviation for [IMG_DIR]/[HASH].[IMG_TYPE]<br />
        [DRUTEX_DIR] - full path to DruTeX base dir');


  $form["drutex_pattern_render_custom"] = array(
    '#type' => 'textarea',
    '#title' => t('Conversion Method (Custom)'),
    '#rows' => 3,
    '#cols' => 50,
    '#default_value' => $default_value,
    '#description' => $description,
  );

  return $form;
}

/**
 * Renders $text in LaTeX.
 */
function drutex_render($text, $filter, $attributes = '', $default_attributes = '') {

  $text = _drutex_unescape($text);


  if ($filter->settings['drutex_submodules']['security']) {
    if (drutex_security($text, $filter) == FALSE) {
      return '<em class="error">Unallowed command detected!</em>';
    }
  }

  $attributes = drutex_parse_attributes(_drutex_unescape($attributes));
  $default_attributes = drutex_parse_attributes($default_attributes);
  $attributes = array_merge($default_attributes, $attributes);

  $image_dir = $filter->settings['drutex_base']['drutex_dir_images'];
  $template_dir = drupal_get_path('module', 'drutex') . '/templates/render';
  $template_file = $filter->settings['drutex_render']['drutex_template'];
  $template = $template_dir . '/' . $template_file;

  $dpi = $filter->settings['drutex_render']['drutex_dpi'];

  if (!empty($attributes['dpi']) AND is_numeric($attributes['dpi']) AND $attributes['dpi'] > 0) {
    $dpi = $attributes['dpi'];
  }

  $image_type = $filter->settings['drutex_imagetype']; //TODO : check the correct setting;

  $hash = sha1($text . $template . $dpi);
  $image_file = "$image_dir/$hash.$image_type";


  $success = TRUE;

  if (!is_file($image_file)) {
    /* create temporary dir */
    $temporary_dir = _drutex_create_temporary_dir($filter);

    /* conversion method */
    $conversion = $filter->settings['drutex_render']['drutex_conversion'];
    /* command patterns */
    $pattern_convert =  $filter->settings['drutex_render']['drutex_pattern_render_custom'];
    $drutex_dir = $filter->settings['drutex_dir'];

    /* replacement arrays */
    $map = array(
      '[HASH]' => $hash,
      '[TMP_DIR]' => $temporary_dir,
      '[IMG_FILE]' => $image_file,
      '[IMG_DIR]' => $image_dir,
      '[IMG_TYPE]' => $image_type,
      '[DPI]' => $dpi,
      '[DRUTEX_DIR]' => $drutex_dir,
    );

    /* conversion commands (seperated by \n) */
    $cmd_convert = str_replace(array_keys($map), $map, $pattern_convert);

    /* load the appropriate template ... */
    $handle = fopen($template, 'r');
    $content = fread($handle, filesize($template));
    fclose($handle);

    /* ... and put the $text in there */
    $content = str_replace('DRUTEX_REPLACE', $text, $content);

    /* write $content to file, for LaTeX to read an render it */
    file_unmanaged_save_data($content, "$temporary_dir/$hash.tex", FILE_EXISTS_REPLACE);

    /* invoke all conversion commands */
    $commands = explode("\n", $cmd_convert);

    foreach ($commands as $cmd) {
      $cmd = trim($cmd);

      if ($cmd) {
        exec($cmd, $cmd_output, $cmd_retval);

        if ($filter->settings['drutex_tools']['drutex_debug']){
          $level = ($cmd_retval == 0) ? WATCHDOG_NOTICE : WATCHDOG_WARNING;
          watchdog('DruTeX', "Command: %cmd<br />Return value: %cmdret", array('%cmd' => $cmd, '%cmdret' => $cmd_retval), $level);
        }
      }
    }

    $success = is_file($image_file);

    /* remove temporary dir */
    if (!$filter->settings['drutex_tools']['drutex_debug']) {
      _drutex_delete_dir($temporary_dir);
    }
    else {
      if ($success) {
        watchdog('DruTeX', "%imgfile was created. Temporary directory was %tdir.", array('%imgfile' => $image_file, '%tdir' => $temporary_dir), WATCHDOG_NOTICE);
      }
      else {
        watchdog('DruTeX', "%imgfile couldn\'t be created. Temporary directory was %tdir.",   array('%imgfile' => $image_file, '%tdir' => $temporary_dir), WATCHDOG_WARNING);
      }
    }
  }

  $img_url = drutex_get_image_url($filter) . "/$hash.$image_type";

  if ($success) {
    $img_alt = _drutex_hide('set', check_plain($text));
    $res = '<img class="teximage" src="' . $img_url . '" alt="' . $img_alt . '" />';
  }
  else {
    $res = '<em class="error">TeX Embedding failed!</em>';
  }
  if (!empty($attributes['tag'])) {
    $class_attr = empty($attributes['class']) ? '' : " class=\"{$attributes['class']}\"";

    if (!empty($attributes['id'])) {
      $id = $attributes['id'];
      $anchor = "<a name=\"$id\" id=\"$id\"></a>";
    }
    else {
      $anchor = '';
    }

    if ($attributes['tag'] == 'table') {
    	
      $name = _drutex_references('get name by id', $id);
      $name = empty($name) ? '' : "($name)";
      $res = "<table{$class_attr}><tr><td class=\"dspleft\">{$anchor}{$res}</td><td class=\"dspright\">{$name}</td></tr></table>";
    }
    else {
      $tag = $attributes['tag'];
      $res = "<{$tag}{$class_attr}>{$res}</{$tag}>";
    }
  }

  return $res;
}

/**
 * Get a list with templates for drutex_render.
 */
function _drutex_get_templates() {
  $dir = drupal_get_path('module', 'drutex') . '/templates/render';

  $A = file_scan_directory($dir, '/.*\.tex/');
  $B = array();

  foreach ($A as $key => $val) {
    $key = substr($key, strlen($dir) + 1);
    $B[$key] = $key;
  }

  return $B;
}
