<?php
/**
 *  PLINK CORE ENGINE
 *  Author: Shea McKinney - sherakama
 *  This class controls and stores all the layout data for your pages
 *  Layout Processing is done in this class and the singleton can be 
 *  accessed in your themes by calling $plink = Plink::singleton();
 **/

// drupal_add_css(
//   drupal_get_path('theme','MYTHEME')."/css/screen.css",
//   array(
//     'media' => '',
//     'preprocess' => TRUE,
//   )
//   );

class Plink
{

		// VARIABLES
		// ----------------------------------------------------------------------------------------
    private static $instance; // ME!
		
		// Master Layout width
		private $page_width = 960;
		
		// Master Layout Grid option
		private $grid_types = array('px' => 'fixed', '%' => 'fluid');
		
		// ignore these regions in region processing
		private $special_regions = array(
			'content_first', 'content', 'content_last', 
			'secondary', 'tertiary', 
			'help', 'highlighted', 'page_title_prefix', 'page_title_suffix'
		);
		
		// storage for the region classes;
    private $region_classes = array('primary' => array(), 'secondary' => array(), 'tertiary' => array()); 
		
		// Prefix and suffix blocks
		private $title_blocks = array(); 
		
		// storage for the current themes settings
		public $theme_settings = array(); 
		private $theme_data = array();
		
		// Whether or not we have run the setup
		private $processed = FALSE;
		
		// The page loads vars from preprocess
		private $vars = array();

		// Storage information for blocks
		private $blocks = array();
		private $blocks_increment = 0;
		private $blocks_region_count = array();

    // Media Queries - Storage for media queries
    private $media_queries = array();
    


    // CONSTRUCTOR
		// ----------------------------------------------------------------------------------------
    private function __construct() 
    {
        // yay lets go!
				global $theme; // current theme
				$this->theme_data = $theme_data = list_themes();
				$default_settings = ($theme && isset($theme_data[$theme]->info['settings'])) ? $theme_data[$theme]->info['settings'] : array();
				
			  // The current settings for this theme
				$saved_settings = variable_get('theme_' . $theme . '_settings', array());
  			$this->theme_settings = array_merge($default_settings,$saved_settings);
				
				// Alter the theme settings if the plinko companion module is around.
				// We change the settings here before anything is processed and this
				// way we can change the page layout for any number of conditions.
				
				// TODO: write a context reaction for this 
				// If someone could write a context reaction that changes page settings I would heart you.
				
				if(module_exists('plinko')) {
				  
				  // Path based settings
					$plinko_settings = plinko_get_path_theme_settings($theme);
										
					if(is_array($plinko_settings)) {
					 $this->theme_settings = array_merge($this->theme_settings, $plinko_settings); 
					}
					
				}
				
		    // Set the master page width
				$this->page_width = $this->theme_settings['main_grid_width'];
		  
		    // Set the available media queries to off by default
		    $this->media_queries['mq1'] = 0;
		    $this->media_queries['mq2'] = 0;
		    $this->media_queries['mq3'] = 0;
			    
    }



    // THE SINGLETON METHOD
		// ----------------------------------------------------------------------------------------
    public static function singleton() 
    {
        if (!isset(self::$instance)) {
            $c = __CLASS__;
            self::$instance = new $c;
        }

        return self::$instance;
    }
    
    // Prevent users from cloning the instance
    // ----------------------------------------------------------------------------------------
		public function __clone()
    {
        trigger_error('Clone is not allowed.', E_USER_ERROR);
    }
				
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		
		public function init_processes(&$vars) {
			global $theme;
			$this->processed = TRUE;
			$this->set_vars($vars);
			
      if (isset($vars['page']['page_title_prefix']) && isset($vars['page']['page_title_suffix'])) {
        $this->set_title_blocks($vars['page']['page_title_prefix'],$vars['page']['page_title_suffix']);
      }
			
			$this->tally_region_block_counts();
				
			// do not process sidebars on overlay page.			
			if(module_exists('overlay') && isset($_REQUEST['render']) && $_REQUEST['render'] == 'overlay') { return; }
			
      // Init Master Layout
			$this->init_grid_layouts();
			
      // Media Query Layouts
			$this->init_media_query_based_layouts();
			
		}
		
		
		
