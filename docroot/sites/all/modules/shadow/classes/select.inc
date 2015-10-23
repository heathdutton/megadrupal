<?php

class ShadowSelectQuery
{
  protected $query;
  protected $guid;
  protected $description;
  protected $offset;
  protected $limit;
  
  function __construct(SelectQuery $query, $guid, $description) {
    $this->query = $query;
    $this->guid = $guid;
    $this->description = $description;
    
    $this->offset = 0;
    $this->limit = 0;
  }
  
  function execute() {
    // If validation fails, simply return NULL.
    // Note that validation routines in preExecute() may throw exceptions instead.
    if (!$this->query->preExecute()) {
      return NULL;
    }
    
    $sql = (string) $this->query;
    $args = $this->query->getArguments();
    shadow_sql($sql, $args, $this->guid, $this->description);
    
    if ($this->limit) {
      $sql = preg_replace('/LIMIT[^\\\']+$/si', '', $sql);
      $sql = "$sql LIMIT {$this->limit} OFFSET {$this->offset}";
    }
    
    $options = array('target' => 'default');
    
    return Database::getConnection()->query($sql, $args, $options);
  }
  
  function getArguments() {
    return $this->query->getArguments();
  }
  
  function &countQuery() {
    $query = new ShadowCountQuery($this);
    return $query;
  }
  
  function __call($name, $arguments) {
    
  }
  
  function __toString() {
    return (string) $this->query;
  }
  
  function range($offset, $limit) {
    $this->offset = (int) $offset;
    $this->limit = (int) $limit;
  }
}

class ShadowCountQuery
{
  protected $query;
  
  function __construct(ShadowSelectQuery $query) {
    $this->query = $query;
  }
  
  function execute() {
    $sql = (string) $this->query;
    $args = $this->query->getArguments();
    
    $sql = preg_replace('/ORDER BY[^\\\']+$/si', '', $sql);
    $sql = preg_replace('/ORDER BY .*$/si', '', $sql);
    
    if (preg_match('/DISTINCT/si', $sql)) {
      $sql = "SELECT COUNT(*) FROM ($sql)";
    }
    else {
      $sql = preg_replace('/^SELECT.+?FROM/si', 'SELECT COUNT(*) FROM', $sql);
    }
    
    $options = array('target' => 'default');
    
    return Database::getConnection()->query($sql, $args, $options);
  }
  
  function &countQuery() {
    return $this;
  }
  
  function __call($name, $arguments) {
    
  }
  
  function __toString() {
    return (string) $this->query;
  }
}
