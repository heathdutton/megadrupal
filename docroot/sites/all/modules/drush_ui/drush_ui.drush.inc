<?php

/**
 * Implementation of hook_drush_command().
 */
function drush_ui_drush_command() {
  $items['ui'] = array(
    'callback' => 'drush_ui',
    'description' => 'Start up the Drush ncurses UI.',
  );
  return $items;
}

/**
 * Drush command; Starts up the Drush GUI.
 */
function drush_ui() {
  if (!function_exists('ncurses_init')) {
    drush_print('To use the Drush GUI, you must first install the ncurses PHP extension.');
    return;
  }

  // Setup the nDialog ncurses page.
  //require_once('ndialog.class.php');
  $Dialog = new nDialog();
  $context = array();

  // Setup a user process loop.
  $sequence = array('initiate', 'getcommand', 'getarguments', 'execute', 'confirm');
  $step = 0;
  while ($step >= 0) {
    // Run the next step in the sequence.
    $function = 'drush_gui_' . $sequence[$step];
    $result = $function($Dialog, $context);
    // Go to the next step, depending on the result.
    if ($result) {
      $step++;
    }
    else {
      $step = -1;
    }
    // Loop back to the beginning.
    if ($step >= count($sequence)) {
      $step = 0;
    }
  }
}

/**
 * Drush GUI Callback; Initiate the contexts.
 */
function drush_gui_initiate(&$Dialog, &$context) {
  $context = array();
  $Dialog->set('title_page_header', t(' Drush GUI @version ', array(
    '@version' => '0.1',
  )));
  return TRUE;
}

/**
 * Drush GUI Callback; Retrieve what command to run from the user.
 */
