<?php
/**
 * @file context_condition_resolution.inc
 * Provides the context CTools plugin base class.
 */

class ContextConditionBreakpoint extends context_condition {

  protected $cookieName = 'context_breakpoints';
  protected $resolutionCookieName = 'context_breakpoint_resolution';

  protected function parseBreakpoint($query) {
    $query = str_replace(' ', '', $query);

    preg_match_all('/\(([a-z\-]+)\:(.+?)\)/', $query, $matches);

    $point = array();
    for ($i = 0; $i < count($matches[0]); $i++) {
      $cmd = $matches[1][$i];
      $value = $matches[2][$i];

      switch ($cmd) {
        case 'width':
        case 'min-width':
        case 'max-width':
        case 'height':
        case 'min-height':
        case 'max-height':
        case 'device-width':
        case 'min-device-width':
        case 'max-device-width':
        case 'device-height':
        case 'min-device-height':
        case 'max-device-height':
          // Only px values are supported.
          if (strpos($value, 'px') !== false) {
            $point[$cmd] = (int) str_replace('px', '', $value);
          }
          break;

        case 'aspect-ratio':
        case 'min-aspect-ratio':
        case 'max-aspect-ratio':
        case 'device-aspect-ratio':
        case 'min-device-aspect-ratio':
        case 'max-device-aspect-ratio':
          $parts = explode('/', $value);

          if (count($parts) === 2 && is_numeric($parts[0]) && is_numeric($parts[1])) {
            $point[$cmd] = (float) $parts[0] / (float) $parts[1];
          }

          break;

        default:
          // Unsupported.
          break;
      }
    }
    return count($point) ? $point : FALSE;
  }

  protected function parseCookie($cookie) {
    $data = explode(',', $cookie);

    return array_filter($data);
  }

  protected function parseResolutionCookie($cookie) {
    $data = array();

    $parts = explode('|', $cookie);

    if (count($parts) !== 2) {
      return FALSE;
    }

    $resParts = explode('x', $parts[0]);
    if (!(count($resParts) === 2
      && is_numeric($resParts[0])
      && is_numeric($resParts[1]))
    ) {
      return FALSE;
    }

    $data['width'] = (int) $resParts[0];
    $data['height'] = (int) $resParts[1];

    $resParts = explode('x', $parts[1]);
    if (!(count($resParts) === 2
      && is_numeric($resParts[0])
      && is_numeric($resParts[1]))
    ) {
      return FALSE;
    }

    $data['device-width'] = (int) $resParts[0];
    $data['device-height'] = (int) $resParts[1];

    return $data;
  }

  protected function isAdminPage() {
    $parts = explode('/', $_GET['q']);

    return (count($parts) && $parts[0] === 'admin');
  }

  public function getJSConfig() {
    $conf = array(
      'settings' => context_breakpoint_get_settings(),
      'contexts' => array(),
      'is_admin' => $this->isAdminPage(),
    );

    $valid = array(
      'width',
      'device-width',
      'min-width',
      'min-device-width',
      'max-width',
      'max-device-width',
      'height',
      'device-height',
      'min-height',
      'min-device-height',
      'max-height',
      'max-device-height',
      'aspect-ratio',
      'device-aspect-ratio',
      'min-aspect-ratio',
      'min-device-aspect-ratio',
      'max-aspect-ratio',
      'max-device-aspect-ratio',
    );

   foreach ($this->get_contexts() as $context) {
      $values = $this->fetch_from_context($context, 'values');
      $options = $this->fetch_from_context($context, 'options');

      if (!count($values)) {
        break;
      }

      $name = $values[0];

      $points = array();

      // Check if it is a group. 
      if (strpos($name, 'group_') === 0) {
        // A group!
        $group_name = substr($name, 6);
        $group = breakpoints_breakpoint_group_load($group_name);

        if (!$group) { break; }
        $points = $group->breakpoints;
      }
      else {
        // Just one point.
        $points[] = $name;
      }

      $point_conditions = array();

      foreach ($points as $point_name) {
        $breakpoint = breakpoints_breakpoint_load_by_fullkey($point_name);
        if (!$breakpoint) continue;

        $conditions = $this->parseBreakpoint($breakpoint->breakpoint);

        $point = array();

        foreach ($conditions as $cmd => $value) {
          if (in_array($cmd, $valid)) {
            $point[$cmd] = $value;
          }
        }

        if (count($point)) {
          $point_conditions[$point_name] = $point;
        }

      }

      if (count($point_conditions)) {
        $options['breakpoints'] = $point_conditions;
        $conf['contexts'][$context->name] = $options;
      }
    }

    return $conf;
  }

