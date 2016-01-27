<?php

/**
 *
 * Title : HTML Output for Php Quick Profiler
 * Author : Created by Ryan Campbell
 * URL : http://particletree.com/features/php-quick-profiler/
 *
 */

function theme_pqp($vars) {
  
  $logCount = count($vars['logs']['console']);
  $fileCount = count($vars['files']);
  $memoryUsed = $vars['memoryTotals']['used'];
  $queryCount = $vars['queryTotals']['count'];
  $speedTotal = $vars['speedTotals']['total'];
  
  $output .= '
  <div id="pQp" class="console">
  
  <div id="pqp-console" class="pqp-box">';
  
  $output .= '
    <div class="side">
    	<div class="alt1"><var>'.$vars['logs']['logCount'].'</var><h4>Logs</h4></div>
    	<div class="alt2"><var>'.$vars['logs']['errorCount'].'</var> <h4>Errors</h4></div>
    	<div class="alt3"><var>'.$vars['logs']['memoryCount'].'</var> <h4>Memory</h4></div>
    	<div class="alt4"><var>'.$vars['logs']['speedCount'].'</var> <h4>Speed</h4></div>
    </div>
    <div class="main">';
    
    $class = '';
    foreach($vars['logs']['console'] as $log) {
    	$output .= '
    	<div class="log-'.$log['type'].'">
    		<div class="type">'.$log['type'].'</div>
    		<div class="row'.$class.'">';
      if($log['type'] == 'log') {        
      	$output .= '<div>'.$log['data'].'</div>';
      }
    	elseif($log['type'] == 'memory') {
    		$output .= '<div><pre>'.$log['data'].'</pre> <em>'.$log['dataType'].'</em>: '.$log['name'].' </div>';
    	}
    	elseif($log['type'] == 'speed') {
    		$output .= '<div><pre>'.$log['data'].'</pre> <em>'.$log['name'].'</em></div>';
    	}
    	elseif($log['type'] == 'error') {
    		$output .= '<div><em>Line '.$log['line'].'</em> : <pre>'.$log['data'].'</pre> <pre>'.$log['file'].'</pre></div>';
    	}
    
    	$output .= '
        </div>
      </div>';
			if($class == '') $class = ' alt';
			else $class = '';
		}
			
		$output .= '</div>';
  
  $output .= '</div>';
  
  $output .= '<div id="pqp-speed" class="pqp-box">';
  
  if($vars['logs']['speedCount'] ==  0) {
  	$output .= '<h3>This panel has no log items.</h3>';
  }
  else {
  	$output .= '<div class="side">
  		  <div class="row"><var>'.$vars['speedTotals']['total'].'</var><h4>Load Time</h4></div>
  		  <div class="row alt"><var>'.$vars['speedTotals']['allowed'].'</var> <h4>Max Execution Time</h4></div>
  		 </div>
  		<div class="main">';
  		
  		$class = '';
  		foreach($vars['logs']['console'] as $log) {
  			if($log['type'] == 'speed') {
  				$output .= '
  				  <div class="log-'.$log['type'].'">
  				    <div class="row'.$class.'">';
  				$output .= '<div><pre>'.$log['data'].'</pre> <em>'.$log['name'].'</em></div>';
  				$output .= '
  				    </div>
            </div>';
  				if($class == '') $class = ' alt';
  				else $class = '';
  			}
  		}
  			
  		$output .= '</div>';
  }
  
  $output .= '</div>';
  
  $output .= '<div id="pqp-queries" class="pqp-box">';
  
  if($vars['queryTotals']['count'] ==  0) {
  	$output .= '<h3>This panel has no log items.</h3>';
  }
  else {
  	$output .= '<div class="side" cellspacing="0">
  		  <div class="row"><var>'.$vars['queryTotals']['count'].'</var><h4>Total Queries</h4></div>
  		  <div class="row alt"><var>'.$vars['queryTotals']['time'].'</var> <h4>Total Time</h4></div>
  		  <div class="row duplicates"><var>'.$vars['queryTotals']['duplicate_count'].'</var> <h4>Duplicates</h4></div>
  		 </div>
  		<div class="main">';
  		
  		$class = '';

      if ($vars['queryTotals']['duplicate_count'] > 0) {
        $output .= '
    		  <div class="log-query duplicates">
    		    <div class="row alt noquery"><h4>Duplicates</h4></div>
    		  </div>';
    		foreach($vars['queryTotals']['duplicates'] as $query) {
    			$output .= '
            <div class="log-query duplicates">
      				<div class="row'.$class.'">
      				  <div>'.$query.'</div>
    			    </div>
    			  </div>';
    			if($class == '') $class = ' alt';
    			else $class = '';
    		}
    		// Separator
    		$output .= '<div class="log-query duplicates"><div class="row noquery">--- End duplicates</div></div>';
      }

  		foreach($vars['queries'] as $query) {
  			$output .= '
          <div class="log-query">
    				<div class="row'.$class.'">
    				  <b>'.$query['time'].'</b><div>'.$query['sql'].'</div>
  			    </div>
  			  </div>';
  			if($class == '') $class = ' alt';
  			else $class = '';
  		}
  			
  		$output .= '</div>';
  }
  
  $output .= '</div>';
  
  $output .= '<div id="pqp-memory" class="pqp-box">';
  
  if($vars['logs']['memoryCount'] ==  0) {
  	$output .= '<h3>This panel has no log items.</h3>';
  }
  else {
  	$output .= '<div class="side">
  		  <div class="row"><var>'.$vars['memoryTotals']['used'].'</var><h4>Used Memory</h4></div>
  		  <div class="row alt"><var>'.$vars['memoryTotals']['total'].'</var> <h4>Total Available</h4></div>
  		 </div>
  		<div class="main">';
  		
  		$class = '';
  		foreach($vars['logs']['console'] as $log) {
  			if($log['type'] == 'memory') {
  				$output .= '<div class="log-'.$log['type'].'">';
  				$output .= '<div class="row'.$class.'"><b>'.$log['data'].'</b> <em>'.$log['dataType'].'</em>: '.$log['name'].'</div>';
  				$output .= '</div>';
  				if($class == '') $class = ' alt';
  				else $class = '';
  			}
  		}
  			
  		$output .= '</div>';
  }
  
  $output .= '</div>';
  
  $output .= '<div id="pqp-files" class="pqp-box">';

  if($vars['fileTotals']['count'] ==  0) {
  	$output .= '<h3>This panel has no log items.</h3>';
  }
  else {
  	$output .= '
      <div class="side">
        <div class="row"><var>'.$vars['fileTotals']['count'].'</var><h4>Total Files</h4></div>
      	<div class="alt"><var>'.$vars['fileTotals']['size'].'</var> <h4>Total Size</h4></div>
      	<div class="row"><var>'.$vars['fileTotals']['largest'].'</var> <h4>Largest</h4></div>
       </div>
      <div class="main">
        <div class="log-files">';
  		
  		$class ='';
  		foreach($vars['files'] as $file) {
  			$output .= '<div class="row'.$class.'"><b>'.$file['size'].'</b> <pre>'.$file['name'].'</pre></div>';
  			if($class == '') $class = ' alt';
  			else $class = '';
  		}
  			
  		$output .= '
  		  </div>
		  </div>';
  }
  
  $output .= '</div>';
  
  $output .= '
  	<div id="pqp-footer">
			<div class="credit">
				<a href="http://particletree.com" target="_blank">
				<strong>PHP</strong> 
				<b class="green">Q</b><b class="blue">u</b><b class="purple">i</b><b class="orange">c</b><b class="red">k</b>
				Profiler</a>
		  </div>
      <div id="pqp-metrics">
      	<div id="pqp_console" class="green metric-tab">
      		<var>'.$logCount.'</var>
      		<h4>Console</h4>
      	</div>
      	<div id="pqp_speed" class="blue metric-tab">
      		<var>'.$speedTotal.'</var>
      		<h4>Load Time</h4>
      	</div>
      	<div id="pqp_queries" class="purple metric-tab">
      		<var>'.$queryCount.' Queries</var>
      		<h4>Database</h4>
      	</div>
      	<div id="pqp_memory" class="orange metric-tab">
      		<var>'.$memoryUsed.'</var>
      		<h4>Memory Used</h4>
      	</div>
      	<div id="pqp_files" class="red metric-tab">
      		<var>'.$fileCount.' Files</var>
      		<h4>Included</h4>
      	</div>
      </div>
			<div class="actions">
				<a id="pqp_toggleDetails" href="#">Details</a>
				<a id="pqp_toggleHeight"  href="#" class="heightToggle">Height</a>
			</div>
      <div class="pqp_clear">&nbsp;</div>
  	</div>';
  		
  $output .= '</div>';

  return $output;

}