function drush_gui_getcommand(&$Dialog, &$context) {
  $Dialog->mode("menu","18x62");
  $Dialog->set("title_dialog_header", t(' Main Menu '));
  $Dialog->set("text", t('Select which command to run.'));

  // Construct an array of available commands.
  $commands = drush_get_commands();
  $available = array(
    'cache-clear' => TRUE,
    'core-cron' => TRUE,
    'status' => TRUE,
    'pm-download' => TRUE,
    'pm-enable' => TRUE,
  );
  $commands = array_intersect_key($commands, $available);
  $items = array();
  $i = 1;
  foreach ($commands as $command => $info) {
    $items[$i] = $command;
    $Dialog->addMenu($command, '', '', $i);
    $i++;
  }
  $result = $Dialog->stroke();
  $Dialog->distroy();
  if ($result > 1) {
    $context['command'] = $items[$result];
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Drush GUI Callback; Retrieve the arguments for the given command.
 */
function drush_gui_getarguments(&$Dialog, &$context) {
  $name = $context['command'];
  $commands = drush_get_commands();
  $command = $commands[$name];
  $context['arguments'] = array();

  foreach ($command['arguments'] as $argument => $description) {
    $Dialog->mode("inputbox","14x62");
    $Dialog->set("title_dialog_header", $name . ': ' .$argument);
    $Dialog->set("text", "\n$description\n");
    $Dialog->set("length", 50);
    $Dialog->set("default_val", "");
    // Show dialog box & get user input
    $result = $Dialog->stroke();
    $context['arguments'][] = $result;
    // Properly cleanup the screen
    $Dialog->distroy();
  }
  return TRUE;
}

/**
 * Drush GUI Callback; Execute the command.
 */
function drush_gui_execute(&$Dialog, &$context) {
  $Dialog->mode("notice","5x30");
  $Dialog->set("text", t("\nPlease wait while\n'@command' executes...\n", array('@command' => $context['command'])));
  $Dialog->set("title_dialog_header", " Please Wait ");
  // Show dialog box & get user input
  $result = $Dialog->stroke();

  // Execute the command.
  $context['return'] = drush_invoke($context['command'], $context['arguments']);

  // Properly cleanup the screen
  $Dialog->distroy();
  return TRUE;
}

/**
 * Drush GUI Callback; Confirmation form that the command took place.
 */
function drush_gui_confirm(&$Dialog, &$context) {
  $Dialog->mode("msgbox","10x60");
  $Dialog->set("title_dialog_header", " Thank You ");
  if ($context['return']) {
    $Dialog->set("text", t("\nThe command '@command' executed successfully.\n\nPress <enter> to continue.\n", array('@command' => $context['command'])));
  }
  else {
    $Dialog->set("text", t("\nThe command '@command' failed to execute.\n\nPress <enter> to continue.\n", array('@command' => $context['command'])));
  }
  // Show dialog box & get user input
  $result = $Dialog->stroke();
  // Properly cleanup the screen
  $Dialog->distroy();
  return TRUE;
}

/******************** nDialog.class.php v.0.9.4 ****************************
 *   Copyright (C) 2007 by J Randolph Smith                                *
 *   johns@servangle.net                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
class nDialog {
	var $title_page_header;
	var $title_dialog_header;
	var $mode;	// tracks the dialog mode
	var $text;
	var $height, $width;
	var $length;
	var $default_val;
	
	var $_screen_max_width, $_screen_max_height;	// auto-detect current screen size
	
	var $button_list;
	var $button_total;
	var $button_cursor;
	
	var $inputbox_list;
	var $inputbox_total;

	
	var $menu_list;	// array of menu items
	var $menu_total; // menu depth
	var $menu_cursor;
	var $menu_label_width;
	var $menu_desc_width;
	
	var $nwin; // notice window handler
	var $swin; // shadow window handler
	
	var $textbox_char; // text char array
	var $textbox_len; //current length of text
	var $textbox_max; //current length of text
	
	var $_focus_list;
	var $_focus_total;
	var $_focus_cursor;
	
	
	/*****************
	// CONSTRUCTOR
	******************/
	function nDialog() 
		{
		$this->title_page_header = "";
		
		$this->_reset();
		

		}

	function __construct() 
		{
		$this->nDialog();
		}
	

	function _reset()
		{
		$this->mode = "";
		$this->test = "";
		$this->size = "";
		$this->height = 0;
		$this->width = 0;

		
		$this->button_list = NULL;
		$this->button_total = 0;
		$this->button_cursor = 0;
		
		$this->inputbox_list = NULL;
		$this->inputbox_total = 0;

		$this->menu_list = NULL;
		$this->menu_total = 0;
		$this->menu_cursor = 0;
		$this->menu_label_width = 0;
		$this->menu_desc_width = 0;
		$this->nwin = $this->swin = NULL;
		$this->textbox_char = NULL;
		$this->textbox_len = $textbox_max = 0;
		
		$this->_focus_list = NULL;
		$this->_focus_total = 0;
		$this->_focus_cursor = 0;
		}
	
	
	/*****************
	// DESTRUCTOR
	******************/
	function distroy() 
		{
		ncurses_clear();
		ncurses_echo();
		ncurses_refresh();
		ncurses_end();
		}
		

	function __destruct() 
		{
		//$this->distroy();
		}

	// PRIVATE
	function _init_screen()
		{
		ncurses_curs_set(0);
		ncurses_noecho();
		$fullscreen = ncurses_newwin ( 0, 0, 0, 0);
		ncurses_getmaxyx($fullscreen,$this->_screen_max_height,$this->_screen_max_width);
		ncurses_delwin($fullscreen);
		
		ncurses_start_color();
		ncurses_init_pair(1,NCURSES_COLOR_WHITE,NCURSES_COLOR_RED);
		ncurses_init_pair(4,NCURSES_COLOR_BLACK,NCURSES_COLOR_RED);
		ncurses_init_pair(2,NCURSES_COLOR_BLACK,NCURSES_COLOR_WHITE);
		ncurses_init_pair(3,NCURSES_COLOR_WHITE,NCURSES_COLOR_WHITE);
		ncurses_init_pair(5,NCURSES_COLOR_WHITE,NCURSES_COLOR_BLUE);
		ncurses_init_pair(6,NCURSES_COLOR_YELLOW,NCURSES_COLOR_BLUE);
		ncurses_init_pair(7,NCURSES_COLOR_YELLOW,NCURSES_COLOR_WHITE);

		ncurses_color_set(5);
		ncurses_erase();
		ncurses_mvhline ( 0, 0, 0, $this->_screen_max_width );
		ncurses_attron ( NCURSES_A_BOLD );
		ncurses_mvaddstr ( 0, 1, $this->title_page_header );
		ncurses_attroff ( NCURSES_A_BOLD );
		for($y=1;$y<$this->_screen_max_height;$y++)
			{
			ncurses_mvhline ( $y, 0, 32, $this->_screen_max_width );
			}
		ncurses_refresh();
		}
		
	// PRIVATE	
	function _drawDialogBorders(&$win,$y,$x)
		{
		// Draw boarders
		//  ============
		// ||          ||
		// ||          ||
		// ||          ||
		//  ------------
		// ||          ||
		//  ============
		ncurses_wattron ( $win, NCURSES_A_BOLD );
		ncurses_wcolor_set($win,3);
		ncurses_wmove ( $win, 0, 1 ); 		ncurses_whline ( $win, 205, $x-2 );	// top line
		ncurses_wmove ( $win, $y-3, 1 ); 	ncurses_whline ( $win, 0, $x-2 ); 	// middle line
		ncurses_wmove ( $win, 1, 0 ); 		ncurses_wvline ( $win, 186, $y-2 );	// left line
		ncurses_wmove ( $win, 0, 0 ); 		ncurses_waddch ( $win, 201 ); 		// top left
		ncurses_wmove ( $win, $y-1, 0 ); 	ncurses_waddch ( $win, 200 ); 		// bottom left
		ncurses_wmove ( $win, $y-3, 0 ); 	ncurses_waddch ( $win, 199 ); 		// middle left

		ncurses_wcolor_set($win,2);
		ncurses_wmove ( $win, $y-1, 1 ); 	ncurses_whline ( $win, 205, $x-2 ); // bottom line
		ncurses_wmove ( $win, 1 ,$x-1 ); 	ncurses_wvline ( $win, 186, $y-2 );	// right line
		ncurses_wmove ( $win, 0, $x-1 ); 	ncurses_waddch ( $win, 187 ); 		// top right
		ncurses_wmove ( $win, $y-1, $x-1 ); ncurses_waddch ( $win, 188 ); 		// bottom right
		ncurses_wmove ( $win, $y-3, $x-1 ); ncurses_waddch ( $win, 182 ); 		// middle right
		ncurses_wattroff ( $win, NCURSES_A_BOLD );
		
		}
		
		
	// PRIVATE
	// Returns the Category of the current focus cursor	
	function _focusCat()
		{
		// get current focus category
		list($cat,$index) = explode("-",$this->_focus_list[$this->_focus_cursor]);
		return($cat);
		}

	// PRIVATE
	// Returns the Sub-index of the current focus cursor	
	function _focusSubindex()
		{
		// get current focus index
		list($cat,$index) = explode("-",$this->_focus_list[$this->_focus_cursor]);
		return($index);
		}

		
	// PRIVATE
	// Returns the top right x,y coordinates based on desired window size and alignment.
	function _getCoordinates($height,$width,$hoz_just="center",$ver_just="middle")
		{
		$result = array('x' => 0, 'y' => 0);

		switch($hoz_just)
			{
			case"center":
				// Calculate offsets to center window based on window size
				$result['x'] = round(($this->_screen_max_width - $width) / 2);
				break;
				
			case"right":
				$result['x'] = $this->_screen_max_width - $width;
				break;				
				
			case"left":
			default:
				break;
			}

		switch($ver_just)
			{
			case"middle":
				// Calculate offsets to center window based on window size
				$result['y'] = round(($this->_screen_max_height - $height) / 2);
				break;
				
			case"bottom":
				$result['y'] = $this->_screen_max_height - $height;
				break;
				
			case"top":
			default:
				break;
			}

		return($result);
		}
		

	// PRIVATE	
	function _createDialogWindow($cord_y, $cord_x, $height, $width, $shadow = false, $bordertype=1)
		{
	
		$win = ncurses_newwin ( $height, $width, $cord_y, $cord_x );
		
		// shadow (optional)
		if($shadow)
			{
			$this->swin = ncurses_newwin ( $height, $width, $cord_y+1, $cord_x+1 );
			ncurses_wrefresh($this->swin);
			}
		
		// fill window
		ncurses_wcolor_set($win,3);		
		for($row=0;$row<$height;$row++)
			{
			ncurses_wmove ( $win, $row, 0 );
			ncurses_whline ( $win, 32, $width );
			}
			
		// Draw the borders
		if($bordertype == 1)
			{
			$this->_drawDialogBorders($win,$height,$width);
			}
		else
			{
			$this->_drawThinBorders($win,0,0,$height,$width);
			}
			
		// Draw dialog box title -- (optional)
		if($this->title_dialog_header != "")
			{
			$len = strlen($this->title_dialog_header);
			$x = round( ($width - $len) / 2 );
			ncurses_wcolor_set($win,7);
			ncurses_wattron ( $win, NCURSES_A_BOLD );
			ncurses_mvwaddstr( $win, 0, $x, $this->title_dialog_header );
			ncurses_wattroff ( $win, NCURSES_A_BOLD );
			ncurses_wcolor_set($win,2);
			}
		
			
		return($win);
		}
		

	function _drawThinTopLeft(&$win,$y,$x,$height,$width)
		{
		ncurses_wmove ( $win, $y, $x+1 ); 		ncurses_whline ( $win, 196, $width-2 );		// top line
		ncurses_wmove ( $win, $y+1, $x ); 		ncurses_wvline ( $win, 179, $height-2 );	// left line
		ncurses_wmove ( $win, $y, $x ); 		ncurses_waddch ( $win, 218 ); 				// top left
		ncurses_wmove ( $win, $y+$height-1, $x ); 	ncurses_waddch ( $win, 192 ); 				// bottom left
		}
		
	function _drawThinBotRight(&$win,$y,$x,$height,$width)
		{
		ncurses_wmove ( $win, $y+$height-1, $x+1 ); 	ncurses_whline ( $win, 196, $width-2 ); // bottom line
		ncurses_wmove ( $win, $y+1 ,$x+$width-1 ); 	ncurses_wvline ( $win, 179, $height-2 );// right line
		ncurses_wmove ( $win, $y, $x+$width-1 ); 	ncurses_waddch ( $win, 191 ); 				// top right
		ncurses_wmove ( $win, $y+$height-1, $x+$width-1 ); ncurses_waddch ( $win, 217 ); 			// bottom right
		}
		
	// PRIVATE	
	function _drawThinBorders(&$win,$y,$x,$height,$width,$type="out")
		{
		// Draw boarders
		//  ------------
		// |			|
		// |			|
		// |			|
		// |			|
		//  ------------
		ncurses_wattron ( $win, NCURSES_A_BOLD );
		if($type == "out") { $this->_drawThinTopLeft($win,$y,$x,$height,$width); } else { $this->_drawThinBotRight($win,$y,$x,$height,$width); }
	
		ncurses_wcolor_set($win,3);
		if($type == "out") { $this->_drawThinBotRight($win,$y,$x,$height,$width); } else { $this->_drawThinTopLeft($win,$y,$x,$height,$width); }
		ncurses_wattroff ( $win, NCURSES_A_BOLD );	
		}
		

		
	// PRIVATE
	// Defines an Input text box input type.
	function addInputBox($win,$name,$label,$cord_y,$cord_x,$max_length,$init_val)
		{
		$this->inputbox_list[$this->inputbox_total]['win'] = $win;	// resource window
		$this->inputbox_list[$this->inputbox_total]['name'] = $name;
		$this->inputbox_list[$this->inputbox_total]['label'] = $label;
		$this->inputbox_list[$this->inputbox_total]['y'] = $cord_y;
		$this->inputbox_list[$this->inputbox_total]['x'] = $cord_x;
		$this->inputbox_list[$this->inputbox_total]['max_length'] = $max_length;	// max allowed input size

		// set a default value - if given.
		if( $init_val != "" )
			{
			$len = strlen($init_val);
			for($c=0;$c<$len;$c++)
				{
				$char = substr($init_val,$c,1);
				$this->inputbox_list[$this->inputbox_total]['val'][$c] = $char;
				}
			$this->inputbox_list[$this->inputbox_total]['val_cursor'] = $c;
			$this->inputbox_list[$this->inputbox_total]['val_len'] = $c;
			}
		else
			{
			$this->inputbox_list[$this->inputbox_total]['val'] = array();
			$this->inputbox_list[$this->inputbox_total]['val_cursor'] = 0;
			$this->inputbox_list[$this->inputbox_total]['val_len'] = 0;
			}

			
		// add to master focus list
		$this->_focus_list[$this->_focus_total] = "I-" . $this->inputbox_total;
			
		$this->inputbox_total++;
		$this->_focus_total++;		
		}
		
		
	// PRIVATE
	// Displays all input text boxes defined
	function _strokeInputBoxes()
		{
		for($i=0;$i<$this->inputbox_total;$i++)
			{
			$this->_drawInputBox($i);
			}
		}
		
		
		
	// PRIVATE	
	// Displays specified input text box
	function _drawInputBox($index)
		{
		$label_offset = 0;
		
		$win 		= &$this->inputbox_list[$index]['win'];
		$label 		= &$this->inputbox_list[$index]['label'];
		$hotkey 	= &$this->inputbox_list[$index]['hot'];
		$cord_y 	= &$this->inputbox_list[$index]['y'];
		$cord_x 	= &$this->inputbox_list[$index]['x'];
		$max_length = &$this->inputbox_list[$index]['max_length'];

		ncurses_wcolor_set($win,2);
		
		// output the label
		if($label != "")
			{
			ncurses_mvwaddstr( $win, $cord_y, $cord_x, $label );
			$label_offset = strlen($label); // overide offset due to label length
			}

		// draw border for input field
		$this->_drawThinBorders($win,$cord_y-1,($cord_x+$label_offset),3,$max_length+2,"out");
		
		// display box contents
		$this->_drawInputBoxContents($index);

		return;
		}
		
		
	// PRIVATE	
	// Displays specified input text box value contents
	function _drawInputBoxContents($index)
		{		
		$win 		= &$this->inputbox_list[$index]['win'];
		$val 		= &$this->inputbox_list[$index]['val'];
		$cord_y 	= &$this->inputbox_list[$index]['y'];
		$cord_x 	= &$this->inputbox_list[$index]['x'];
		$label 		= &$this->inputbox_list[$index]['label'];
		
		// label offset
		if($label != "")
			{
			$label_offset = strlen($label); // overide offset due to label length
			}

		// output the value
		for($n=0;$n<$this->inputbox_list[$index]['max_length'];$n++)
			{
			// Set content color based on has focus or not.
			if( ( $this->_focusCat() == "I" ) && ( $this->_focusSubindex() == $index ) )
				{
				if( $n == $this->inputbox_list[$index]['val_cursor'] )
					{
					ncurses_wcolor_set($win,2);
					}
				else
					{
					ncurses_wcolor_set($win,5);
					}
				}
			else
				{
				ncurses_wcolor_set($win,2);
				}

			if(isset($val[$n]))	// print chars that exist
				{
				$char = $val[$n];
				}
			else
				{
				$char = " ";
				}
			ncurses_mvwaddstr( $win, $cord_y, ($cord_x+$label_offset+1+$n), $char );
			}
		}


		
	// PRIVATE
	// outputs a paragraph text and returns offset lines taken up.
	function _stroke_para(&$win,$text,$y,$x,$align="center",$wrap=true)
		{
		if($wrap)
			{ $text = wordwrap($text, ($x-2), "|");	}	
		$text = str_replace ( "\n", "|", $text ); // decode linebreaks
		
		$lines = explode("|",$text);
		
		$curs_y = 1;
		foreach($lines as $line)
			{
			$len = strlen($line) + 1;
			if($align == "center")	// text alignment
				{
				$text_offset_x = round( ($x - $len) / 2 );		
				}
			else
				{
				$text_offset_x = 1;
				}
			ncurses_mvwaddstr( $win, $curs_y, $text_offset_x, $line ); 
			$curs_y++;
			}
		return($curs_y - 1); 	// return lines taken up
		}

	// PRIVATE
	// initializes a button input type.
	function _addbutton($name,$hot,$value,$y,$x)
		{
		$this->button_list[$this->button_total]['name'] = $name;
		$this->button_list[$this->button_total]['hot'] = $hot;
		$this->button_list[$this->button_total]['value'] = $value;
		$this->button_list[$this->button_total]['y'] = $y;
		$this->button_list[$this->button_total]['x'] = $x;		
		
		// add to master focus list
		$this->_focus_list[$this->_focus_total] = "B-" . $this->button_total;
		
		$this->button_total++;
		$this->_focus_total++;		
		}




	// PRIVATE	
	// Check if any user key input matches button hotkey designations
	// Returns the value assigned to that button.
	function _checkHotButtons($key)
		{
		for($n=0;$n<$this->button_total;$n++)
			{
			$thischar = strtolower(chr($key));
			$hotchar = strtolower( $this->button_list[$n]['hot'] );

			if($thischar == $hotchar)
				{
				return($this->button_list[$n]['value']);
				}			
			}
		return(NULL);
		}
		

		
	// PRIVATE	
	// Check if any user key input matches Menu item hotkey designations
	// Returns the value assigned to that Menu item.
	function _checkHotMenuItems($key)
		{
		for($n=0;$n<$this->menu_total;$n++)
			{
			$thischar = strtolower(chr($key));
			$hotchar = strtolower( $this->menu_list[$n]['hot'] );

			if($thischar == $hotchar)
				{
				if($this->mode == "checklist") // checklists support
					{
					$this->menu_list[$n]['selected'] = ( $this->menu_list[$n]['selected'] == false ? true : false );
					return(NULL);
					}
				else
					{
					$this->menu_cursor = $n;
					return($this->menu_list[$n]['value']);
					}
				}			
			}
		return(NULL);
		}



	// PRIVATE
	// Returns array name/value of all defined input boxes
	function _returnInputboxVals()
		{
		$results = array();
		
		for($n=0;$n<$this->inputbox_total;$n++)
			{
			$name = $this->inputbox_list[$n]['name'];
			$value = implode("",$this->inputbox_list[$n]['val']); // merge value array to string
			$results[$name] = $value;
			}
		return($results);
		}




	// PRIVATE
	// Displays all defined button types
	function _strokeAllButtons(&$win)
		{
		for($i=0;$i<$this->button_total;$i++)
			{
			ncurses_mvwaddstr( $win, $this->button_list[$i]['y'], $this->button_list[$i]['x'], "" );
			$len = strlen($this->button_list[$i]['name']);

			for($n=0;$n<$len;$n++)
				{
				$char = substr($this->button_list[$i]['name'],$n,1);
				$ord = ord($char);
				if($char == $this->button_list[$i]['hot']) // highlight char that is the hotkey
					{
					if(( $this->_focusCat() == "B" ) && ($i == $this->_focusSubindex() )) { ncurses_wcolor_set($win,6); } else { ncurses_wcolor_set($win,7); }	
					ncurses_wattron ( $win, NCURSES_A_BOLD );					
					ncurses_waddch ($win, $ord);	
					ncurses_wattroff ( $win, NCURSES_A_BOLD );				
					}
				else
					{
					if(( $this->_focusCat() == "B" ) && ($i == $this->_focusSubindex() )) { ncurses_wcolor_set($win,5); } else { ncurses_wcolor_set($win,2); }
					ncurses_waddch ($win, $ord);					
					}
				}

			}

		}
		
		

		
		
	// PRIVATE
	// Gets user input and acts based on button navigation style
	function _getButtonInput(&$win)
		{
		$exit = NULL;
		$keyPressed = ncurses_getch();
		//print" {".$keyPressed."} ";

		switch($keyPressed)
			{
			case NCURSES_KEY_RIGHT :
			case NCURSES_KEY_DOWN :
			case 9 :	// tab
				$this->_focus_cursor++;
				if($this->_focus_cursor >= $this->_focus_total)
					{ $this->_focus_cursor = 0;  }
				break;
			
			
			case NCURSES_KEY_LEFT :
			case NCURSES_KEY_UP :
			case 353 :	// shift-tab
				$this->_focus_cursor--;
				if($this->_focus_cursor < 0)
					{ $this->_focus_cursor = $this->_focus_total -1 ;  }
				break;			
			
			case 32 : // space
			case 13 : // enter	
				// get current focus
				list($ftype,$fnum) = explode("-",$this->_focus_list[$this->_focus_cursor]);
				// send the button value we are focused on
				$exit = $this->button_list[$fnum]['value'];	

				if($exit === "#FORM#")	// Support for inputbox 
					{
					$exit = $this->_returnInputboxVals();
					}

				break;			
			
			default:
				// check if any button hotkeys were pressed - return 'null' if no match
				$exit = $this->_checkHotButtons($keyPressed);
				break;
			}
		return($exit);		
		}
		
		
		
	// PRIVATE
	// Gets user input and acts based on textbox navigation style
	function _getTextboxInput(&$win)
		{
		$exit = NULL;
		$char = NULL;
		$keyPressed = ncurses_getch();
		if($keyPressed<256) // is this an ascii code/
			{ $char = chr($keyPressed); }
		//print" {".$keyPressed."} ";

		switch($keyPressed)
			{
			case NCURSES_KEY_DOWN :
			case 9 :	// tab
			case 13 : // enter	
				$this->_focus_cursor++;
				if($this->_focus_cursor >= $this->_focus_total)
					{ $this->_focus_cursor = 0;  }
				break;
			
			
			case NCURSES_KEY_UP :
			case 353 :	// shift-tab
				$this->_focus_cursor--;
				if($this->_focus_cursor < 0)
					{ $this->_focus_cursor = $this->_focus_total -1 ;  }
				break;	
			
			
			case NCURSES_KEY_RIGHT :
				$ii = $this->_focusSubindex(); // get selected inputbox
				$val_len = &$this->inputbox_list[$ii]['val_len'];
				$cursor = &$this->inputbox_list[$ii]['val_cursor'];
				if($cursor < $val_len) { $cursor++; }	
				else { $crap = ncurses_beep(); }
				break;
			
			
			case NCURSES_KEY_LEFT :
				$ii = $this->_focusSubindex(); // get selected inputbox
				$cursor = &$this->inputbox_list[$ii]['val_cursor'];
				if($cursor > 0) { $cursor--; }
				else { $crap = ncurses_beep(); }				
				break;			
			
		
			case 263 :	// backspace 
				$ii = $this->_focusSubindex(); // get selected inputbox				
				
				$cursor = &$this->inputbox_list[$ii]['val_cursor'];
				$value = &$this->inputbox_list[$ii]['val'];
				$value_len = &$this->inputbox_list[$ii]['val_len'];

				if($cursor > 0) 
					{ 
					$sub1 = array_slice($value, 0, $cursor-1);
					$sub2 = array_slice($value, $cursor);
					$value = array_merge($sub1,$sub2);
					$cursor--; 
					$value_len = count($value);
					}
				else
					{
					$crap = ncurses_beep();
					}
				
				
				// Refresh current input box
				$this->_drawInputBoxContents($ii);
				break;
			
			
			case 330 :	// del key 
				$ii = $this->_focusSubindex(); // get selected inputbox				
				
				$cursor = &$this->inputbox_list[$ii]['val_cursor'];
				$value = &$this->inputbox_list[$ii]['val'];
				$value_len = &$this->inputbox_list[$ii]['val_len'];
				
				$sub1 = array_slice($value, 0, $cursor);
				$sub2 = array_slice($value, $cursor+1);
				$value = array_merge($sub1,$sub2);

				$value_len = count($value);

				// Refresh current input box
				$this->_drawInputBoxContents($ii);
				break;

			default:
	
				$ii = $this->_focusSubindex(); // get selected inputbox

				$value = &$this->inputbox_list[$ii]['val'];
				$value_len = &$this->inputbox_list[$ii]['val_len'];
				$value_max = &$this->inputbox_list[$ii]['max_length'];
				$cursor = &$this->inputbox_list[$ii]['val_cursor'];

				if( ($keyPressed >= 32) && ($keyPressed <= 126) )
					{
					if($value_len < $value_max)
						{
						
						$sub1 = array_slice($value, 0, $cursor);
						$sub2 = array_slice($value, $cursor);
						$new = array($char);
						$value = array_merge($sub1,$new,$sub2);
								
						
						//$value[$cursor] = $char;
						$cursor++;
						$value_len++;
						}
					else
						{
						$crap = ncurses_beep();
						}
					}
				
				// Refresh current input box
				$this->_drawInputBoxContents($ii);
					
			
				break;
			}

		return($exit);		
		}
		
		
		
		
		
		

	// PRIVATE
	// Gets user input and acts based on Menu/Checklist navigation style
	function _getMenuInput(&$win)
		{
		$exit = NULL;
		$keyPressed = ncurses_getch();
		//print" {".$keyPressed."} ";

		switch($keyPressed)
			{
			case NCURSES_KEY_DOWN :
				$this->menu_cursor++;
				if($this->menu_cursor >= $this->menu_total)
					{ $this->menu_cursor = 0;  }
				break;
			
			case NCURSES_KEY_UP :
				$this->menu_cursor--;
				if($this->menu_cursor < 0)
					{ $this->menu_cursor = $this->menu_total -1 ;  }
				break;

			
			case NCURSES_KEY_RIGHT :
			case 9 :	// tab
				$this->_focus_cursor++;
				if($this->_focus_cursor >= $this->_focus_total)
					{ $this->_focus_cursor = 0;  }
				break;
			
			
			case NCURSES_KEY_LEFT :
			case 353 :	// shift-tab
				$this->_focus_cursor--;
				if($this->_focus_cursor < 0)
					{ $this->_focus_cursor = $this->_focus_total -1 ;  }
				break;			
			
			case 32 : // space
				if($this->mode == "checklist")
					{
					// toggle item selection
					$this->menu_list[$this->menu_cursor]['selected'] = ( $this->menu_list[$this->menu_cursor]['selected'] == false ? true : false );
					}
				break;
					
			case 13 : // enter	
				$ii = $this->_focusSubindex(); // get selected inputbox
				if( $this->button_list[$ii]['value'] == true )
					{
					if($this->mode == "checklist") // checklists
						{
						$exit = $this->_getAllCheckboxVals();
						}
					else	// menu
						{
						// return the menu value we are focused on
						$exit = $this->menu_list[$this->menu_cursor]['value'];
						}
					}
				else
					{
					$exit = false;
					}
				break;			
			
			default:
				// check if any button hotkeys were pressed - return 'null' if no match
				$hot = $this->_checkHotButtons($keyPressed);	// try trap for hot buttons 1st.
				
				if($hot === true) // was hot button pressed?
					{
					if($this->mode == "checklist") // checklist support -- (optional)
						{
						$exit = $this->_getAllCheckboxVals();
						}
					else	// menu selection
						{
						// return the menu value we are focused on
						$exit = $this->menu_list[$this->menu_cursor]['value'];
						}
					}
				elseif($hot === false)
					{ $exit = false; }
				else // $hot == null , so let see if there is a hot-checkbox match
					{ $exit = $this->_checkHotMenuItems($keyPressed); }

				break;
				
			}
		return($exit);		
		}
		
		
		
	// PRIVATE
	// Returns array name/value of selected check boxes
	function _getAllCheckboxVals()
		{
		$exit = null;
		for($n=0;$n<$this->menu_total;$n++)
			{
			// return all items selected
			if( $this->menu_list[$n]['selected'] )
				{
				$name = &$this->menu_list[$n]['name'];
				$value = &$this->menu_list[$n]['value'];
				$exit[$name] = $value;
				}							
			}
		if(!is_array($exit)) // if nothing was selected we still need to return something
			{
			$exit = true;
			}
		return($exit);
		}
		
	// PRIVATE
	// Initializes Menu Items (or Checklist) types
	function addMenuItem($type,$name,$label,$hotkey,$desc,$value=NULL,$selected=false)
		{
		$this->menu_list[$this->menu_total]['name'] = $name;
		$this->menu_list[$this->menu_total]['type'] = $type;
		$this->menu_list[$this->menu_total]['selected'] = $selected;
		$this->menu_list[$this->menu_total]['label'] = $label;
		$this->menu_list[$this->menu_total]['desc'] = $desc;
		$this->menu_list[$this->menu_total]['hot'] = $hotkey;
		$this->menu_list[$this->menu_total]['value'] = ( $value === NULL ? $this->menu_total : $value );	// default value if non given
		$this->menu_list[$this->menu_total]['y'] = $this->menu_total;
		$this->menu_list[$this->menu_total]['x'] = 0;		
		$this->menu_total++;	
		
		$len = strlen($label);		
		if($len > $this->menu_label_width) { $this->menu_label_width = $len; }	// track largest label width

		$len = strlen($desc);		
		if($len > $this->menu_desc_width) { $this->menu_desc_width = $len; }	// track largest description width
		}
		
		
		
	// PRIVATE
	// Displays all Menu Items (or Checklist) to screen
	function _strokeAllMenuItems(&$win)
		{
		for($i=0;$i<$this->menu_total;$i++)
			{
			if($this->menu_cursor == $i) { ncurses_wcolor_set($win,5); } else { ncurses_wcolor_set($win,2); }
			
			// output Checkbox -- (optional)
			if($this->mode == "checklist")	// for checklist type menus
				{
				$sel = ( $this->menu_list[$i]['selected'] == true ? "\xFB" : " " );
				ncurses_mvwaddstr( $win, $this->menu_list[$i]['y'], $this->menu_list[$i]['x'], "[".$sel."]" );
				ncurses_mvwaddstr( $win, $this->menu_list[$i]['y'], $this->menu_list[$i]['x']+4, "" );
				$desc_offset = 5;
				}
			else
				{
				ncurses_mvwaddstr( $win, $this->menu_list[$i]['y'], $this->menu_list[$i]['x'], "" );
				$desc_offset = 2;
				}
			
			// output menu item label
			$len = strlen($this->menu_list[$i]['label']);
			for($n=0;$n<$len;$n++)
				{
				$char = substr($this->menu_list[$i]['label'],$n,1);
				$ord = ord($char);
				if($char == $this->menu_list[$i]['hot']) // highlight char that is the hotkey
					{
					if($this->menu_cursor == $i) { ncurses_wcolor_set($win,6); } else { ncurses_wcolor_set($win,7); }	
					ncurses_wattron ( $win, NCURSES_A_BOLD );					
					ncurses_waddch ($win, $ord);	
					ncurses_wattroff ( $win, NCURSES_A_BOLD );				
					}
				else
					{
					if($this->menu_cursor == $i) { ncurses_wcolor_set($win,5); } else { ncurses_wcolor_set($win,2); }
					ncurses_waddch ($win, $ord);					
					}
				}
			// output menu item description
			if($this->menu_cursor == $i) { ncurses_wcolor_set($win,5); } else { ncurses_wcolor_set($win,2); }
			ncurses_mvwaddstr( $win, $this->menu_list[$i]['y'], $this->menu_list[$i]['x']+$this->menu_label_width+$desc_offset, $this->menu_list[$i]['desc'] );
			}
		}

		


	// PRIVATE	
	// Displays and creates a sub-window to contain the Menu Items
	// Returns the resource ID of the sub-window
	function _createMenuSubWindow(&$win,$parent_height, $parent_width, $para_offset_y)
		{
		// auto-detect window width based on menu contents
		if($this->mode == "checklist" )
			{
			$menu_width = 1 + 4 + $this->menu_label_width + 2 + $this->menu_desc_width + 1;
			}
		else
			{
			$menu_width = 1 + $this->menu_label_width + 2 + $this->menu_desc_width + 1;
			}
		
		// Draw borders
		$cord['y'] = 1 + $para_offset_y;
		$cord['x'] = round( ($parent_width-$menu_width) / 2   );
		//$cord = $this->_getCoordinates($parent_height,$menu_width,"center","middle");
		$y = $this->menu_total+2;
		$x = $menu_width;
		$this->_drawThinBorders($win, $cord['y'], $cord['x'] ,$y, $x );
		
		// draw window inside borders
		$cord = $this->_getCoordinates($parent_height,$menu_width,"center","middle");
		$swin = ncurses_newwin ( $y-2, $x-2, $cord['y']+$para_offset_y+2, $cord['x']+1 );

		// fill window
		ncurses_wcolor_set($swin,2);	
		for($row=0;$row<$y-2;$row++)
			{
			ncurses_wmove ( $swin, $row, 0 );
			ncurses_whline ( $swin, 32, $x-2 );
			}
			
		return($swin);
		}
		

		
		
		
		
		
		
		
		
		
		
		
		
// ################## PUBLIC ###################

	// PUBLIC
	// Used to set internal class variables
	function set($varname,$varvalue)
		{
		$this->$varname = $varvalue;
		}


	// PUBLIC
	// Wrapper method to initialize Checkbox items
	function addChecklist($name,$label,$hotkey,$desc,$value=NULL,$selected=false)
		{
		$this->addMenuItem("check",$name,$label,$hotkey,$desc,$value,$selected);
		}

	// PUBLIC
	// Wrapper method to initialize Menu items
	function addMenu($label,$hotkey,$desc,$value=NULL)
		{
		$name = "";
		$this->addMenuItem("menu",$name,$label,$hotkey,$desc,$value,false);
		}

		
	// PUBLIC
	function _menu()
		{
		// open a dialog box window
		$cord = $this->_getCoordinates($this->height,$this->width,"center","middle");
		$win = $this->_createDialogWindow($cord['y'], $cord['x'], $this->height, $this->width, true);

		// output dialog text
		$para_offset_y = $this->_stroke_para($win,$this->text,$this->height,$this->width,"center",true);

		// Create menu sub-window
		$mwin = $this->_createMenuSubWindow($win,$this->height,$this->width, $para_offset_y);

		// configure buttons
		$ok_offset_x_yes = round( ($this->width - 23) / 2 );
		$ok_offset_x_no = $ok_offset_x_yes + 13;		
		$this->_addbutton("[ Select ]","S",true,$this->height-2,$ok_offset_x_yes);
		$this->_addbutton("[ Cancel ]","C",false,$this->height-2,$ok_offset_x_no);

		// wait for input
		do
			{
			$this->_strokeAllButtons($win);
			$this->_strokeAllMenuItems($mwin);
			ncurses_wrefresh($win);
			ncurses_wrefresh($mwin);
			// get keyboard input			
			$status = $this->_getMenuInput($win);			
			}
			while( $status === NULL );

		return($status);
		}

		


		
	// PUBLIC
	function _messagebox()
		{
		// open a dialog box window
		$cord = $this->_getCoordinates($this->height,$this->width,"center","middle");
		$win = $this->_createDialogWindow($cord['y'], $cord['x'], $this->height, $this->width, true);

		// output dialog text
		$dontcare = $this->_stroke_para($win,$this->text,$this->height,$this->width,"center",true);
		
		// ok button
		$ok_offset_x = round( ($this->width - 6) / 2 );
		$this->_addbutton("[ OK ]","O",true,$this->height-2,$ok_offset_x);

		// wait for input
		do
			{
			$this->_strokeAllButtons($win);
			ncurses_wrefresh($win);
			// get keyboard input			
			$status = $this->_getButtonInput($win);			
			}
			while( $status === NULL );

		return(true);
		}




	// PUBLIC
	function _yesno()
		{
		// open a dialog box window
		$cord = $this->_getCoordinates($this->height,$this->width,"center","middle");
		$win = $this->_createDialogWindow($cord['y'], $cord['x'], $this->height, $this->width, true);

		// output dialog text
		$dontcare = $this->_stroke_para($win,$this->text,$this->height,$this->width,"center",true);
		
		// configure buttons
		$ok_offset_x_yes = round( ($this->width - 16) / 2 );
		$ok_offset_x_no = $ok_offset_x_yes + 10;		
		$this->_addbutton("[ Yes ]","Y",TRUE,$this->height-2,$ok_offset_x_yes);
		$this->_addbutton("[ No ]","N",FALSE,$this->height-2,$ok_offset_x_no);

		// wait for input
		do
			{
			$this->_strokeAllButtons($win);
			ncurses_wrefresh($win);
			// get keyboard input			
			$status = $this->_getButtonInput($win);			
			}
			while( $status === NULL );

		return($status);
		}




		
	// PUBLIC
	function _notice()
		{
		if( ($this->height == 0) || ($this->height == 0) )
			{
			// auto-detect required height & width based on text
			$this->width = $this->height = 0;		
			$text = str_replace ( "\n", "|", $this->text ); // decode any linebreaks		
			$lines = explode("|",$text);		
			foreach($lines as $line)
				{
				$len = strlen($line);			
				if($len > $this->width)
					{ $this->width = $len; }
				$this->height++;
				}
			}

		// open a dialog box window
		$cord = $this->_getCoordinates($this->height,$this->width,"center","middle");
		$this->nwin = $this->_createDialogWindow($cord['y'], $cord['x'], $this->height+2, $this->width+2, true, 2);

		$crap = $this->_stroke_para($this->nwin,$this->text,$this->height,$this->width+2,"center",false);		
		ncurses_wrefresh($this->nwin);
		
		}
		

		
		
	// PUBLIC
	function _inputbox()
		{
		// open a dialog box window
		$cord = $this->_getCoordinates($this->height,$this->width,"center","middle");
		$win = $this->_createDialogWindow($cord['y'], $cord['x'], $this->height, $this->width, true);

		// output dialog text
		$para_offset_y = $this->_stroke_para($win,$this->text,$this->height,$this->width,"center",true);

		// Create menu sub-window
		$cord_x = round(($this->width/2) - ($this->length/2) );
		$cord_y =  2 + $para_offset_y;
		$this->addInputBox($win,"val","",$cord_y,$cord_x,$this->length,$this->default_val);

		// configure buttons
		$ok_offset_x_yes = round( ($this->width - 23) / 2 );
		$ok_offset_x_no = $ok_offset_x_yes + 13;		
		$this->_addbutton("[ Accept ]","A","#FORM#",$this->height-2,$ok_offset_x_yes);
		$this->_addbutton("[ Cancel ]","C",false,$this->height-2,$ok_offset_x_no);

		// wait for input
		do
			{
			$this->_strokeAllButtons($win);
			$this->_strokeInputBoxes($win);
			ncurses_wrefresh($win);

			// get keyboard input
			if( $this->_focusCat() == "B" )
				{
				$status = $this->_getButtonInput($win);
				}
			else
				{
				$status = $this->_getTextboxInput($win);
				}
			}
			while( $status === NULL );

		if(isset($status['val']))
			{ $status = $status['val']; }
			
		return($status);
		}
		
		
		
	// PUBLIC
	function _checklist()
		{
		// open a dialog box window
		$cord = $this->_getCoordinates($this->height,$this->width,"center","middle");
		$win = $this->_createDialogWindow($cord['y'], $cord['x'], $this->height, $this->width, true);

		// output dialog text
		$para_offset_y = $this->_stroke_para($win,$this->text,$this->height,$this->width,"center",true);

		// Create menu sub-window
		$mwin = $this->_createMenuSubWindow($win,$this->height, $this->width, $para_offset_y);

		// configure buttons
		$ok_offset_x_yes = round( ($this->width - 19) / 2 );
		$ok_offset_x_no = $ok_offset_x_yes + 9;		
		$this->_addbutton("[ OK ]","O",true,$this->height-2,$ok_offset_x_yes);
		$this->_addbutton("[ Cancel ]","C",false,$this->height-2,$ok_offset_x_no);

		// wait for input
		do
			{
			$this->_strokeAllButtons($win);
			$this->_strokeAllMenuItems($mwin);
			ncurses_wrefresh($win);
			ncurses_wrefresh($mwin);
			// get keyboard input			
			$status = $this->_getMenuInput($win);			
			}
			while( $status === NULL );

		return($status);
		}
		
	



	function mode($mode,$size="")
		{
		$this->_reset();
		
		$this->mode = $mode;
		
		// set hieght/width
		if($size != "")
			{
			list($this->height,$this->width) = explode("x",$size);
			}
		else
			{
			$this->height = $this->width = 0;
			}
		}




	function stroke()
		{
		// startup ncurses
		ncurses_init();
		$this->_init_screen();

		switch($this->mode)
			{
			case"menu":
				$results = $this->_menu();
				break;
			
			case"checklist":
				$results = $this->_checklist();
				break;
			
			case"notice":
				$results = $this->_notice();
				break;
				
			case"msgbox":
				$results = $this->_messagebox();
				break;
				
			case"yesno":
				$results = $this->_yesno();
				break;
				
			case"inputbox":
				$results = $this->_inputbox();
				break;
				
				
				
				
			default:
				$results = "ERROR: unknown <mode>\n\n";
				
				
			}
		

		
		return($results);
		}





	
}

