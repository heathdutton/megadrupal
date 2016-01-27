<?php
/**
 * @file
 * Nodeflow classes.
 */

namespace Nodeflow {
  class NodeflowQueue extends \ArrayIterator {
    protected $locked = array();
    protected $unlocked = array();
    protected $no_position = array();
    
    
    public function __construct() {
      // do nothing
    }
    
    /**
     * Insert new nodeflow record into a nodeflow queue.  Record will be inserted into the correct
     * position.
     * 
     * @param \Nodeflow\Db\Nodeflow $q
     * @return boolean
     * @see build();
     */
    public function insert(\Nodeflow\Db\Nodeflow $q) {
      
      // If no position is defined (future_position row), then the row can be added to the end of the array.
      // It's actual position is not relevant to the ordering of the main queue.
      $pos = $q->position;
      if (!is_int($pos)) {
        array_push($this->no_position, $q);
        return TRUE;
      }
 
      // Store the unlocked and locked nodes in different variables for sorting later
      $type = $q->locked ? 'locked' : 'unlocked';
      
      // Row doesn't exist yet, so create it.
      if (empty($this->{$type}[$pos])) {
        
        // Create a holder element so that array_splice() will maintain our keys/order and insert our new row
        // into the correct place.  These will be removed later in build().
        if ($type == 'locked') {
          $this->unlocked[$pos] = array();
        }
        
        // This will also replace any empty holder elements in unlocked if they are receiving a new row.
        $this->{$type}[$pos] = $q;

      } else {
        // Row does exist so we will splice it into the correct place (and thus moving everything else down a position).
        
        // First we need to check if the position we are moving into is a locked position.
        // If we are moving into a locked position, then that nodeflow row moves into the next available position and loses its lock.
        if ($this->{$type}[$pos]->locked) {
          $moving = $this->{$type}[$pos];
          $moving->locked = 0;
          
          $this->{$type}[$pos] = $q;
          $q = $moving;
          $type = 'unlocked';
        }
        
        array_splice($this->{$type}, $pos, 0, array($q));
      }
      
      return TRUE;
    }
    
    /**
     * Find nodeflow queue record and position in the queue.
     * 
     * @param int node ID
     * @return array
     */
    protected function find($nid) {
      foreach (array('locked', 'unlocked', 'no_position') as $var) {
        foreach ($this->{$var} as $key => $element) {
          if ($element->nid == $nid) {
            return array($var, $key);
          }
        }
      }
      return FALSE;
    }
    
    /**
     * Remove element from the nodeflow queue by nid.
     * 
     * @param int node ID
     */
    public function remove($nid) {
      $row = $this->find($nid);
      if ($row) {
        unset($this->{$row[0]}[$row[1]]);
      }
    }
    
    /**
     * Move a row from the "future_publish" queue into the main queue.
     * If the row being moved into is locked, then that row moves into the next available position and loses its lock.
     * The locked position is not maintained because anything moving into a locked position via this method is wanted.
     * 
     * @param \Nodeflow\Db\Nodeflow $q
     */
    public function moveIntoQueue(\Nodeflow\Db\Nodeflow $q) {
          
      $q->position = $q->future_position;
      $q->future_position = NULL;
      
      // Check if requested position is a locked position.
      if (!empty($this->locked[$q->position])) {
        // Copy this row to be reinserted into the next available position
        $to_insert = $this->locked[$q->position];
        // Replace with the moved row
        $this->locked[$q->position] = $q;
        // Remove the lock
        $to_insert->locked = 0;
      } else {
        $to_insert = $q;
      }
      
      // Remove from no_position stack
      foreach ($this->no_position as $key => $element) {
        if ($to_insert->nid == $element->nid) {
          unset($this->no_position[$key]);
          break;
        }
      }

      $this->insert($to_insert);
    }
    
    /**
     * Build the nodeflow queue in the correct order.
     * 
     * @TODO this should be called and generated automatically.
     */
    public function build() {
      // The new combined queue will be based on the available positions within $locked
      $new = $this->locked;
      // Set current position to 0
      $key = 0;
      // Iterate through $unlocked and move each element into the available position in $new
      foreach ($this->unlocked as $element) {
        // Ignore empty elements in the unlocked array (they were added to preserve keys in during array_splice)
        if (empty($element)) {
          continue;
        }
        while (isset($new[$key])) {
          // Position is already taken.
          ++$key;
        }
        
        $element->position = $key;
        $new[$key] = $element;
      }
      // Sort into the correct order
      ksort($new);

      // Key the array by nid
      $new_index = array();
      foreach ($new as $element) {
        $new_index[$element->nid] = $element;
      }
      
      // Add undefined position rows to the end
      foreach ($this->no_position as $element) {
        $new_index[$element->nid] = $element;
      }

      // Pass to ArrayIterator
      parent::__construct($new_index);
    }
    
