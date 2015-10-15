<?php
/**
 * @file
 * Structured panel display renderer.
 */

class RestfulPanelsStructuredRenderer extends panels_renderer_standard {

  /**
   * {@inheritdoc}
   */
  public function render_layout() {
    if (empty($this->prep_run)) {
      $this->prepare();
    }
    $this->render_panes();
    $this->render_regions();

    if ($this->admin && !empty($this->plugins['layout']['admin theme'])) {
      $theme = $this->plugins['layout']['admin theme'];
    }
    else {
      $theme = $this->plugins['layout']['theme'];
    }

    return array(
      'prefix' => $this->prefix,
      'layout' => $this->plugins['layout'],
      'content' => $this->rendered['regions'],
      'suffix' => $this->suffix,
    );
  }

  /**
   * {@inheritdoc}
   */
  function render_pane(&$pane) {
    module_invoke_all('panels_pane_prerender', $pane);

    $content = $this->render_pane_content($pane);
    if ($this->display->hide_title == PANELS_TITLE_PANE && !empty($this->display->title_pane) && $this->display->title_pane == $pane->pid) {

      // If the user selected to override the title with nothing, and selected
      // this as the title pane, assume the user actually wanted the original
      // title to bubble up to the top but not actually be used on the pane.
      if (empty($content->title) && !empty($content->original_title)) {
        $this->display->stored_pane_title = $content->original_title;
      }
      else {
        $this->display->stored_pane_title = !empty($content->title) ? $content->title : '';
      }
    }

    if (!empty($content->content)) {
      $return = array(
        'content' => $content,
        'pane' => $pane,
      );
      if (!empty($pane->style['style'])) {
        $style = panels_get_style($pane->style['style']);
        $return['style'] = $style;
      }

      return $return;
    }
  }

  /**
   * {@inheritdoc}
   */
  function render_region($region_id, $panes) {
    $style = $this->prepared['regions'][$region_id]['style'];
    $style_settings = $this->prepared['regions'][$region_id]['style settings'];

    // Retrieve the pid (can be a panel page id, a mini panel id, etc.), this
    // might be used (or even necessary) for some panel display styles.
    // TODO: Got to fix this to use panel page name instead of pid, since pid is
    // no longer guaranteed. This needs an API to be able to set the final id.
    $owner_id = 0;
    if (isset($this->display->owner) && is_object($this->display->owner) && isset($this->display->owner->id)) {
      $owner_id = $this->display->owner->id;
    }

    $output = array(
      'owner_id' => $owner_id,
      'region_id' => $region_id,
      'style' => $style,
      'panes' => $panes,
      'settings' => $style_settings,
    );
    return $output;
  }

}
