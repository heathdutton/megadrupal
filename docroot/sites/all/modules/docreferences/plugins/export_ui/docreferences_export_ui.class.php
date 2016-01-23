<?php
/**
 * CTools export UI extending class. Slightly customized for Context.
 *
 * Based on: context/context_ui/export_ui/context_export_ui.class.php.
 */
class docreferences_export_ui extends ctools_export_ui {
  /*
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    $form['top row']['submit'] = $form['bottom row']['submit'];
    $form['top row']['reset'] = $form['bottom row']['reset'];
    $form['bottom row']['#access'] = FALSE;
    // Invalidate the context cache.
    context_invalidate_cache();
    return;
  }
  // */

  function list_css() {
    ctools_add_css('export-ui-list');
    drupal_add_css(drupal_get_path("module", "docreferences") ."/docreferences.css");
  }

  function list_render(&$form_state) {
    $table = array(
      'header' => $this->list_table_header(),
      'rows' => $this->rows, 
      'attributes' => array(
        'class' => array('docreferences-admin'),
        'id' => 'ctools-export-ui-list-items',
      ),
    );
    return theme('table', $table);
  }

  function list_build_row($item, &$form_state, $operations) {
    $name = $item->name;
    // Add a row for packages.
    $package = !empty($item->package) ? $item->package : t('< Unpackageged >');
    if (!isset($this->rows[$package])) {
      $this->rows[$package]['data'] = array();
      $this->rows[$package]['data'][] = array('data' => check_plain($package), 'colspan' => 3, 'class' => array('package'));
      $this->sorts["{$package}"] = $package;
    }

    // Build row for each context item.
    $this->rows["{$package}:{$name}"]['data'] = array();
    $this->rows["{$package}:{$name}"]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');
    $this->rows["{$package}:{$name}"]['data'][] = array(
      'data' => check_plain($name) . "<div class='description'>" . check_plain($item->description) . "</div>",
      'class' => array('ctools-export-ui-name')
    );
    $this->rows["{$package}:{$name}"]['data'][] = array(
      'data' => check_plain($item->type),
      'class' => array('ctools-export-ui-storage')
    );
    $this->rows["{$package}:{$name}"]['data'][] = array(
      'data' => theme('links', array(
        'links' => $operations,
        'attributes' => array('class' => array('links inline'))
      )),
      'class' => array('ctools-export-ui-operations'),
    );

    // Sort by package, name.
    $this->sorts["{$package}:{$name}"] = $package . $name;
    /*
    // Add a row for packages (feature sets, "packages" in context module).
    $package = !empty($item->package) ? $item->package : t('< No Package / Feature Set >');
    if (!isset($this->rows[$package])) {
      $this->rows[$package]['data'] = array();
      $this->rows[$package]['data'][] = array('data' => check_plain($package), 'colspan' => 3, 'class' => array('package');
      $this->sorts["{$package}"] = $package;
    }

    // Build row for each context item.
    $this->rows["{$package}:{$name}"]['data'] = array();
    $this->rows["{$package}:{$name}"]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');
    $this->rows["{$package}:{$name}"]['data'][] = array(
      'data' => check_plain($name) . "<div class='description'>" . check_plain($item->description) . "</div>",
      'class' => array('ctools-export-ui-name')
    );
    $this->rows["{$package}:{$name}"]['data'][] = array(
      'data' => check_plain($item->type),
      'class' => array('ctools-export-ui-storage')
    );
    $this->rows["{$package}:{$name}"]['data'][] = array(
      'data' => theme('links', array(
        'links' => $operations,
        'attributes' => array('class' => array('links inline'))
      )),
      'class' => array('ctools-export-ui-operations'),
    );

    // Sort by package, name.
    $this->sorts["{$package}:{$name}"] = $package . $name;
// */
    // */
  }
  
  /**
   * Override of edit_form_submit().
   * Don't copy values from $form_state['values'].
   */
  /*
  function edit_form_submit(&$form, &$form_state) {
    if (!empty($this->plugin['form']['submit'])) {
      $this->plugin['form']['submit']($form, $form_state);
    }
  }
  // */
}


