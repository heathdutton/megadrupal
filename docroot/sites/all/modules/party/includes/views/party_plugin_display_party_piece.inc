<?php
/**
 * @file Views plugin for party pieces.
 */

class party_plugin_display_party_piece extends views_plugin_display {
  function option_definition() {
    $options = parent::option_definition();

    // @todo: we can't provide a usable default here because we don't have
    // access to the view or this display's ID (WTF?).
    // Hence add validation to prevent the view being saved with this default
    // unchanged.
    $options['party_piece_path'] = array('default' => '');
    $options['party_piece_label'] = array('default' => 'Views');

    // This is not set in the UI, but here for documentation.
    $options['data_set'] = array('default' => '');

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['party_piece'] = array(
      'title' => t('Party component'),
      'column' => 'second',
      'build' => array(
        '#weight' => -10,
      ),
    );

    $options['party_piece_path'] = array(
      'category' => 'party_piece',
      'title' => t('Party component path'),
      'value' => 'party/%id/' . $this->get_option('party_piece_path'),
    );
    $options['party_piece_label'] = array(
      'category' => 'party_piece',
      'title' => t('Party component label'),
      'value' => $this->get_option('party_piece_label'),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'party_piece_path':
        $form['#title'] .= t('Piece path');
        $form['party_piece_path'] = array(
          '#type' => 'textfield',
          '#required' => TRUE,
          '#title' => t("The path at which to show this component"),
          '#field_prefix' => 'party/%id/',
          '#default_value' => $this->get_option('party_piece_path'),
        );
        break;
      case 'party_piece_label':
        $form['#title'] .= t('Piece label');
        $form['party_piece_label'] = array(
          '#type' => 'textfield',
          '#required' => TRUE,
          '#title' => t("The label for this component's tab"),
          '#default_value' => $this->get_option('party_piece_label'),
        );
        break;
    }
  }

  function options_validate(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_validate($form, $form_state);

    // Only allow word characters and hyphens.
    // TODO: make this match accented characters.
    if (preg_match('/[^\w-]/', $form_state['values']['party_piece_path'])) {
      form_error($form['party_piece_path'], t('The path may only contain letters, numbers, underscores, and hyphens.'));
    }

    // Check for duplicate paths.
    $pieces = party_get_party_piece_info();
    if (isset($pieces[$form_state['values']['party_piece_path']])) {
      form_error($form['party_piece_path'], t('The path already exists.'));
    }
  }

  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);

    switch ($form_state['section']) {
      case 'party_piece_path':
        // @todo: validate as proper path-y string. Can core menu help?
        // @todo: validate as not clashing with other paths on the party.
        $this->set_option('party_piece_path', $form_state['values']['party_piece_path']);
        break;
      case 'party_piece_label':
        $this->set_option('party_piece_label', $form_state['values']['party_piece_label']);
        break;
    }
  }

  /**
   * metacribbed from eva. may need work.
   * @todo this doesn't even seem to run!?!?!
   */
  function execute() {
    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    $this->view->build();

    // And now render the view.
    return $this->view->render();
  }

  /**
   * Returns items for hook_party_party_pieces().
   *
   * This is the same pattern as the block display plugin uses. It's neat ;)
   */
  function execute_hook_party_party_pieces() {
    $key = $this->get_option('party_piece_path');

    // Build a piece to return.
    // Note that access settings are handled by party_party_party_pieces().
    $pieces = array(
      $key => array(
        'title' => $this->get_option('party_piece_label'),
        'page callback' => 'party_page_view_piece_views',
        // We need to be able to find this view in the page callback.
        'page arguments' => array(1, $this->view->name, $this->display->id),
        'file' => 'party.pages.inc',
        // Add a key to say we came from a dataset. This is then handled by
        // party_party_party_pieces() which in turn enables local tasks for
        // adding entities within this set in party_menu_local_tasks_alter().
        'data_set' => $this->options['data_set'],
      ),
    );

    return $pieces;
  }
}
