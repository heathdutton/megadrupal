<?php
/**
 * @file:   Functions supporting the generation of XSLTPipe instances by
 *          traversing a directory structure.
 */

/**
 * Generate an XSLT pipeline configuration by inspecting the given directory
 *
 * @param $path The full path to a directory on the server filesystem which
 *              contains XSLT stylesheets.
 */
function feeds_xsltparser_config_generate_from_directory($path) {
  // Generate a list of all files in the first and second level of the
  // directory structure with an extension of .xsl or .xslt.
  $pathlist = glob($path . '/{*,*/*}.{[xX][sS][lL],[xX][sS][lL][tT]}',
    GLOB_MARK|GLOB_BRACE|GLOB_NOSORT);
  sort($pathlist);

  $extract_stylesheets = array();
  $counter_stylesheets = array();
  $preproc_stylesheets = array();
  $global_counter = array();
  $global_preproc = array();

  // Phase 1: Loop thru all files and assign them to the proper stylesheets
  // chain
  foreach ($pathlist as $abspath) {
    $relpath = drupal_substr($abspath, drupal_strlen($path) + 1);

    // Silently ignore directories whose names happened to match the glob
    if (drupal_substr($relpath, -1) == '/') {
      continue;
    }

    // Derive the label from path
    $matchcount = preg_match('/^_?([0-9]+[_-]?)?[a-z][a-z0-9_]*/i', $relpath, $matches);
    if ($matchcount != 1) {
      drupal_set_message(t('Files and directories in the first level of the XSLT pipeline directory must begin with a character. Please rename %path appropriately, otherwise the path is ignored.', array('%path' => $relpath)), 'warning');
      continue;
    }

    $label = reset($matches);
    $label = preg_replace('/^(_?)([0-9]+[_-]?)?([a-z].*)$/i', '\\1\\3', $label);

    // Handle global magic chains
    if ('_count' == $label) {
      $global_counter[] = $abspath;
      continue;
    }
    elseif ('_preproc' == $label) {
      $global_preproc[] = $abspath;
      continue;
    }
    elseif ($label[0] == '_') {
      drupal_set_message(t('Pipes beginning with an underscore are reserved for internal use, therefore %label was ignored. Currently only the special pipes <_preproc</em> and <em>_count</em> are supported.', array('%label' => $label)), 'warning');
      continue;
    }

    // Handle per chain counters and preprocessors
    if ('_count' == drupal_substr($label, -6)) {
      $label = drupal_substr($label, 0, -6);
      $counter_stylesheets[$label][] = $abspath;
    }
    elseif ('_preproc' == drupal_substr($label, -8)) {
      $label = drupal_substr($label, 0, -8);
      $preproc_stylesheets[$label][] = $abspath;
    }
    // All other stylesheets are assigned to an extract-chain.
    else {
      $extract_stylesheets[$label][] = $abspath;
    }
  }

  // Phase 2: generate the XSLT pipe configuration

  // Build up the pipe- and chain-configurations used to extract field values
  $field_extractors = array();
  foreach ($extract_stylesheets as $label => $paths) {
    $chain = array(
      'type' => 'XSLTChain',
      'stylesheets' => $paths,
    );

    $pipe = array();
    if (isset($counter_stylesheets[$label])) {
      $pipe['type'] = 'XSLTMap';
      $pipe['mapper'] = array(
        'type' => 'XSLTRepeat',
        'counter' => array(
          'type' => 'XSLTChain',
          'stylesheets' => $counter_stylesheets[$label]),
        'pipe' => $chain,
      );
      $pipe['paramKey'] = '_element';

      if (isset($preproc_stylesheets[$label])) {
        $pipe['preproc'] = array(
          'type' => 'XSLTChain',
          'stylesheets' => $preproc_stylesheets[$label]
        );
      }
    }
    else {
      $pipe = $chain;
    }

    $field_extractors[$label] = $pipe;
  }

  // Wrap the field extractors into an XSLTMap with static mapping
  $field_mapper = array(
    'type' => 'XSLTMap',
    'mapper' => array(
      'type' => 'ArrayIterator',
      'items' => $field_extractors,
    ),
    'paramKey' => '_field',
  );

  // Wrap the field mapper into toplevel XSLTMap with the global counter
  // and preprocessor
  if (!empty($global_counter)) {
    $mapper = array(
      'type' => 'XSLTRepeat',
      'counter' => array(
        'type' => 'XSLTChain',
        'stylesheets' => $global_counter,
      ),
      'pipe' => $field_mapper,
    );
  }
  else {
    $mapper = array(
      'type' => 'ArrayIterator',
      'items' => array(0 => $field_mapper),
    );
  }

  $outer_map = array(
    'type' => 'XSLTMap',
    'mapper' => $mapper,
    'paramKey' => '_pass',
  );

  if (!empty($global_preproc)) {
    $outer_map['preproc'] = array(
      'type' => 'XSLTChain',
      'stylesheets' => $global_preproc,
    );
  }

  return $outer_map;
}

/**
 * Find the XSLTMap responsible for the field mapping in the config structure
 * by looking for an array containing 'paramKey' => '_field'.
 */
function feeds_xsltparser_config_extract_fieldmap($config) {
  if (is_array($config)) {
    if (isset($config['type']) && $config['type'] == 'XSLTMap' &&
        isset($config['paramKey']) && $config['paramKey'] == '_field') {
      return $config;
    }

    foreach ($config as $value) {
      $result = feeds_xsltparser_config_extract_fieldmap($value);
      if (is_array($result)) {
        return $result;
      }
    }
  }

  return FALSE;
}


/**
 * Construct a pipe using the given configuration
 */
function feeds_xsltparser_config_construct_pipe($config, $logproxy, $logprefix = '') {
  switch ($config['type']) {
  case 'XSLTMap':
    $mapfunc = feeds_xsltparser_config_construct_pipe($config['mapper'], $logproxy, $logprefix);
    $mapobj = new XSLTMap($mapfunc);

    if (isset($config['paramKey'])) {
      $mapobj->setXsltParamKeyName($config['paramKey']);
    }
    if (isset($config['preproc'])) {
      $preproc = feeds_xsltparser_config_construct_pipe($config['preproc'], $logproxy, $logprefix . '_preproc');
      $mapobj->setPreprocessor($preproc);
    }

    $result = $mapobj;
    break;

  case 'XSLTChain':
    $chainobj = new XSLTChain();
    $chainobj->setLogProxy($logproxy, $logprefix);
    $paths = $config['stylesheets'];
    foreach ($paths as $path) {
      $chainobj->addProcessorFromPath($path);
    }

    $result = $chainobj;
    break;

  case 'XSLTRepeat':
    $counter = feeds_xsltparser_config_construct_pipe($config['counter'], $logproxy, $logprefix . '_counter');
    $pipe = feeds_xsltparser_config_construct_pipe($config['pipe'], $logproxy, $logprefix);
    $repobj = new XSLTRepeat($counter, $pipe);

    $result = $repobj;
    break;

  case 'ArrayIterator':
    $items = array();
    foreach ($config['items'] as $itemkey => $itemconfig) {
      $item = feeds_xsltparser_config_construct_pipe($itemconfig, $logproxy, $logprefix . '[' . $itemkey . ']');
      $items[$itemkey] = $item;
    }
    $result = new ArrayIterator($items);
    break;

  default:
    $result = NULL;
    break;
  }

  return $result;
}