    /*public function offsetGet($offset) {
      /*if (!$built) {
        $this->build();
      }*
      return parent::offsetGet($offset);
    }*/
  }
}


namespace Nodeflow\Db {
  
  abstract class Record {
    /**
     * Copy of the table row.
     * 
     * @var type array
     */
    private $record = array();
    /**
     * Table column names & types.
     * 
     * @var type array
     */
    protected $keys = array();
    
    
    /**
     * Table primary key column name.
     * 
     * @return string
     */
    abstract public function getPrimaryKey();
    /**
     * Unique key to be used when doing update/inserts.
     * 
     * @return array
     */
    abstract public function getUpdateKey();

    /**
     * Return actual table name.
     * 
     * @return string
     */
    abstract public function getName();
    /**
     * Return column key validation mappings.
     * 
     * @return array
     * @see Validate filters (php.net)
     * @TODO autoread columns and create mappings from the actual table.
     */
    //abstract protected function getColumns();


    public function __set($name, $value) {
      if ($this->keys[$name]) {
        // Allow NULL values.  @TODO update to allow only for position and future_position
        if (NULL === $value) {
          $this->record[$name] = NULL;
        } else {
          $v = filter_var($value, $this->keys[$name]);
          if ($v !== FALSE) {
            $this->record[$name] = $v;
          }
        }
      }
    }

    public function __get($name) {
      if (isset($this->record[$name])) {
        return $this->record[$name];
      }
      return NULL;
    }
    
    /**
     * Save record to database via db_merge.
     * 
     * @returns int last inserted in
     */
    public function save() {
      
      db_transaction();
      $status = db_merge($this->getName())
        ->key($this->getUpdateKey())
        ->fields($this->record)
        ->execute();
      
      switch ($status) {
        // Fetch out last inserted ID
        case 1:
          $pk = $this->getPrimaryKey();
          $id = db_select($this->getName(), 'n')
            ->fields('n', array($pk))
            ->orderby($pk, 'desc')
            ->range(0, 1)
            ->execute()
            ->fetchAssoc();
          
          $insert_id = $id[$pk];
        break;
        default:
          $insert_id = 0;
      }
 
      return $insert_id;
    }
  }

  /**
   * nodeflow
   */
  class Nodeflow extends Record {
    protected $keys = array('nfid' => FILTER_VALIDATE_INT, 'nid' => FILTER_VALIDATE_INT, 'position' => FILTER_VALIDATE_INT, 'locked' => FILTER_VALIDATE_INT, 'future_position' => FILTER_VALIDATE_INT);


    public function __construct($nfid, $nid, $position, $locked = 0, $future_position = NULL) {
      $this->__set('nfid', $nfid);
      $this->__set('nid', $nid);
      $this->__set('position', $position);
      $this->__set('locked', $locked);
      $this->__set('future_position', $future_position);
    }
    
    public function getPrimaryKey() {
      return 'pid';
    }
    
    public function getUpdateKey() {
      return array('nfid' => $this->nfid, 'nid' => $this->nid);
    }

    public function getName() {
      return 'nodeflow';
    }
  }

  /**
   * nodeflow_queues
   */
  class NodeflowQueue extends Record {
    protected $keys = array('nfid' => FILTER_VALIDATE_INT, 'name' => FILTER_SANITIZE_STRING, 'weight' => FILTER_VALIDATE_INT);


    public function __construct($name, $weight) {
      $this->__set('name', $name);
      $this->__set('weight', $weight);
    }
    
    public function getPrimaryKey() {
      return 'nfid';
    }
    
    public function getUpdateKey() {
      $value = !empty($this->nfid) ? $this->nfid : NULL;
      return array('nfid' => $value);
    }

    public function getName() {
      return 'nodeflow_queues';
    }
    
    /*
    public function save() {
      $id = parent::save();
      // $id == 0 === update
      // create a view for each new queue?
      
      return $id;
    }*/
  }
}