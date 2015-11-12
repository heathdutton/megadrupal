<?php

class SearchApiSolrReplicationService extends SearchApiSolrService {
  public function configurationForm(array $form, array &$form_state) {
    $options = $this->options + array(
      'primary' => array(),
      'secondary' => array(),
    );

    // Get a list of Search API Solr servers.
    $servers = array();
    $conditions = array('class' => 'search_api_solr_service', 'enabled' => TRUE);
    foreach (search_api_server_load_multiple(FALSE, $conditions) as $server) {
      $servers[$server->identifier()] = $server->label();
    }

    $form['primary'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Primary solr servers'),
      '#description' => t('Queries that cause changes to the index will be sent to these servers.'),
      '#default_value' => $options['primary'],
      '#required' => TRUE,
      '#options' => $servers,
    );
    $form['secondary'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Secondary solr servers'),
      '#description' => t('Queries that just read data will be sent to these servers.'),
      '#default_value' => $options['secondary'],
      '#required' => TRUE,
      '#options' => $servers,
    );

    return $form;
  }


  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
    $values += array_filter($values['primary']);
    $values += array_filter($values['secondary']);
    if (!empty($this->options)) {
      $values += $this->options;
    }
    $this->options = $values;
  }


  protected function connect() {
    if (!$this->solr) {
      if (!class_exists('Apache_Solr_Service')) {
        throw new Exception(t('SolrPhpClient library not found! Please follow the instructions in search_api_solr/INSTALL.txt for installing the Solr search module.'));
      }
      $this->solr = new SearchApiSolrReplicationConnection($this->options);
    }
  }

  public function viewSettings() {
    $output = '';
    $options = $this->options;

    $output .= "<dl>\n  <dt>";
    $output .= t('Primary servers');
    $output .= "</dt>\n  <dd>";

    $server_links = array();
    foreach ($options['primary'] as $server_id) {
      if ($server = search_api_server_load($server_id)) {
        $uri = entity_uri('search_api_server', $server);
        $uri += array('options' => array());
        $server_links[] = l($server->label(), $uri['path'], $uri['options']);
      }
    }
    $output .= implode(', ', $server_links);
    $output .= '</dd>';

    $output .= '<dt>';
    $output .= t('Secondary servers');
    $output .= "</dt>\n  <dd>";
    $server_links = array();
    foreach ($options['secondary'] as $server_id) {
      if ($server = search_api_server_load($server_id)) {
        $uri = entity_uri('search_api_server', $server);
        $uri += array('options' => array());
        $server_links[] = l($server->label(), $uri['path'], $uri['options']);
      }
    }
    $output .= implode(', ', $server_links);
    $output .= '</dd>';

    $output .= "\n</dl>";

    return $output;
  }

}