<?php
/**
 * @file.
 */
class CountryListJSON extends MigrateListJSON {
	/**
	 * The default implementation assumes the IDs are top-level array elements,
	 * but the array elements are the data items - we need to look inside them
	 * for the IDs.
	 */
	protected function getIDsFromJSON(array $data) {
		$ids = array ();
		foreach ( $data as $item ) {
			$ids [] = $item ['id'];
		}
		return $ids;
	}
}
class CountryItemJSON extends MigrateItemJSON {
	protected $data = array ();
	public function getItem($id) {
		// We cache the parsed JSON at $this->data.
		if (empty ( $this->data )) {
			$data = $this->loadJSONUrl ( $this->itemUrl );
			if (isset ( $data ) && ! empty ( $data )) {
				// Let's index the array by the ID for easy retrieval.
				foreach ( $data as $item ) {
					$this->data [$item->id] = $item;
				}
			} else {
				// Error-handling here....
			}
		}
		// Return the requested item
		if (isset ( $this->data [$id] )) {
			return $this->data [$id];
		} else {
			return NULL;
		}
	}
}

// and the sweet class that extends the above two helper classes
class CountryOutlines extends Migration {
	public function __construct() {
		$this->description = 'Imports country outliines';
		$this->softDependencies = array('LocationLevel1');
		
		parent::__construct ( MigrateGroup::getInstance ( 'json_migration' ) );
		//$item_url = 'file:///var/www/robotboy/oam1/site1/profiles/openaidmap/modules/custom/iati_import_countryoutlines/data/countryBoundary.geojson';
		$item_url = str_replace('includes', 'data', dirname(__FILE__)) . '/countryBoundary.geojson';
		$fields = array (
				'id' => 'id',
				'level' => 'level',
				'formal_en' => 'formal_en',
				'coordinates' => 'coordinates',
				'geo_type' => 'geo_type',
				'iso_a2' => 'iso_a2',
				'continent'=>'continent',
				 
		);
		$this->source = new MigrateSourceList ( new CountryListJSON ( $item_url ), new CountryItemJSON ( $item_url, array () ), $fields );
		$this->destination = new MigrateDestinationTerm ( 'iati_admin_boundaries' );
		// map for the migration
		$this->map = new MigrateSQLMap ( $this->machineName, array (
				'id' => array (
						'type' => 'varchar',
						'length' => 16,
						'not null' => true,
						'description' => 'id',
						'alias' => 'id' 
				) 
		), MigrateDestinationTerm::getKeySchema () );
		
		$this->addFieldMapping ( 'name', 'formal_en' );
		$this->addFieldMapping ( 'field_level', 'level' );
		$this->addFieldMapping ( 'field_iati_code', 'iso_a2' );
		$geo_arguments = array(
				'wkt' => array('source_field' => 'coordinates'),
				'geo_type' => array('source_field', 'geo_type'),
				'input_format' => array('source_field' => 'input_format'),
		);
		// The geometry type should be passed in as the primary value.
		$this->addFieldMapping('field_boundaries', 'geo_type')->arguments($geo_arguments);
	}
	public function prepareRow($row) {
		$row->geo_type = drupal_strtolower($row->geometry->type);
		$row->input_format = 'wkt';
		$row->coordinates = $this->setcoordinates($row->geometry);
		$row->iso_a2 = $row->properties->iso_a2;
		//drupal_set_message($row->properties->iso_a2);
	}
	public function prepare($entity, $source_row) {
		// $fred = 1;
		$entity->name = $source_row->properties->name;
		$entity->field_level ['LANGUAGE_NONE'] [0] ['value'] = $source_row->properties->level;
		
		//havre marie
		$outlinemaptable = str_replace('countryoutlines', 'locationlevel1', $this->getMap()->getMapTable());
		$outlines = db_select($outlinemaptable, 'mt')
		->fields('mt', array('destid1'))
		->condition('sourceid1', $source_row->iso_a2)
		->execute()
		->fetchAssoc();
		if ($outlines === FALSE) {
			drupal_set_message('Country does not exist, it will be created.');
		}
		else {
		// Don'create a new location but overwrite the existing one.
		$entity->is_new = FALSE;
		$entity->tid = $outlines['destid1'];
		
		}
		
	}
	public function setcoordinates($geometry){
	  $wkt = $geometry->type . ' (';
	  if (isset($geometry->coordinates) && !empty($geometry->coordinates)) {
	    foreach ($geometry->coordinates as $polygon) {
	      $wkt .= '( ';
	      if ($geometry->type == 'Polygon') {
            if (isset($polygon) && !empty($polygon)) {
	          foreach ($polygon as $point) {
	      		$wkt .= $point[0] . ' ' . $point[1] . ', ';
	      	  }
	        }
	      }
	      else {
	      	if (isset($polygon) && !empty($polygon)) {
	      		$wkt .= '( ';
	      		foreach ($polygon as $points) {
	      		  foreach ($points as $point) {
	      		    $wkt .= $point[0] . ' ' . $point[1] . ', ';
	        	  }
	      		}
	            $wkt = substr($wkt, 0, strlen($wkt) - 2);
	      		$wkt .= '), ';
	      	}
	      }
	      $wkt = substr($wkt, 0, strlen($wkt) - 2);
	      $wkt .= '), ';
	  	}
	  }
	  $wkt = substr($wkt, 0, strlen($wkt) - 2);
	  $wkt .= ')';
	 // drupal_set_message($wkt);
	  return $wkt;
	}
}