<?php

////////////////////////////////////////////////////////////////////////////
//LETTER FOR A SEASON OF ALL ACTIVE SERIES PROJECTS
function mnn_programming_exports_letters_pdf() {
  $project_data =  $_SESSION['mnn_active_series_project_data']; 
  $airing_data = $_SESSION['mnn_project_airing_data']; 

  $letters = array();
  foreach($airing_data as $airing) {
    $letters[$airing['project_id']] = $letters[$airing['project_id']] ? 
      $letters[$airing['project_id']] :array();
    $letters[$airing['project_id']][] = $airing;
  }

  mnn_programming_exports_letter_pdfbuild($project_data, $letters);
}

////////////////////////////////////////////////////////////////////////////
//LETTER FOR A SINGLE SERIES PROJECT
function mnn_programming_exports_project_letter_pdf ($project) {
  $var = 'mnn_pnid_'.$project->nid."_project_data";
  $project_data = $_SESSION[$var];
  
  $var = 'mnn_pnid_'.$project->nid."_airing_data";
  $airing_data = $_SESSION[$var];

  foreach($airing_data as $airing) {
    $letters[$project->nid] = $letters[$project->nid] ? 
      $letters[$project->nid] : array();
    $letters[$project->nid][] = $airing;
  }

  mnn_programming_exports_letter_pdfbuild($project_data, $letters);
}


function mnn_programming_exports_letter_pdfbuild($project_data, $letters) {
  set_time_limit(0);

  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);
  
  $ldate = date('M j, Y');

  $pdf = new TCPDF('Portrait', PDF_UNIT, PDF_PAGE_FORMAT, true); 
  $pdf->SetMargins(8,8,8);
  $pdf->setPrintHeader(FALSE);
  $pdf->AddPage();
  $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 8);
  
  $is_first = TRUE;
  foreach ($letters as $project_id=>$airings) {

    if (!$is_first) {
      $pdf->AddPage();
    }
    else {
      $is_first = FALSE;
    }
    $project_array = $project_data[$project_id];
    $quarter_data = $project_array['quarter'];

    $dates = explode( '--', $quarter_data);
    $quarter = mnn_programming_exports_get_quarter_name(strtotime($dates[0]));
    $quarter_start = date('F d, Y', strtotime($dates[0]));
    $quarter_end = date('F d, Y', strtotime($dates[1]));

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 8);
    $pdf->Write(4, date('M j, Y'));
    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 16);
    $pdf->Write(4, "P-".$project_id, '', 0, 'R');

    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 12);
    $pdf->Write(4, "MNN $quarter Quarter Confirmation", '', 0, 'C');

    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 11);
    $pdf->Write(4, "Below is your confirmed schedule for the Spring Quarter, which runs from $quarter_start through $quarter_end. Please note that your Project ID is an important identifier that MNN uses to track and program your shows. You may be asked for this number when submitting requests or communicating with MNN.");

    $pdf->Ln();
    $pdf->Ln();

    $pdf->Write(4, "Thank you for your involvement with MNN, and best wishes for a successful quarter!");
    $pdf->Ln();
    $pdf->Ln();
    	 
    $pdf->Write(4, "Name: ".$project_array['display_name'] );
    $pdf->Ln();
    $pdf->Write(4, "Project ID: ".$project_id);
    $pdf->Ln();
    $pdf->Write(4, "Show Name: ".$project_array['title']);
    $pdf->Ln();
    $pdf->Ln();
	
    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 11);
    $pdf->Write(4, "For producers who are renewing shows: ");

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 11);
    $pdf->Write(4, "     You may notice that your labels look different and do not contain episode numbers. For this quarter, we will be handling episode numbers internally.");
    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 11);
    $pdf->Write(4, "To all producers: ");
    $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 11);
    $pdf->Write(4, "   Please do not write on your labels!");
    $pdf->Ln();
    $pdf->Ln();

    $pdf->SetFont(PDF_FONT_NAME_MAIN, "B", 12);
    $pdf->Write(4, "Confirmed $quarter Quarter Schedule");
    $pdf->Ln();

    $pdf->SetFont("dejavusans", "", 9);
    //FIXME: how can this be generalized? 

    list($paras, $airings_rows, $airings_header) = 
      mnn_programming_exports_letter_template($project_array, $airings);

    mnn_programming_exports_write_paragraphs($pdf, $paras);

    if (count($airings) > 35 ) {
      $pdf->Write(4, "  (See next page.)");
      mnn_programming_exports_write_address($pdf);
      $pdf->AddPage();
      $pdf->SetFont("dejavusans", "", 8);
      $pdf->Write(4, $ldate);
      $pdf->SetFont("dejavusans", "B", 16);
      $pdf->Write(4, "P-".$project_id, '', 0, 'R');
      $pdf->SetFont("dejavusans", "B", 8);
      $pdf->Ln();
      $pdf->Ln();
    }

    $pdf->SetFont("dejavusans", "", 10);
    $pdf->Ln();
    $airings_table = theme_table(array('header'=>$airings_header, 
				       'rows'=>$airings_rows,
				       'attributes'=>array()));

    $pdf->writeHTML($airings_table, false, 0, false, 0);
    mnn_programming_exports_write_address($pdf);
  } 

  $pdf->lastPage();
  $pdf->Output("mnn_letter_batch.pdf", "I", "I");
}

