<?php

# Copyright 2009 Max Pozdeev

class DatabaseResult_Drupal
{
	var $parent;
	var $q;
	var $query;
	var $rows = NULL;
	var $prefix = '';

	function __construct($query, &$h, $resultless = 0)
	{
		$this->parent = $h;
		$this->query = $query;

		$this->result = db_query($query);
	}

	function affected()
	{
		return $this->rows();
	}

	function fetch_row()
	{
		if ($row = $this->result->fetchAssoc())
			return array_values($row);
		else
			return null;
	}

	function fetch_assoc()
	{
		return $this->result->fetchAssoc();
	}

	function rows()
	{
		if (is_null($this->rows))
			$this->rows = $this->result->rowCount();

		return $this->rows;
	}
}

class Database_Drupal
{
	var $dbh;
	var $error_str;

	function __construct()
	{
	}

	function sq($query, $p = NULL)
	{
		$q = $this->_dq($query, $p);

		if($q->rows()) $res = $q->fetch_row();
		else return NULL;

		if(sizeof($res) > 1) return $res;
		else return $res[0];
	}

	function sqa($query, $p = NULL)
	{
		$q = $this->_dq($query, $p);

		if($q->rows()) $res = $q->fetch_assoc();
		else return NULL;

		if(sizeof($res) > 1) return $res;
		else return $res[0];
	}

	function dq($query, $p = NULL)
	{
		return $this->_dq($query, $p);
	}

	function ex($query, $p = NULL)
	{
		return $this->_dq($query, $p, 1);
	}

	private function _dq($query, $p = NULL, $resultless = 0)
	{
		if(!isset($p)) $p = array();
		elseif(!is_array($p)) $p = array($p);

		$m = explode('?', $query);

		if(sizeof($p)>0)
		{
			if(sizeof($m)< sizeof($p)+1) {
				throw new Exception("params to set MORE than query params");
			}
			if(sizeof($m)> sizeof($p)+1) {
				throw new Exception("params to set LESS than query params");
			}
			$query = "";
			for($i=0; $i<sizeof($m)-1; $i++) {
				$query .= $m[$i]. (is_null($p[$i]) ? 'NULL' : $this->quote($p[$i]));
			}
			$query .= $m[$i];
		}
		return new DatabaseResult_Drupal($query, $this, $resultless);
	}

	function quote($s)
	{
		return '\''. addslashes($s). '\'';
	}

	function quoteForLike($format, $s)
	{
		$s = str_replace(array('%','_'), array('\%','\_'), addslashes($s));
		return '\''. sprintf($format, $s). '\'';
	}

	function table_exists($table)
	{
		$table = addslashes($table);
		return db_query("SELECT 1 FROM :table WHERE 1=0", array(':table' => $table))->rowCount();
	}
}

?>