		// PUBLIC METHODS
		// ----------------------------------------------------------------------------------------
		
		/**
		 * Returns an object with all theme settings 
		 * 
		 * @return the theme setting values
		 */
		public function theme_settings() {	  
		  return (object) $this->theme_settings;
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		
		public function is_processed() {
			return $this->processed;
		}

		/**
		 * NEEDS DOCUMENTATION
		 **/
    public function is_grid() {
			return $this->is_grid;
		}		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		
		private function set_vars($vars) {
			$this->vars = $vars;
		}

		/**
		 * NEEDS DOCUMENTATION
		 **/
    public function set_title_blocks($prefix = null,$suffix = null)
    {
        if(!is_null($prefix)){ $this->title_blocks['prefix'] = $prefix; }
				if(!is_null($suffix)){ $this->title_blocks['suffix'] = $suffix; }
    }

		/** 
		 * NEEDS DOCUMENTATION
		 **/
		
		public function get_vars() {
			return $this->vars;
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		
		public function get_title_blocks() {
			return $this->title_blocks;
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		public function get_body_classes() {
			$vars = $this->get_vars();
			$ret = array();
						
			// Check for media queries
			$ret[] = ($this->theme_settings['enable_responsive_mq1']) ? 'mq1' : '';
			$ret[] = ($this->theme_settings['enable_responsive_mq2']) ? 'mq2' : '';
			$ret[] = ($this->theme_settings['enable_responsive_mq3']) ? 'mq3' : '';			
						
			return $ret;
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		public function get_main_region_classes() {		  
			return 'container-' . $this->theme_settings['main_grid_columns'];
		}
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		
		public function get_media_query_region_classes() {
		  $ts = $this->theme_settings;
		  $ret = array();
		  
		  $i = 1;
      while(isset($ts['enable_responsive_mq'.$i])) {
        // Check to see if enabled
        if(!$ts['enable_responsive_mq'.$i]) { $i++; continue; }
        $ret[$ts['mq'.$i.'_grid_columns']] = 'container-' . $ts['mq'.$i.'_grid_columns'];
        $i++;
      }
      
      return implode($ret, ' ');
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		public function get_content_region_classes($id = 'primary') {			
			return implode($this->region_classes[$id],' ');
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		public function get_region_classes($region = null) {
			
			// Handle missing param
			if(is_null($region)) { trigger_error('No region name provided', E_USER_ERROR); }
			
			// handle ignored regions
			if(in_array($region,$this->special_regions)){ return ''; }
			
			// shortcut var
			$ts = $this->theme_settings;

			// check for full width region
			if(!isset($ts['full_width_regions'][$region]) || is_numeric($ts['full_width_regions'][$region])) {
				// not a full width region
				return $this->get_grid_containers();
			}		
			
			
			// full width region or one that doesnt exist
			return '';
			
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		public function get_region_inner_classes($region = null) {
			
			// Handle missing param
			if(is_null($region)) { trigger_error('No region name provided', E_USER_ERROR); }
			
			// handle ignored regions
			if(in_array($region,$this->special_regions)){ return ''; }
			
			// shortcut var
			$ts = $this->theme_settings;

			// check for full width region
			if(!isset($ts['full_width_regions'][$region]) || is_numeric($ts['full_width_regions'][$region])) {
				// not a full width region
				return $this->get_inner_container_grids();
			}			
			
			// full width region or one that doesnt exist
			return $this->get_grid_containers();
			
		}
		
		/**
		 * Returns grid widths for the inner wrapper 
		 **/
		
		private function get_inner_container_grids() {
		  $ts = $this->theme_settings; // shortcut var
		  $ar[] = 'grid-' . $ts['main_grid_columns'];
		  
		  $i = 1;
      while(isset($ts['enable_responsive_mq'.$i])) {
        
        // Check to see if enabled
        if(!$ts['enable_responsive_mq'.$i]) { $i++; continue; }
        
            $ar[$ts["mq".$i."_grid_columns"]] = "mq".$i."-grid-" . $ts["mq".$i."_grid_columns"];
        $i++;
      }
		  
		  
		  return implode($ar, ' ');
		}
		
		
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		
		public function get_block_classes($info) {
			$vars = $this->get_vars();
			$block = $info['block'];
			$ret = array();
			
			$this->blocks[$block->region][] = array('bid' => $block->bid, 'title' => $block->title);
			$region_count = count($this->blocks[$block->region]);
			$region_total = $this->get_region_block_counts($block->region);
			
			if($region_count == 1) { $ret[] = 'first'; };
			if($region_count == $region_total) { $ret[] = 'last'; }
			
			$ret[] = $info['block_zebra'];
			$ret[] = drupal_clean_css_identifier(strtolower($block->title));					
															
			$this->blocks_increment++;
			return $ret;
		}

		/**
		 * NEEDS DOCUMENTATION
		 **/
		private function get_region_block_counts($region = null) {
			
			if(is_null($region)) { return $this->blocks_region_count; }
			return $this->blocks_region_count[$region];
			
		}
		
    
		// PROCESS METHODS
		// ----------------------------------------------------------------------------------------
		
		/**
		 * Returns the active grid containers 
		 * 
		 * 
		 **/
		
		function get_grid_containers() {
		  $ts = $this->theme_settings; // shortcut var
		  $ar[$ts['main_grid_columns']] = 'container-' . $ts['main_grid_columns'];
		  
		  $i = 1;
      while(isset($ts['enable_responsive_mq'.$i])) {
        
        // Check to see if enabled
        if(!$ts['enable_responsive_mq'.$i]) { $i++; continue; }
        
            $ar[$ts["mq".$i."_grid_columns"]] = "container-" . $ts["mq".$i."_grid_columns"];
        $i++;
      }
		  
		  
		  return implode($ar," ");
		  // 'container-' . $ts['main_grid_columns']
		}
		
		
		
		
		/**
		 * NEEDS DOCUMENTATION
		 * Process Block Counts
		 **/
		
		private function tally_region_block_counts() {
			global $theme;
			$vars = $this->get_vars();
			$ignore = array("#sorted",'#theme_wrappers','#region');
			
			foreach($this->theme_data[$theme]->info['regions'] as $k => $v){
				if(isset($vars['page'][$k])) {
					foreach($vars['page'][$k] as $key => $value) {
						if(!in_array($key,$ignore)){
							if(!isset($this->blocks_region_count[$k])) { $this->blocks_region_count[$k] = 0; }
							$this->blocks_region_count[$k]++;
						}
					}
				}
			}	
		}
		
		
		
		/**
		 * NEEDS DOCUMENTATION
		 **/
		private function init_grid_layouts() {
			$vars = $this->get_vars();
			$ts = $this->theme_settings; // shortcut var
			
			// Create and add in the appropriate stylesheets
      $width = $ts['main_grid_width'];
      $columns = $ts['main_grid_columns'];
      $gutter = $ts['main_grid_gutter'];
      $min = $ts['main_screen_min'];
      $max = $ts['main_screen_max'];
      
      $css = $this->generate_mq_css($width, $columns, $gutter, $min, $max);

			// setup the classes
			$this->process_content_region_classes();
		}
		
		
		/**
		 * NEEDS DOCUMENTATION
		 * @param $class_prefix - string - goes before the push/pull/grid classes
		 **/
		public function process_content_region_classes($class_prefix = '', $media_query = null) {
						
			// the page variable			
			$vars = $this->get_vars();
			$page = $vars['page'];
					
			$ts = $this->theme_settings;    // shortcut var	
			$available_grids;               // The total amount of grids available to this layout
			$grid_unit;                     // The width of a grid in px
			$page_width;                    // Current Page width
			$layout;                        // Current Layout to process
			$secondary_grids;               // # of grids width for the secondary column
	    $tertiary_grids;                // same as above
	
	    // SOME DEFAULT CLASSES
      $this->region_classes['secondary'][] = 'clearfix';
      $this->region_classes['tertiary'][] = 'clearfix';
      
	
			if(!is_null($media_query)) {
			  
			  $page_width = $ts['mq'.$media_query.'_grid_width'];
			  $layout = $ts['responsive_layouts_mq'.$media_query];
			  $secondary_grids = $ts['mq'.$media_query.'_secondary_width'];
  			$tertiary_grids = $ts['mq'.$media_query.'_tertiary_width'];
  	    $grid_unit = $page_width / $ts['mq'.$media_query.'_grid_columns'];
  			$available_grids = $ts['mq'.$media_query.'_grid_columns'];
  			
			} else {
			  
			  $page_width = $this->page_width;
			  $layout = $this->theme_settings['main_layout'];
			  $secondary_grids = $ts['main_secondary_width'];
  			$tertiary_grids = $ts['main_tertiary_width'];
  	    $grid_unit = $page_width / $this->theme_settings['main_grid_columns'];
  			$available_grids = $this->theme_settings['main_grid_columns'];
  			
			}
					
			
			


			switch($layout) {
				
				// Layout 1
				// -----------------------------------------------------------------------------------
				case "1" :          

          // SET THE SIDEBARS
          $this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;

          // Calculate whats available and assign it to the primary
          $available_grids -= $secondary_grids + $tertiary_grids; 
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					
					// PUSH AND PULL INTO PLACE
					$this->region_classes['primary'][] = $class_prefix . 'push-' . $secondary_grids;
					$this->region_classes['secondary'][] = $class_prefix . 'pull-' . $available_grids;
					
				break;
				
				// Layout 2
				// -----------------------------------------------------------------------------------
				case "2" ;
						$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;	
						$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
						
            $available_grids -= $secondary_grids + $tertiary_grids; 
  					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
  					
  					// PUSH AND PULL INTO PLACE
						$this->region_classes['secondary'][] = $class_prefix . 'push-' . $tertiary_grids;
						$this->region_classes['tertiary'][] = $class_prefix . 'pull-' . ($available_grids + $secondary_grids);
						$this->region_classes['primary'][] = $class_prefix . 'push-' . $tertiary_grids;
					
				break;
				
				// Layout 3
				// -----------------------------------------------------------------------------------
				case "3" ;
						$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
						$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
						
            $available_grids -= $secondary_grids + $tertiary_grids; 
					  $this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
				break;
				
				// Layout 4
				// -----------------------------------------------------------------------------------
				case "4" ;
						$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
						$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
            
            $available_grids -= $secondary_grids + $tertiary_grids; 
					  $this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;

						// PUSH AND PULL INTO PLACE
  					$this->region_classes['secondary'][] = $class_prefix . 'push-' . $tertiary_grids;
						$this->region_classes['tertiary'][] = $class_prefix . 'pull-' . $secondary_grids;
					
				break;
				
				// Layout 5
				// -----------------------------------------------------------------------------------
				case "5" ;
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
          
          $available_grids -= $secondary_grids + $tertiary_grids; 
				  $this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					
					// PUSH AND PULL INTO PLACE
					$this->region_classes['primary'][] = $class_prefix . 'push-' . ($secondary_grids + $tertiary_grids);
					$this->region_classes['secondary'][] = $class_prefix . 'pull-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'pull-' . $available_grids;
					
				break;
				
				// Layout 6
				// -----------------------------------------------------------------------------------
				case "6" ;
  				$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
  				$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
        
          $available_grids -= $secondary_grids + $tertiary_grids; 
  			  $this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;

					// PUSH AND PULL INTO PLACE
					$this->region_classes['primary'][] = $class_prefix . 'push-' . ($secondary_grids + $tertiary_grids);
				  $this->region_classes['secondary'][] = $class_prefix . 'pull-' . ($available_grids - $tertiary_grids);
					$this->region_classes['tertiary'][] = $class_prefix . 'pull-' . ($available_grids + $secondary_grids);

				break;
				
				// Layout 7
				// -----------------------------------------------------------------------------------
				case "7" ;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'pclear';
				
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$available_grids -= $secondary_grids; 
				
					// add in the rest of the available grids to the primary region
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					
				break;
				
				// Layout 8
				// -----------------------------------------------------------------------------------
				case "8" ;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'pclear';
		
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$available_grids -= $secondary_grids; 
					$this->region_classes['secondary'][] = $class_prefix . 'pull-' . $available_grids;
					$this->region_classes['primary'][] = $class_prefix . 'push-' . $secondary_grids;
		
					// add in the rest of the available grids to the primary region
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
				break;
				
				// Layout 9
				// -----------------------------------------------------------------------------------
				case "9" ;
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
					
					$this->region_classes['primary'][] = $class_prefix . 'pclear';
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					
				break;
				
				// Layout 10
				// -----------------------------------------------------------------------------------
				case "10" ;
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$this->region_classes['secondary'][] = $class_prefix . 'push-' . $tertiary_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $tertiary_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'pull-' . $secondary_grids;
				
					$this->region_classes['primary'][] = $class_prefix . 'pclear';
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
				break;
				
				// Layout 11
				// -----------------------------------------------------------------------------------
				case "11" ;
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$available_grids -= $secondary_grids; 
		
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'pclear';
				break;
				
				// Layout 12
				// -----------------------------------------------------------------------------------
				case "12" ;
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $secondary_grids;
					$available_grids -= $secondary_grids; 
					$this->region_classes['secondary'][] = $class_prefix . 'pull-' . $available_grids;
						
					$this->region_classes['primary'][] = $class_prefix . 'push-' . $secondary_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'push-' . $secondary_grids;						
					
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $available_grids;
				break;

				// Layout 13
				// -----------------------------------------------------------------------------------				
				case "13" ;
				
				  // Everything full width
					$this->region_classes['secondary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['primary'][] = $class_prefix . 'grid-' . $available_grids;
					$this->region_classes['tertiary'][] = $class_prefix . 'grid-' . $available_grids;						
					
				break;
				
			}

		}
		
// ////////////////////////////////////////////////////////////////////////////////////////////////
// MEDIA QUERY BASED FUNCTIONS
// // ////////////////////////////////////////////////////////////////////////////////////////////////

  /**
  * NEEDS DOCUMENTATION
  */

    private function init_media_query_based_layouts() {
      
      $vars = $this->get_vars();
			$ts = $this->theme_settings; // shortcut var
			
      $i = 1;
      while(isset($ts['enable_responsive_mq'.$i])) {
        
        // Check to see if enabled
        if(!$ts['enable_responsive_mq'.$i]) { $i++; continue; }
        
        // Create and add in the appropriate stylesheets
        $width = $ts['mq'.$i.'_grid_width'];
        $columns = $ts['mq'.$i.'_grid_columns'];
        $gutter = $ts['mq'.$i.'_grid_gutter'];
        $min = $ts['mq'.$i.'_screen_min'];
        $max = $ts['mq'.$i.'_screen_max'];

        $css = $this->generate_mq_css($width, $columns, $gutter, $min, $max, 'mq'.$i."-");

  			// setup the classes
  			$this->process_content_region_classes('mq'.$i."-",$i);
        
        $i++;
      }
      
      
    }
    

    /* 
      Generates, creates, and saves a grid css file to the files directory
      Then attaches the css file 
    */
    
    private function generate_mq_css($width, $cols, $gutter, $min, $max, $grid_prefix = '') {
      
      // Gen CSS
      $css = $this->generate_grid_css($width, $cols, $gutter, $min, $max, $grid_prefix);
      
      // Create file
      $css_file = $this->generate_css_file($css);
      
      // Add the css file to the page
      $min = ($min) ? " and (min-width: " . $min. "px)" : "";
      $max = ($max) ? " and (max-width: " . $max. "px)" : "";
      
      drupal_add_css($css_file, array(
        'group' => CSS_THEME,
//      'every_page' => TRUE,
//       'weight' => 100,
        // 'media' => 'screen' . $min . $max,
        'preprocess' => TRUE,
      ));
            
    }
    
    /** 
    * Generates grid css markup 
    **/
  
    private function generate_grid_css($width, $cols, $gutter, $min, $max, $grid_prefix = '') {
      $css = "";
      // $strength = (!empty($grid_prefix)) ? "body " : "";
      
      $min = ($min) ? " and (min-width : " . $min. "px)" : "";
      $max = ($max) ? " and (max-width : " . $max. "px)" : "";
      
      $css .= '@media screen' . $min . $max . " {";

      // Default cover alls
      $css .= ".block { width: 100%; margin: 0px; display: inline; float: left; } \r\n";
      $css .= ".pclear { clear: none; }";

      $css .= ".container-" . $cols . " { margin-left: auto; margin-right: auto; width: " . $width . "px; } \r\n";
      $css .= ".container-" . $cols . " .alpha { margin-left: 0px; }";
      $css .= ".container-" . $cols . " .omega { margin-right: 0px; }";
      $css .= ".container-" . $cols . " .grid-0 { display: none; }";
      $css .= ".container-" . $cols . " ." . $grid_prefix . "pclear { clear: both; }";
            
      $grids = array();
      $pushs = array();
      $pulls = array();
      
      $i = 1;
      while($i <= $cols) {
        $grids[] = "." . $grid_prefix . "grid-".$i;
        $pushs[] = "." . $grid_prefix . "push-".$i;
        $pulls[] = "." . $grid_prefix . "pull-".$i;
        $i++;
      }
      
      // Grid class gutters
      $css .= implode($grids,",");
      $css .= "{ display: inline; float:left; margin-left: " . $gutter . "px; margin-right: " . $gutter . "px; }\r\n";

      // push and pulls
      $css .= implode($pulls, ",");
      $css .= ", " . implode($pushs, ",");
      $css .= "{ position: relative; }\r\n";

      // grid sizes plus push and pulls prefix and suffix
      $k=0;
      while($k <= $cols) {
        $unit = (($width / $cols) * $k) - (2 * $gutter);
        $fx = (($width / $cols) * $k);
        
        // grid
        $css .= ".container-" . $cols . " ." . $grid_prefix . "grid-" . $k . "{ width: " . $unit . "px; }\r\n";
        
        // prefix
        $css .= ".container-" . $cols . " ." . $grid_prefix . "prefix-" . $k . " { padding-left: " . $fx . "px; }\r\n";
        
        // suffix
        $css .= ".container-" . $cols . " ." . $grid_prefix . "suffix-" . $k . " { padding-right: " . $fx . "px; }\r\n";
        
        // push
        $css .= ".container-" . $cols . " ." . $grid_prefix . "push-" . $k . " { left: " . $fx . "px; }\r\n";
        
        // pull
        $css .= ".container-" . $cols . " ." . $grid_prefix . "pull-" . $k . " { left: -" . $fx . "px; }\r\n";
      
        $k++;
      }
      
      $css .= "}";

      return $css;
    }
    
	  /**
	   * Generates and saves a css file
	   **/
	
	  private function generate_css_file($css) {
	    	    
	    $md5 = md5($css); // unique key
	    $dir = 'public://plink';
	    $destination = file_default_scheme() . '://plink/' . $md5 . ".css";			    
	    	    
	    // first check to see if the file exists
	    if(file_exists($destination)) { return $destination; }
	    
	    // File does not exist. Create it.
	    file_prepare_directory($dir,FILE_CREATE_DIRECTORY);
	    return file_unmanaged_save_data($css, $destination, FILE_EXISTS_ERROR);		  
	  }
	

}
