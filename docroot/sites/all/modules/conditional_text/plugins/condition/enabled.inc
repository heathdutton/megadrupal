<?php

/**
 * @file
 * Enabled condition plugin.
 */

$plugin = array(
  'title' => t('Module related expressions'),
  'description' => t(''),
  'evaluate' => function($expression, $context = NULL) {
    list($modulename, $predicate, $givenversion) =
      conditional_text_enabled_plugin_parse_expression($expression);
    $version = conditional_text_enabled_plugin_determine_version(
      $context, $givenversion, $modulename);

    if ($version) {
      $vc = conditional_text_enabled_plugin_version_compare($version, $givenversion);

      switch ($predicate) {
        case '<':
          return $vc == -1;
        case '>':
          return $vc == 1;
        case '=': case '==':
          return $vc == 0;
        case '<=':
          return $vc == -1 || $vc == 0;
        case '>=':
          return $vc == 1 || $vc == 0;
      }
    }

    return FALSE;
  },
  'reason' => function($expression, $context = NULL) {
    list($modulename, $predicate, $givenversion) =
      conditional_text_enabled_plugin_parse_expression($expression);
    $pretty_modulename =
      conditional_text_enabled_plugin_get_module_name($modulename, $context);

    if ($givenversion == 0) {
      return t('%modulename is enabled', array(
        '%modulename' => $pretty_modulename,
      ));
    }
    else {
      ${'<'} = '%modulename, versions before %version';
      ${'>'} = '%modulename, versions after %version';
      ${'='} = ${'=='} = '%modulename, version %version';
      ${'<='} = '%modulename, version %version or earlier';
      ${'>='} = '%modulename, version %version or later';

      return isset(${$predicate}) ? t(${$predicate}, array(
        '%modulename' => $pretty_modulename,
        '%version' => $givenversion,
      )) : NULL;
    }
  },
  'settings' => function(&$form_state, $filter) {
    $form = array();

    $form['#tree'] = TRUE;

    $form['modules'] = array(
      '#type' => 'fieldset',
      '#title' => t('Modules enabled'),
      '#prefix' => '<div id="conditional-text-enabled-settings-wrapper">',
      '#suffix' => '</div>',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['modules']['data'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array(
        'conditional_text_form_table',
      ),
    );

    $form['modules']['data']['header'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array(
        'conditional_text_form_tr',
      ),
    );

    $form['modules']['data']['header'][] = array(
      '#markup' => t('Additional module'),
      '#theme_wrappers' => array(
        'conditional_text_form_th',
      ),
    );

    $form['modules']['data']['header'][] = array(
      '#markup' => t('Display name'),
      '#theme_wrappers' => array(
        'conditional_text_form_th',
      ),
    );

    if (empty($form_state['enabled']['num_modules'])) {
      $form_state['enabled']['num_modules'] = empty($filter['modules']) ?
        0 : count($filter['modules']['data']);
    }

    times($form_state['enabled']['num_modules'],
      function($i) use(&$form, $filter) {
        $form['modules']['data'][$i] = array(
          '#type' => 'container',
          '#theme_wrappers' => array(
            'conditional_text_form_tr',
          ),
        );

        $form['modules']['data'][$i]['module'] = array(
          '#type' => 'textfield',
          '#title' => t('Additional module'),
          '#description' => t('Filter as though this module/version is enabled. Example: ' .
                              'conditional_text-7.x-1.x'),
          '#element_validate' => array(
            'conditional_text_enabled_module_name_validate',
          ),
          '#default_value' => isset($filter['modules']['data'][$i]['module']) ?
            $filter['modules']['data'][$i]['module'] : NULL,
          '#theme_wrappers' => array(
            'form_element',
            'conditional_text_form_td',
          ),
          '#title_display' => 'invisible',
        );

        $form['modules']['data'][$i]['name'] = array(
          '#type' => 'textfield',
          '#title' => t('Display name'),
          '#description' => t('Optional.'),
          '#default_value' => isset($filter['modules']['data'][$i]['name']) ?
            $filter['modules']['data'][$i]['name'] : NULL,
          '#theme_wrappers' => array(
            'form_element',
            'conditional_text_form_td',
          ),
          '#title_display' => 'invisible',
        );
      });

    $form['modules']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add more'),
      '#submit' => array('conditional_text_enabled_add_button_submit'),
      '#ajax' => array(
        'callback' => 'conditional_text_enabled_add_button_callback',
        'wrapper' => 'conditional-text-enabled-settings-wrapper', // TODO
      ),
    );

    if ($form_state['enabled']['num_modules'] > 1) {
      $form['modules']['remove'] = array(
        '#type' => 'submit',
        '#value' => t('Remove last'),
        '#submit' => array('conditional_text_enabled_remove_button_submit'),
        '#ajax' => array(
          'callback' => 'conditional_text_enabled_add_button_callback',
          'wrapper' => 'conditional-text-enabled-settings-wrapper', // TODO
        ),
      );
    }

    return $form;
  },
  'settings include' => drupal_get_path('module', 'conditional_text') .
    '/plugins/condition/enabled/form.inc',
  'identifier token' => 'enabled',
  'short help' => repeat(t('Module enabled condition type: ' .
    '[condition enabled MODULENAME] or ' .
    '[condition enabled MODULENAME OP VERSION].')),
  'long help' => repeat(t(<<<MARKUP
Module enabled plugin: [condition enabled MODULENAME] or [condition enabled MODULENAME OP VERSION], where:
<ul>
<li>MODULENAME is the machine name of a module, such as views or block. The machine name of a module is the name of the module file (e.g., if the module file is views.module, the module name is views).</li>
<li>OP is an operator. Supported operators are: =, ==, &gt;, &lt;, &gt;=, &lt;=.</li>
<li>VERSION is a version number, such as 7.2, 7.x, 7.x-3.1, etc.</li>
</ul>
MARKUP
    )),
);

