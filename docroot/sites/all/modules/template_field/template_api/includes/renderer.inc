<?php

/**
 * @file
 * renderer.inc
 *
 * The interface and abstract class for the renderer plugin
 */

/**
 * The interface for the renderer plugin
 */
interface TemplateApiRendererInterface {
  /**
   * Render the template
   *
   * @param Template
   * @param array $data the data to render in the template
   * @return array a render array
   */
  public function render(Template $template, $data);

  /**
   * Verify anything before rendering
   * @param Template
   * @param array $data the data to render in the template
   * @return boolean
   */
  public function valid(Template $template, $data);
}

/**
 * Abstract class for renderer plugin
 */
abstract class TemplateApiRenderer extends TemplateApiPlugin implements TemplateApiRendererInterface {
  /**
   * Verify anything before rendering
   * @param Template
   * @param array $data the data to render in the template
   * @return boolean
   */
  public function valid(Template $template, $data) {
    return TRUE;
  }
}

/**
 * The controller for the renderer
 */
class TemplateApiRendererController extends TemplateApiPluginController {
  /**
   * The plugin type
   */
  protected function pluginType() {
    return 'renderer';
  }

  /**
   * View the renderer
   */
  public function render(Template $template, $data) {
    if ($this->plugin->valid($template, $data)) {
      $return = $this->plugin->render($template, $data);
      if (!empty($template->attached)) {
        $return['#attached'] = array();
        if (!empty($template->attached['js'])) {
          $return['#attached']['js'] = array(
            $this->attach('js', $template) => array(
              'type' => $this->getAttachmentIncludeType('js', $template),
              'group' => JS_DEFAULT,
              'scope' => 'footer',
            ),
          );
        }
        if (!empty($template->attached['css'])) {
          $return['#attached']['css'] = array(
            $this->attach('css', $template) => array(
              'type' => $this->getAttachmentIncludeType('css', $template),
              'group' => CSS_DEFAULT,
            )
          );
        }
        if(!empty($template->attached['libraries'])) {
          foreach ($template->attached['libraries'] as $library) {
            if (is_array($library)) {
              $return['#attached']['library'][] = array(
                $library['module'], $library['name'],
              );
            }
          }
        }
      }
    }
    else {
      watchdog('template_api', t('Field failed to render'));
      $return = array();
    }
    return $return;
  }

  protected function attach($type, Template $template) {
    $return = '';
    if (!empty($template->attached[$type])) {
      $return = $template->attached[$type];
    }

    return $return;
  }

  protected function getAttachmentIncludeType($type, Template $template) {
    $include_type = 'inline';
    if (is_file($template->attached[$type])) {
      $include_type = 'file';
    }
    return $include_type;
  }

  /**
   * Load a file
   */
  protected function getFileContents($path) {
    $path = drupal_realpath($path);
    if (file_exists($path)) {
      return file_get_contents($path);
    }
    return '';
  }
}
