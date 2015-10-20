<?php
/**
 * @file
 * Contains the gallery summary style plugin.
 */

/**
 * The style plugin for gallery summaries.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_gallery_summary extends views_plugin_style_summary {
  function option_definition() {
    $options = parent::option_definition();
    $options['gallery_display'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $displays = array();
    foreach ($this->view->display as $id => $display) {
      $displays[$id] = $display->display_title;
    }
    $form['gallery_display'] = array(
      '#type' => 'select',
      '#title' => t('Gallery display'),
      '#options' => $displays,
      '#default_value' => $this->options['gallery_display'],
      '#description' => t('Select the display that will be used to summarize the each gallery.'),
    );
  }

  function pre_render(&$result) {
    // After trying several alternatives, the only that worked was to get the
    // view again with views_get_view setting the argument and executing it
    // again. Not sure if this is the best alternative but I don't see many
    // others cause we need to run a query and the query is what the view does
    // plus we need the render of the output in order to attach it to the
    // summary row.+


    $argument = $this->view->argument[$this->view->build_info['summary_level']];

    foreach ($result as &$row) {
      $arg = $argument->summary_argument($row);
      if ($arg) {
        // Load a view from the views editing cache if within the administrative
        // interface. Otherwise the list of displays may be out of date.
        if (strpos($_GET['q'], 'admin/structure/views') === 0) {
          $view = ctools_object_cache_get('view', $this->view->name, TRUE);
        }
        if (!isset($view)) {
          $view = views_get_view($this->view->name);
        }

        // Clones the view for reseting arguments.
        $view = $view->clone_view();

        // Loads the choosen display.
        $display = $this->options['gallery_display'];

        // This simple flag indicates to the theme layer to make certain
        // adjustments for the embedded views.
        $view->gallery_summary = TRUE;

        // TODO: This should handle multiple arguments.
        //Attach the output of this view to the main item.
        $row->gallery = $view->preview($display, array($arg));
      }
    }
  }
}
