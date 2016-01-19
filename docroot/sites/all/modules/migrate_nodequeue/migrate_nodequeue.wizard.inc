<?php

/**
 * @file
 * Wizard implementation for nodequeue migrations.
 */

// TODO maybe import strongarm variables.
class MigrateNodequeueWizard extends MigrateD2DWizard {

  /**
   * Array of queues we will migrate.
   */
  protected $source_qids;

  /**
   * Whether we migrate nodes/nodequeues relations.
   * Array of node migrations.
   */
  protected $toggle_nodes;
  protected $migrations_nodes;

  /**
   * Whether we migrate roles.
   * Array of role migrations.
   */
  protected $toggle_roles;
  protected $migrations_roles;

  /**
   * Lay out the steps of the wizard.
   */
  public function __construct() {
    $this->addStep(t('Credentials'), 'sourceDataForm');
    $this->addStep(t('Queues'), 'nodequeueQueues');
    $this->addStep(t('Review'), 'reviewForm');
  }

  /**
   * Identify ourselves.
   *
   * @return string
   */
  public function getSourceName() {
    return t('Nodequeue');
  }

  /**
   * Check if a module exists on source site.
   *
   * @param $module
   *   Module name.
   * @return
   *   TRUE if module exists, FALSE otherwise.
   */
  public function moduleExists($module) {
    return Database::getConnection('default', $this->sourceConnection)
      ->select('system', 's')
      ->fields('s', array('status'))
      ->condition('name', $module)
      ->condition('type', 'module')
      ->execute()
      ->fetchField();
  }

  /**
   * @override
   */
  protected function sourceDataFormValidate(&$form_state) {
    parent::sourceDataFormValidate($form_state);

    // We only support migrations from D6.
    if ($this->sourceVersion != 6) {
      form_set_error('driver', t('Nodequeue migrations are supported only from Drupal 6.'));
    }

    // Nodequeue needs to be enabled on source site.
    if (!$this->moduleExists('nodequeue')) {
      form_set_error('driver', t('Nodequeue must be enabled on the source site.'));
    }
  }

