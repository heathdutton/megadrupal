<?php

class EntityListHandlerNodeQueue implements EntityListHandler {

  protected $config = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  protected $qid = NULL;

  function __construct(Array $config = array()) {
    $defaults = array(
      'name' => '',
      'depth' => 0,
    );
    $this->config = array_merge($defaults, $config);

    if (!empty($this->config['name'])) {
      // Figure out the qid.
      $qid_map = nodequeue_get_qid_map();
      $this->qid = $qid_map[$this->config['name']];
    }
  }

  public function entityType() {
    return 'node';
  }

  public function requiredArguments() {
    return array(
      'entity:taxonomy_term.tid' => array(
        'label' => 'Subqueue reference',
        'required' => FALSE,
      ),
    );
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

  }

  public function count() {
    return FALSE;
  }

  public function execute() {
    $queue_query = db_select('node', 'n')
      ->fields('n', array('nid'));
    $queue_query->condition('n.status', 1);
    $queue_query->join('nodequeue_nodes', 'nqn', 'n.nid = nqn.nid AND nqn.qid = :qid', array(':qid' => $this->qid));

    $arguments = $this->getArguments();
    // Construct subquery to limit nodes on argument input (taxonomy term).
    if (!empty($arguments)) {
      $hierarchy_query = db_select('taxonomy_index', 'ti')
        ->fields('ti', array('nid'));
      // TODO: We could be a bit smarter and skip this first join in some cases.
      $hierarchy_query->leftJoin('taxonomy_term_hierarchy', 'th0', 'th0.tid = ti.tid');
      $hierarchy_or = db_or();
      $hierarchy_or->condition('th0.tid', $arguments[0]);
      for ($i = 1; $i <= $this->config['depth']; $i++) {
        $hierarchy_query->leftJoin('taxonomy_term_hierarchy', "th$i", 'th' . ($i-1) . ".parent = th$i.tid");
        $hierarchy_or->condition("th$i.tid", $arguments[0]);
      }
      $hierarchy_query->condition($hierarchy_or);
      // Add the subquery.
      $queue_query->condition('n.nid', $hierarchy_query, 'IN');
    }

    $results = $queue_query
      ->orderBy('nqn.position', 'ASC')
      ->execute();
    $nids = array();
    foreach ($results as $result) {
      $nids[] = $result->nid;
    }

    return entity_load('node', $nids);
  }

  public function cacheTags() {
    return array('nodequeue' => array($this->config['name']));
  }

  public function adminPath() {
    if (!user_access('maninpulate nodequeues')) {
      return FALSE;
    }

    $arguments = $this->getArguments();
    $parent_term = end(taxonomy_get_parents_all($arguments[0]));
    $subqueue = reset(nodequeue_load_subqueues_by_reference(array($this->qid => $parent_term->tid)));
    if (!empty($subqueue->sqid)) {
      return 'admin/structure/nodequeue/' . $this->qid . '/view/' . $subqueue->sqid;
    }
    else {
      return 'admin/structure/nodequeue/' . $this->qid . '/view';
    }
  }

  public function configForm(&$form_state) {
    $qid_map = nodequeue_get_qid_map();
    $queues = nodequeue_load_queues($qid_map);

    $name_options = array();
    foreach ($queues as $queue) {
      $name_options[$queue->name] = $queue->title;
    }
    $form['name'] = array(
      '#type' => 'select',
      '#title' => t('Nodequeue'),
      '#options' => $name_options,
      '#default_value' => $this->config['name'],
    );

    $depth_options = array();
    for ($i = 0; $i <= 10; $i++) {
      $depth_options[$i] = t("$i");
    }
    $form['depth'] = array(
      '#type' => 'select',
      '#title' => t('Taxonomy queue depth'),
      '#description' => t('The depth that should be included in subqueues. This only applied if the current queue is a taxonomy queue that assumes parent term for hierarchical vocabularies.'),
      '#options' => $depth_options,
      '#default_value' => $this->config['depth'],
    );
    return $form;
  }
}
