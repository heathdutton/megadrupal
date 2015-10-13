<?php
/**
 *  Query Handler Base Class
 */
  class Parser 
  {
// $errorMsg stores error messages if the input doesn't pass validation
  public $errorMsg;
/** Constucts a new Parser object
 *  @param
 *  @return void
 */
  public function __construct() {
      $this->errorMsg;
      $this->validate();
	$this->noSQL();
  }
/** Validator method
 *  @param  
 *  @return  bool 
 */  
  protected function validate() {
  // Empty in Base class
  }
/** Adds an error message
 * @param string
 * @return void
 */   
  protected function setError ($msg) {
      $this->errorMsg = $msg;
  }
  //Returns true if input validates, false if not
  //@return boolean
  public function isValid () {
      if ( !empty($this->errorMsg) ) {
          return FALSE;
      } 
      else {
        return TRUE;
      }
  }
	
	//@param
	//@return void
	protected function noSQL() {
	if ( 
	   (preg_match('/select/i', $_SERVER['QUERY_STRING']) && preg_match('/from/i', $_SERVER['QUERY_STRING']) )
	|| (preg_match('/drop/i', $_SERVER['QUERY_STRING']) && preg_match('/table|index|database/i', $_SERVER['QUERY_STRING']))
	|| (preg_match('/alter/i', $_SERVER['QUERY_STRING']) && preg_match('/table|index|database/i', $_SERVER['QUERY_STRING']))
	){
	  	$this->setError('Query input has SQL-like strings.');
	}
	}
  }
 //ParserAfter subclass of Parser
 //Validates an 'after' term
  class ParseAfter extends Parser
  {
   // $after to validate
    public $after;
    //A constructor. 
	//Constucts a new object
    public function __construct($after) {
		$this->after=$after;
		parent::__construct();
    }
   //Validates an after term 
   //@return void
    protected function validate() {  
    if (!is_int(strtotime($this->after))) {
	    $this->setError('After input string "' . $this->after .'" has the wrong input type or input is empty');
        }
	if (preg_match('/^\d{4}$/', $this->after)) {
	$this->setError('After input string "' . $this->after .'" has only the year digits. Add a month');
	}
    }
   //Construct SQL 
   //@return string	
	public function getSQL() {
	  if ($this->isValid()) {
	  $sql = 'changed > ' . strtotime($this->after);
	  return $sql;
      }
	  else {
    return '';
    }
	}
  }

 //ParserBefore subclass of Parser
 //Validates a 'before' term
  class ParseBefore extends Parser
  {
   
   // $before to validate
    public $before;
    //A constructor. 
	//Constucts a new object
    public function __construct($before) {
		$this->before=$before;
		parent::__construct();
    }
   //Validates an before term 
   //@return void
    protected function validate() {  
    if (!is_int(strtotime($this->before))){
	    $this->setError('Before input string "' . $this->before .'" has the wrong input type or is empty');
        }
	if (preg_match('/^\d{4}$/', $this->before)) {
	$this->setError('Before input string "' . $this->before .'" has only the year digits. Add a month');
	}
    }
   //Construct SQL 
   //@return string	
	public function getSQL() {
	  if ($this->isValid()) {
	  $sql = 'changed < ' . strtotime($this->before);
	  return $sql;
      }
	  else {
    return '';
    }
	}
  }

 //ParserType subclass of Parser
 //Validates a 'type' term
  class ParseType extends Parser
  {
   // $type to validate
    public $type;
    //A constructor. 
	//Constucts a new object
    public function __construct($type) {
		$this->type = $type;
		parent::__construct();
    }
   //Validates an type term
   //@return void
    protected function validate() {
	$content_types = node_type_get_names();  //get all content types as an array of objects with names of types as keys
	if (preg_match('/\s/', $this->type) > 0) {
      $query_types = explode(' ' , $this->type);
	    foreach ($query_types as $key=>$value) {
	      if (!array_key_exists($value, $content_types)) {
	      $this->setError('Type "' . $value . '" does not exist on this site or input is empty');
	} 
	}
	}	
  elseif (!array_key_exists($this->type, $content_types)) {   //check if the passed query type is in the list of types on the site
	  $this->setError('Type "' . $this->type . '" does not exist on this site or input is empty');
    }
    }
   //Construct SQL 
   //@return string	
	public function getSQL() {
	  if ($this->isValid()) {
	    if (preg_match('/\s/', $this->type) > 0) {
          $types = explode(' ' , $this->type);
  		  for ($i = 0; $i < sizeof($types); $i++) {  		  //handle multiple types input        
		   $types[$i] = "'" .  $types[$i] . "'";		
		  }
		  $sql = "type IN (" . implode( ',', $types) . ")";
		  }
		else{	  	  
	    $sql = "type IN ('" .  check_plain($this->type)  . "')";   //handle single type input
	    }
	  return $sql;
      }
	  else {
    return '';
    }
	}
  }

 //ParserLimit subclass of Parser
 //Validates a 'limit' term
  class ParseLimit extends Parser
  {
   // $limit to validate
    public $limit;
    //A constructor. 
	//Constucts a new object
    public function __construct($limit) {
		$this->limit = check_plain($limit);
		parent::__construct();
    }
   //Validates a limit term 
   //@return void
    protected function validate() {  
    if (!is_numeric($this->limit)){
	    $this->setError('Limit input string "' . $this->limit .'" has the wrong input type or input is empty');
        }
    }
   //Construct SQL 
   //@return string	
	public function getLimit(){
	  if ($this->isValid()){
	  return $this->limit;
      }
	  else {return FALSE;}
	}
  }

 //ParserSearch subclass of Parser
 //Validates a 'search' term
  class ParseSearch extends Parser
  {
   // $search to validate
  public $search;
	public $limit;
	public $solr_added_nodes;
    //A constructor. 
	//Constucts a new object
    public function __construct($search) {
		$this->search = $search;
		parent::__construct();
    }
   //Validates a search term 
   //@return void
    protected function validate() {  	
	if (strlen($this->search) > 2) {
      if (module_exists('cs_solr')) {	
      $return_object = cs_solr_slr_search(check_plain($this->search)); //a call to a function in CS Solr module
        if (!$return_object->error) {
          $this->solr_added_nodes = array();
          foreach ($return_object->docs as $docs){
            foreach ($docs as $field => $value){
              if ($field == 'nid') $this->solr_added_nodes[] = $value;
            }
          }
  }
  else{
  $this->setError('Solr Search Engine returned an error: ' . $return_object->error);
  }
  }
  else{
  $this->setError('SC Solr module does not exist or is not enabled.');
  }
  }
  else{
  $this->setError('Search string is too short. Need at least 3 characters.');
  }
  }
  
   //Construct SQL 
   //@return string	
	public function getSQL() {
	  if ($this->isValid()){
	  $sql = ($this->solr_added_nodes) ? ' nid IN (' . implode(',', $this->solr_added_nodes) . ')' : ''; 
	  return $sql;
      }
	  else {
	  return '';
	  }
	}
  }

 //ParserField subclass of Parser
 //Validates a 'field' term
  class ParseField extends Parser
  { 
   // $field to validate
    public $field;
    //A constructor. 
	//Constucts a new object
    public function __construct($field) {
		$this->field = check_plain($field);
		parent::__construct();
    }
	
	
	
   //Validates an field term 
   //@return void
    protected function validate() {
	if (strlen($this->field) < 2) {
	    $this->setError('Field input string "' . $this->field .'" is too short or empty');
        }
    }

  // Construct FIELD 
  // @return string	
	public function getField(){
	  if ($this->isValid()){
		$fields_array = array();
		if (preg_match('/\s/', $this->field) > 0) {
        $fields_array = explode(' ' , $this->field);
		  }
		else{
		$fields_array[] = $this->field;}
		foreach ($fields_array as $value){
		$return_fields[$value] =  $value;
		}	  
	  return $return_fields;
      }
	  else {
	  return array();
	  }
	}
 }

 // ParseSort subclass of Parser
 // Validates a 'sort' term
  class ParseSort extends Parser
  {
  // $sort to validate
    public $sort;
  // A constructor. 
	// Constucts a new object
    public function __construct($sort) {
		$this->sort = check_plain($sort);
		parent::__construct();
    }
  // Validates a sort term 
  // @return void
    protected function validate() {
    if ((preg_match('/\s/', $this->sort) > 0) || !preg_match('/asc|desc/i', $this->sort)){
	    $this->setError('Sort input string "' . $this->sort .'" has the wrong input string or input is empty');
        }
    }
  // Construct SQL 
  // @return string	
	public function getSQL(){
	  if ($this->isValid()){
	  $sql = ' ORDER BY changed ' . $this->sort;
	  return $sql;
      }
	  else {
	  return '';
	  }
	}
  }

 // ParseBatch subclass of Parser
 // Validates a 'batch' term
  class ParseBatch extends Parser
  {
  // $batch to validate
    public $batch;
  // A constructor. 
	// Constucts a new object
    public function __construct($batch) {
		$this->batch = check_plain($batch);
		parent::__construct();
    }
  // Validates an batch term 
  // @return void
    protected function validate() {
    if (!is_numeric($this->batch) || $this->batch <= 0) {
	    $this->setError('Batch from input string "' . $this->batch .'" has the wrong input type or input is empty');
        }
    }
  // Construct SQL 
  // @return string	
	public function getBatch() {
	  if ($this->isValid()){
	  return $this->batch;
      }
	  else {
	  return 1;
	  }
	}
  }

 //ParseSize subclass of Parse
 //Validates a 'size' term
 //@return void
  class ParseSize extends Parser
  {
  // $size to validate
    public $size;
  // A constructor. 
	// Constucts a new object
    public function __construct($size) {
		$this->size = check_plain($size);
		parent::__construct();
    }
  // Validates an size term 
  // @return void
    protected function validate() {
    if ($this->size != 'size'){
	    $this->setError('Size from input string "' . $this->size .'" has the wrong input type or input is empty');
        }
    }
  }
  
 //ParseHelp subclass of Parse
 //Validates a 'help' term
 //@return string
  class ParseHelp extends Parser
  {
  // $help to validate
    public $help;
  // A constructor. 
    public function __construct($help) {
		$this->help = check_plain($help);
		parent::__construct();
    } 
  // Validates an help term 
  // @return void
    protected function validate() {
    if ($this->help != 'help'){
	    $this->setError('Help from input string "' . $this->help .'" has the wrong input type or input is empty');
        }
    }
  // Construct output 
  // @return string	
	public function getHelp() {
	  if ($this->isValid()) {
      $help_array = array();
      $help_array['message'] = 'You are using Resource Share help function.';
      $help_array['URL'] = 'http://drupal.org/sandbox/krylov/1530604';
      global $user;
      $types =  ($user->uid == 0) ? variable_get('content_share_resource_types_anon', array()) : variable_get('resource_share_resource_types_auth', array());
      $fields = ($user->uid == 0) ? variable_get('resource_share_resource_fields_anon', array()) : variable_get('resource_share_resource_fields_auth', array());
      foreach ($types as $key=>$value) {
      if ($key === $value) {
      $help_array['types'][] = $key;
      }
      }
      foreach ($fields as $key=>$value) {
      if ($key === $value) {
      $help_array['fields'][] = $key;
      }
      }
	  return $help_array;
      }
	  else {
	  return FALSE;
	  }
	}
  }
 
 
 //ParseTable subclass of Parse
 //Validates table and other custom table terms
 //@return void
  class ParseTable extends Parser
  {
  // $table to validate
    public $table;
    public $fields;
    public $where;

  // A constructor. 
	// Constucts a new object
    public function __construct($request_array) {
    if (isset($request_array['table']) && 
        isset($request_array['fields'])) {
		  $this->table  = check_plain($request_array['table']);
      $this->fields = check_plain($request_array['fields']);
      if (isset($request_array['where'])){
        $this->where  = $request_array['where'];
      }
    }
    else {
      $this->setError('Custom table query terms are missing');
    }
		parent::__construct();
    }
  // Validates the terms 
  // @return void
    protected function validate() {
      $tables = variable_get('resource_share_tables', array());
      if (empty($this->table) || $tables[$this->table] !== $this->table) {
	      $this->setError('Custom table input is empty or this table is not accessible');
        }
      if (empty($this->fields)){
	      $this->setError('Custom table fields input is empty');
        }
    }

  // Construct SQL 
  // @return string	
	public function getSQL(){
	  if ($this->isValid()){
	    $sql = "SELECT " . $this->fields . 
             " FROM " . $this->table;
      if (isset($this->where)){
	      $sql .= " WHERE " . $this->where;
        }             
	    return $sql;
      }
	  else {
	    return '';
	  }
	}
  }
  
  
//*** ADD CLASSES FOR EACH TERM PARSING HERE