  /**
   * Present information and options around Queues migrations.
   */
  protected function nodequeueQueues(&$form_state) {
    $qids = $this->connection()
      ->select('nodequeue_queue', 'q')
      ->fields('q', array('qid', 'title'))
      ->orderBy('title')
      ->execute()
      ->fetchAllKeyed(0);
    foreach ($qids as $qid => $title) {
      $nodes = $this->connection()
        ->select('nodequeue_nodes', 'n')
        ->condition('qid', $qid)
        ->countQuery()
        ->execute()
        ->fetchField();
      $subqueues = $this->connection()
        ->select('nodequeue_subqueue', 's')
        ->condition('qid', $qid)
        ->countQuery()
        ->execute()
        ->fetchField();
      $qids[$qid] = array(
        'title' => $title,
        'subqueues' => $subqueues,
        'nodes' => $nodes,
      );
    }
    $form['qids'] = array(
      '#type' => 'tableselect',
      '#header' => array('title' => t('Queue Title'), 'subqueues' => t('Number of Subqueues'), 'nodes' => t('Number of Nodes')),
      '#options' => $qids,
      '#empty' => t('No queue found on your Drupal!version site.', array('!version' => $this->sourceVersion)),
    );

    $form['toggle_nodes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Migrate relations between nodes and nodequeues.'),
      '#description' => t('This will create an additional migration for nodes/nodequeues relations. If a node was in a nodequeue in D6, then it will remain in the nodequeue in D7.'),
      '#default_value' => FALSE,
    );
    $form['migrations_nodes'] = array(
      '#title' => t('Node migrations'),
      '#description' => t('If you have no node migration, you may create one with <a href="http://drupal.org/project/migrate_d2d">migrate_d2d_ui</a>.'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $this->getTypedMigrations('DrupalNodeMigration'),
      '#states' => array(
        'invisible' => array(':input[name="toggle_nodes"]' => array('checked' => FALSE)),
      ),
    );
    // TODO migrations_nodes & migrations_roles should be required
    $form['toggle_roles'] = array(
      '#type' => 'checkbox',
      '#title' => t('Migrate roles configuration'),
      '#description' => t('This will migrate the roles configuration, i.e. which roles are allowed to add nodes to the queues.'),
      '#default_value' => FALSE,
    );
    $form['migrations_roles'] = array(
      '#title' => t('Role migrations'),
      '#description' => t('If you have no role migration, you may create one with <a href="http://drupal.org/project/migrate_d2d">migrate_d2d_ui</a>.'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $this->getTypedMigrations('DrupalRoleMigration'),
      '#states' => array(
        'invisible' => array(':input[name="toggle_roles"]' => array('checked' => FALSE)),
      ),
    );

    return $form;
  }

  /**
   * Return a list of available migrations inheriting a given class.
   *
   * @param $superclass
   *   Super class.
   * @return
   *   Array of migrations.
   */
  protected function getTypedMigrations($superclass) {
    $migrations = array();
    foreach (migrate_migrations() as $name => $object) {
      if (is_subclass_of($object, $superclass)) {
        $migrations[$name] = $name;
      }
    }
    return $migrations;
  }

  /**
   * Prepare for registration.
   *
   * @param array $form_state
   */
  protected function nodequeueQueuesValidate(&$form_state) {
    $this->source_qids      = $form_state['values']['qids'];
    $this->toggle_nodes     = $form_state['values']['toggle_nodes'];
    $this->toggle_roles     = $form_state['values']['toggle_roles'];
    $this->migrations_nodes = $form_state['values']['migrations_nodes'];
    $this->migrations_roles = $form_state['values']['migrations_roles'];
  }

  /**
   * Show the user what they've chosen to migrate, and give them one last chance
   * to say yay or nay.
   */
  protected function reviewForm(&$form_state) {
    $this->groupArguments = array(
      'encrypted_arguments' => array('source_database'),
      'source_version' => $this->sourceVersion,
      'source_connection' => $this->groupName . '_legacy',
      'source_database' => $this->database,
      'source_system' => t('Drupal @version Nodequeues', array('@version' => $this->sourceVersion)),
    );

    $form['description'] = array(
      '#prefix' => '<div>',
      '#markup' => t('Please review your migration settings. When you submit this form, a migration job containing a migration task for each type of item to be imported will be created and you will be left at the dashboard.'),
      '#suffix' => '</div>',
    );

    $this->addMigration('Nodequeues', "NodequeueQueueDrupal{$this->sourceVersion}Migration", array(
      'source_qids' => $this->source_qids,
      'migrations_roles' => $this->toggle_roles ? $this->migrations_roles : array(),
    ));
    $this->addMigration('NodequeuesSubqueues', "NodequeueSubqueueDrupal{$this->sourceVersion}Migration", array(
      'source_qids' => $this->source_qids,
      'dependencies' => array('Nodequeues'),
      'migration_queues' => $this->groupName . 'Nodequeues',
    ));

    $imports[] = array(
      '#theme' => 'item_list',
      '#prefix' => t('Queues to be migrated:'),
      '#items' => $this->connection()
        ->select('nodequeue_queue', 'q')
        ->fields('q', array('title'))
        ->condition('qid', $this->source_qids, 'IN')
        ->orderBy('title')
        ->execute()
        ->fetchCol(),
    );

    if ($this->toggle_nodes) {
      $this->addMigration('NodequeuesNodeRelations', "NodequeueNodeDrupal{$this->sourceVersion}Migration", array(
        'source_qids' => $this->source_qids,
        'dependencies' => array('Nodequeues', 'NodequeuesSubqueues'),
        'migration_queues' => $this->groupName . 'Nodequeues',
        'migration_subqueues' => $this->groupName . 'NodequeuesSubqueues',
        'migrations_nodes' => $this->migrations_nodes,
      ));
      $imports[] = array(
        '#theme' => 'item_list',
        '#prefix' => t('Nodes/nodequeues relations will be migrated based on these migrations:'),
        '#items' => $this->migrations_nodes,
      );
    }

    if ($this->toggle_roles) {
      $this->addMigration('NodequeuesRoleConfig', "NodequeueRoleDrupal{$this->sourceVersion}Migration", array(
        'source_qids' => $this->source_qids,
        'dependencies' => array('Nodequeues'),
        'migration_queues' => $this->groupName . 'Nodequeues',
        'migrations_roles' => $this->migrations_roles,
      ));
      $imports[] = array(
        '#theme' => 'item_list',
        '#prefix' => t('Roles configuration will be migrated baed on these migrations:'),
        '#items' => $this->migrations_roles,
      );
    }

    $form['imports'] = array(
      '#theme' => 'item_list',
      '#items' => array_map('drupal_render', $imports),
    );

    return $form;
  }

}