function mnn_programming_exports_write_next_element(&$pdf, $text, &$x, &$y, $x_adder, $y_adder, $style) {
  $pdf->SetFont("dejavusans", $style, 11);
  $y += $y_adder;
  $x += $x_adder;
  $pdf->SetXY($x,$y);
  $pdf->write(2,$text);
}
 
/**
 *  these functions have mnn in the name to remind us how very mnn-specific they are
 */
function mnn_programming_exports_letter_template($project_array, $airings) {
  $quarter_data = $project_array['quarter'];

  $dates = explode( '--', $quarter_data);
  $quarter = mnn_programming_exports_get_quarter_name(strtotime($dates[0]));
  $quarter_start = date('F d, Y', strtotime($dates[0]));
  $quarter_end = date('F d, Y', strtotime($dates[1]));

  $paras = array(
		 );
  
  
  $airing_rows = array();

  foreach ( $airings as $a ) {
    $airing_rows[] = array ( 'data' => 
			     array (  // list of cells
				    array ( 'data' => $a['orig_channel']
					    ),
				    array ( 'data' => $a['airdate']
					    ),
				    array ( 'data' => $a['airtime']
					    ),
				    //array ( 'data' => $a['episode_number'] ),
				      ),
			     );
  }
  $airings_header = array( 
      array( 'data' => "<strong>Channel</strong>",
	     ),
      array( 'data' => "<b>Air Date</b>",
	     ),
      array( 'data' => "<strong><b>Air Time</b></strong>",
	     ),
//      array( 'data' => "<strong><b>Episode Number</b></strong>",),
			   );

  return array( $paras, $airing_rows, $airings_header );
}

function mnn_programming_exports_write_paragraphs(&$pdf, $paras) {
  foreach ( $paras as $p ) {
    $pdf->writeHTML($p, false, 0, false, 0);
    $pdf->Ln();
    $pdf->Ln();
  }
}

function mnn_programming_exports_write_address(&$pdf) {
  $pdf->SetFont("dejavusans", "B", 6);
  $alines = array(
		  "Manhattan Neighborhood Network",
		  "537 West 59th Street New York, NY 10019",
		  "ph: 212-757-2670 fx: 212-757-1603 http://www.mnn.org",
		  );




  if ( $pdf->GetY() < 256 ) {
    $pdf->SetY(264);
  }
  else { 
    $pdf->Ln();
  }
  foreach ( $alines as $l ) {
    $pdf->Write(4, $l, '', 0, 'C');
    $pdf->Ln();
  }
  $pdf->SetFont("dejavusans", "", 8);
}


function mnn_programming_exports_get_quarter_name($unixtime) {
  //FIXME: horribly hardcoded!
  $r = array(
	     3 => "Spring",
	     4 => "Spring",
	     5 => "Spring",
	     6 => "Summer",
	     7 => "Summer",
	     8 => "Summer",
	     9 => "Fall",
	     10 => "Fall",
	     11 => "Fall",
	     12 => "Winter",
	     1 => "Winter",
	     2 => "Winter",
	     );
  $month = date('n', $unixtime);
  $year = date('Y', $unixtime);
  if ( $month == 12 ) {
    $year++;
  }
  return $r[$month] . ' ' . $year;
}