/**
 * Parses a given expression into standard format.
 *
 * @param array $expression
 *   Expression token list.
 *
 * @return array
 *   An array with exactly 3 items: module name, predicate, given version.
 */
function conditional_text_enabled_plugin_parse_expression($expression) {
  // full syntax, eg.: views >= 7.x-3.0
  if (count($expression) >= 3) {
    list($modulename, $predicate, $givenversion) = $expression;
  }
  else { // short syntax, just the module name
    $modulename = head($expression);
    $predicate = '>';
    $givenversion = '0';
  }

  return array($modulename, $predicate, $givenversion);
}

/**
 * Determines a version for a given module in an appropriate format for a
 * given version.
 *
 * @param null|array $context
 *   Context array.
 * @param string $givenversion
 *   Given version. This helps determining whether to use short format
 *   (without core version) or long format.
 * @param string $modulename
 *   Name of the module.
 *
 * @return null|string
 *   Version string.
 */
function conditional_text_enabled_plugin_determine_version($context,
                                                  $givenversion, $modulename) {
  $version = NULL;
  if (is_array($context) && isset($context['modules']['data'])) {
    $given_modules = array();
    array_map(function($item) use(&$given_modules) {
        $moddata = explode('-', $item['module'], 2);
        $name = array_shift($moddata);
        $version = array_shift($moddata);
        $given_modules[$name] = $version ?: DRUPAL_CORE_COMPATIBILITY;
      }, $context['modules']['data']);
    if (isset($given_modules[$modulename])) {
      $version = $given_modules[$modulename];
    }
  }
  if ($version === NULL) {
    if ($modulename == 'drupal') {
      $version = $givenversion == DRUPAL_CORE_COMPATIBILITY ?
        DRUPAL_CORE_COMPATIBILITY : VERSION;
    }
    elseif (module_exists($modulename)) {
      $module_info = system_get_info('module', $modulename);
      // This is an edge case. When a module is not unpacked from a tarball,
      // downloaded from Drupal.org, the version variable is most likely
      // omitted. In this case, the core version is going to be used as a
      // module version.
      if ($module_info['version'] === NULL) {
        $module_info['version'] = $module_info['core'];
      }
      $version = "{$module_info['core']}-{$module_info['version']}";
    }
  }

  return $version;
}

/**
 * Returns a human readable name of a module.
 *
 * @param string $modulename
 *   Machine name of the module.
 *
 * @return string
 *   Human readable module name.
 */
function conditional_text_enabled_plugin_get_module_name($modulename, $context) {
  if ($modulename == 'drupal') {
    return t('Drupal Core');
  }
  else {
    if (isset($context['enabled']['modules']['data'])) {
      foreach ($context['enabled']['modules']['data'] as $data) {
        if (strpos($data['module'], "{$modulename}-") === 0 &&
            drupal_strlen($data['name']) > 0) {
          return $data['name'];
        }
      }
    }

    $module_info = system_get_info('module', $modulename);
    if (isset($module_info['name'])) {
      return $module_info['name'];
    }
    return drupal_ucfirst(str_replace(array('-', '_'), ' ', $modulename));
  }
}

function conditional_text_enabled_plugin_version_compare($l, $r) {
  $notstable = array(
    'alpha' => 0,
    'beta' => 1,
    'rc' => 2,
    'stable' => 3,
  );

  $normalize = function($v) use($notstable) {
    $numx = '([\d]+|x)';
    $num = '([\d]+)';
    $alphabetarc = '(alpha|beta|rc)([\d+])';

    $matches = array();

    if (preg_match("#^{$num}\\.{$numx}-{$num}\\.{$numx}-{$alphabetarc}$#i", $v, $matches)) {
      $matches[5] = isset($notstable[$matches[5]]) ?
        $notstable[$matches[5]] : $notstable['stable'];
      return $matches;
    }
    elseif (preg_match("#^{$num}\\.{$numx}-{$num}\\.{$numx}$#i", $v, $matches)) {
      $matches[] = $notstable['stable'];
      $matches[] = 0;
      return $matches;
    }
    elseif (preg_match("#^{$num}\\.{$numx}$#i", $v, $matches)) {
      $matches[] = $matches[1];
      $matches[] = $matches[2];
      $matches[] = $notstable['stable'];
      $matches[] = 0;
      return $matches;
    }

    return array();
  };

  $l = tail($normalize($l));
  $r = tail($normalize($r));

  while (count($l) || count($r)) {
    $li = array_shift($l);
    $ri = array_shift($r);

    if ($li === 'x') {
      $ri = 'x';
    }
    elseif ($ri === 'x') {
      $li = 'x';
    }

    if ($li != $ri) {
      if ($li > $ri) {
        return 1;
      }
      if ($li < $ri) {
        return -1;
      }
    }
  }

  return 0;
}