  /**
   * Condition values.
   */
  function condition_values() {
    $breakpoints = array();

    foreach (breakpoints_breakpoint_group_load_all() as $group) {

      $valid = true;

      $point_names = array();

      foreach ($group->breakpoints as $breakpoint) {
        $point = breakpoints_breakpoint_load_by_fullkey($breakpoint);
        if (!$this->parseBreakpoint($point->breakpoint)) {
          $valid = false;
          break;
        }
          $point_names[] = $point->name;
      }

      if ($valid) {
        $name = t('Group: ') . $group->name . ' | ' . implode(',', $point_names);
        $breakpoints['group_' . $group->machine_name] = $name;
      }
    }

    foreach (breakpoints_breakpoint_load_all() as $point) {
      if ($this->parseBreakpoint($point->breakpoint)) {
        $breakpoints[$point->machine_name] = $point->name . ' | ' . $point->breakpoint;
      }
    }

    return $breakpoints;
  }

  /**
   * Condition form.
   */
  function condition_form($context) {
    $supported = implode(', ', array(
      '(min/max-)width',
      '(min/max-)height',
      '(min/max-)device-width',
      '(min/max-)device-height',
      '(min/max-)aspect-ratio',
      '(min/max-)device-aspect-ratio',
    ));

    $values = $this->fetch_from_context($context, 'values');
    $bp = ($values && count($values)) ? $values[0] : '';

    return array(
      '#title' => $this->title,
      '#description' => t('Choose the breakpoint or breakpoint group you want to react to. Only compatible breakpoints are displayed (all breakpoints in a group need to be compatible). <br />Supported: ' . $supported . '. "and" queries are supported. <br />' . l('Configure breakpoints', 'admin/config/media/breakpoints')),
      '#options' => $this->condition_values(),
      '#type' => 'select',
      '#default_value' => $bp,
    );
  }

  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    return array($values);
  }

  function options_form($context = NULL) {
    $defaults = $this->fetch_from_context($context, 'options');
    $form = array(
      'autoreload' => array(
        '#type' => 'checkbox',
        '#title' => t('Auto-reload'),
        '#description' => t('When enabled, the page will automatically reload when when this breakpoint becomes active or unactive(due to resizing the browser).<br /> Only works for width, height and aspect-ratio.'),
        '#default_value' => isset($defaults['autoreload']) ? $defaults['autoreload'] : 0,
      ),
      'global' => array(
        '#type' => 'markup',
        '#markup' => '<div>' . l(t('Site-wide Context Breakpoint settings'), 'admin/config/media/context-breakpoint') . ' - ' . t('Includes performance-relevant caching settings.') . '</div>',
      ),
    );

    return $form;
  }

  function execute() {
    $active_contexts = array();
    $resolutionData = array();

     // Check if there is a valid resolution stored in the cookie.
    if (isset($_COOKIE[$this->cookieName])) {
      $active_contexts = $this->parseCookie($_COOKIE[$this->cookieName]);
    }
    if (isset($_COOKIE[$this->resolutionCookieName])) {
      $resolutionData = $this->parseResolutionCookie($_COOKIE[$this->resolutionCookieName]);
    }

    if ($active_contexts) {
      foreach ($this->get_contexts() as $context) {
        if (in_array($context->name, $active_contexts)) {
          $this->condition_met($context);
        }
      }
    }
  }
}
