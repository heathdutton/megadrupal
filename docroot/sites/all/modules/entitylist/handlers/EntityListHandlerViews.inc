<?php

class EntityListHandlerViews implements EntityListHandler {

  protected $config = array();

  protected $entityType = NULL;

  protected $requiredArguments = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  public function __construct(Array $config = array()) {
    $defaults = array(
      'view' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  /**
   * Helper method to return the view this handler is dealing with. We can't
   * save it in the object, because of wierdness (?) when a list context is
   * passed around.
   *
   * @todo
   *   Fix so that we can store one view object and pass it by reference.
   */
  protected function view() {
    if (!empty($this->config['view'])) {
      list($name, $display) = explode(':', $this->config['view']);
      $view = views_get_view($name);
      if (!empty($view)) {
        $view->set_display($display);
        return $view;
      }
    }
    return FALSE;
  }

  protected function viewOptions() {
    $infos = views_get_applicable_views('returns context');
    $options = array();
    foreach ($infos as $info) {
      $view = $info[0];
      $display_id = $info[1];
      $options[$view->name . ':' . $display_id] = $view->human_name . ' (' . $view->display_handler->display->display_title . ')';
    }
    return $options;
  }

  public function entityType() {
    if ($this->entityType === NULL && $view = $this->view()) {
      $table = views_fetch_data($view->base_table);
      if (!isset($table['table']['entity type'])) {
        throw new EntityListException(t('Could not determine entity type of view @view', array('@view' => $view->name)));
      }
      $this->entityType = $table['table']['entity type'];
    }
    return $this->entityType;
  }

  public function requiredArguments() {
    if (empty($this->requiredArguments) && $view = $this->view()) {
      // Check that this view can handle CTools contexts and argument input.
      $plugin = views_fetch_plugin_data('display', $view->display[$view->current_display]->display_plugin);
      if (empty($plugin['returns context'])) {
        return array();
      }
      // This code is pretty much taken directly from views_content.module.
      $arguments = $view->display_handler->get_argument_input();
      ctools_include('views');
      $contexts = array();
      foreach ($arguments as $argument) {
        if ($argument['type'] == 'context') {
          list($context, $converter) = explode('.', $argument['context'], 2);
          if ($context == 'term' || $context == 'vocabulary') {
            $context = 'entity:taxonomy_' . $context;
          }
          elseif ($entity = entity_get_info($context)) {
            $context = 'entity:' . $context;
          }
          $this->requiredArguments[$context . '.' . $converter] = array(
            'label' => $argument['name'] ? $argument['name'] : '',
            'required' => empty($argument['context_optional']) ? TRUE : FALSE,
          );
        }
      }
    }
    return $this->requiredArguments;
  }

  public function setArguments(Array $arguments = array()) {
    $this->arguments = $arguments;
    return $this;
  }

  public function getArguments() {
    return $this->arguments;
  }

  public function setParameters(Array $parameters = array()) {
    $this->parameters = $parameters;
    return $this;
  }

  public function getParameters() {
    return $this->parameters;
  }

  public function setRange($offset = NULL, $limit = NULL) {
    $this->range = array(
      'offset' => $offset,
      'limit' => $limit,
    );
    return $this;
  }

  public function getRange() {
    return $this->range;
  }

  public function buildQuery() {
    if (!$this->view = $this->view()) {
      return;
    }
    $arguments = $this->getArguments();
    $this->view->set_arguments($arguments);
    $this->view->build();
  }

  public function count() {
    if (empty($this->view)) {
      return 0;
    }
    $this->view->get_total_rows = TRUE;
    $this->view->query->execute($this->view);
    return $this->view->query->pager->get_total_items();
  }

  public function execute() {
    if (empty($this->view)) {
      return array();
    }
    $range = $this->getRange();
    if (!empty($range['offset'])) {
      $this->view->query->offset = $range['offset'];
    }
    if (!empty($range['limit'])) {
      $this->view->query->limit = $range['limit'];
    }
    $this->view->execute();
    $entities = $this->view->query->get_result_entities($this->view->result);

    if (empty($entities[1])) {
      return array();
    }
    return $entities[1];
  }

  /**
   * @todo Set tags with arguments for better granularity.
   */
  public function cacheTags() {
    list($name, $display) = explode(':', $this->config['view']);
    return array('views' => array($name, "$name:$display"));
  }

  public function adminPath() {
    if (!user_access('administer views')) {
      return FALSE;
    }
    list($name, $display) = explode(':', $this->config['view']);
    return 'admin/structure/views/view/' . check_plain($name) . '/edit/' . $display;
  }

  public function configForm(&$form_state) {
    $form = array();
    $form['view'] = array(
      '#type' => 'select',
      '#title' => t('View'),
      '#description' => t('Select which view to use for this handler. Only view display types that returns a context will work.'),
      '#options' => $this->viewOptions(),
      '#default_value' => array($this->config['view']),
    );
    return $form;
  }
}
