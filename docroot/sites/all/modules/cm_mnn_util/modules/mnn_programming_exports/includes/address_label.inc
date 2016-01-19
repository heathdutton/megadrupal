<?php

////////////////////////////////////////////////////////////////////////////
//ADDRESS LABEL FOR A PROJECT
function mnn_programming_exports_project_address_label_pdf($project) {


  $var = 'mnn_pnid_'.$project->nid."_project_data";
  $project_data = $_SESSION[$var];
  $project_data = $project_data[$project->nid] ? $project_data[$project->nid] :
    array();

  $var = 'mnn_pnid_'.$project->nid."_airing_data";
  $airing_data = $_SESSION[$var];
  
  mnn_programming_exports_single_project_address_download($project_data);
}

////////////////////////////////////////////////////////////////////////////
//ADDRESS LABELS FOR A SEASON OF ALL ACTIVE SERIES PROJECTS
function mnn_programming_exports_address_labels_pdf() {
  $project_data =  $_SESSION['mnn_active_series_project_data']; 

  mnn_programming_exports_batch_project_address_download($project_data);
}


function mnn_programming_exports_single_project_address_download($label_info) {
  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);

  // create new PDF document
  $pdf = new TCPDF(L, mm, array(25.00,102.00), true);

  // set document information
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("MNN");
  $pdf->SetTitle("mailing label");
  $pdf->SetSubject("mailing label");
  $pdf->SetKeywords("PDF");

  // remove default header/footer
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);

  //set margins
  $pdf->SetMargins(0,0,0);

  //set auto page breaks
  $pdf->SetAutoPageBreak(FALSE, PDF_MARGIN_BOTTOM);

  //set image scale factor
  $pdf->setImageScale(1);

  //initialize document
  $pdf->AliasNbPages();

  // add a page
  $pdf->AddPage();

  // set font
  $pdf->SetFont("dejavusans", "", 10);

  $style = array(
    "position" => "S",
    "border" => false,
    "padding" => 1,
    "fgcolor" => array(0,0,0),
    "bgcolor" => false, //array(255,255,255),
    "text" => true,
    "font" => "dejavusans",
    "fontsize" => 8,
    "stretchtext" => 4
  );

  $name =$label_info['display_name'];
  $address_one =$label_info['street_address'];
  $address_two =$label_info['supplemental_address'];
  $city = $label_info['city'];
  $state = $label_info['state_province'];
  $zip =$label_info['postal_code'];
  $project_number_print='P-'.$label_info['project_id'];

  //set xy and print
  $pdf->SetXY(1,1);
  $pdf->write(2," ");
  $pdf->Ln();
  $pdf->SetXY(1,2);
  $pdf->write(2,$name);
  $pdf->write(2,"   ");
  $pdf->write(2,$project_number_print);
  $pdf->Ln();
  $pdf->SetXY(1,7);
  $pdf->write(3,$address_one);
  $pdf->Ln();
  $pdf->SetXY(1,13);
  if ($address_two) {
    $pdf->write(3,$address_two);
    $pdf->Ln();
    $pdf->SetXY(1,19);
  }
  $pdf->write(2,$city);
  $pdf->write(2,", ");
  $pdf->write(2,$state);
  $pdf->write(2,"  ");
  $pdf->write(2,$zip);
  $pdf->Ln();

  //Close and output PDF document
  $pdf->Output("Mailing Label".$project_number_print."pdf", "I");

}

function mnn_programming_exports_batch_project_address_download($addresses) {
  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);

  //label style/layout configuration section
  $label_rows = 10;
  $label_columns = 2;

  //starting x point for all columns
  $col_1_start =7;
  $col_2_start =112;

  //starting y point for all rows
  $row_1_start = 13;
  $row_2_start = 38;
  $row_3_start = 63;
  $row_4_start = 89;
  $row_5_start = 114;
  $row_6_start = 140;
  $row_7_start = 168;
  $row_8_start = 191;
  $row_9_start = 216;
  $row_10_start = 242;

  //offsets for height of lines
  $text_height_offset = 4;
  $barcode_text_height_offset= 3;
  $barcode_height_offset = 11;

  // since barcodes don't line up where they should
  //we need to use an offset to add to the Y for the rest of that cell
  $barcode_y_offset = 5;

  // create new PDF document
  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, LETTER, true);

  // set document information
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("MNN");
  $pdf->SetTitle("MNN Mailing labels");
  $pdf->SetSubject("MNN Mailing labels");
  $pdf->SetKeywords("PDF");

  //set auto page breaks
  //$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);
  $pdf->SetAutoPageBreak(false);

  // remove default header/footer
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);
  $pdf->setFooterMargin(0);
  $pdf->setPageOrientation(P,false,0);

  //set margins
  $pdf->SetMargins(0,0,0);

  //set image scale factor
  $pdf->setImageScale(1);

  //initialize document
  $pdf->AliasNbPages();

  // add a page
  $pdf->AddPage();

  // set font for type
  $pdf->SetFont("dejavusans", "", 10);

  $style = array(
    "position" => "S",
    "border" => false,
    "padding" => 1,
    "fgcolor" => array(0,0,0),
    "bgcolor" => false, //array(255,255,255),
    "text" => true,
    "font" => "dejavusans",
    "fontsize" => 8,
    "stretchtext" => 4

  );
  //initialize col and row counters
  $row_counter =1;
  $col_counter =1;


  //for each loop starts here
  foreach($addresses as $project_number => $label_item){


  $name =$label_item['display_name'];
  $address_one =$label_item['street_address'];
  $address_two =$label_item['supplemental_address'];
  $city = $label_item['city'];
  $state = $label_item['state_province'];
  $zip =$label_item['postal_code'];
  $project_number_print='P-'.$label_item['project_id'];

  //first check to see if we're at the end of a page
  if ($row_counter > $label_rows){
    $row_counter = 1;
    // add a page
      $pdf->AddPage();

  }

  //check to see if we are at the end of a row
  if ($col_counter > $label_columns){
    $col_counter = 1;
  }

  //get starting x and y for this cell

  if ($col_counter == 1){
    $startingX=$col_1_start;
  }elseif ($col_counter == 2){
      $startingX=$col_2_start;
  }

  if ($row_counter == 1){
    $startingY=($row_1_start);
  }elseif ($row_counter == 2){
    $startingY=($row_2_start);
  }elseif ($row_counter == 3){
    $startingY=($row_3_start);
  }elseif ($row_counter == 4){
    $startingY=($row_4_start);
  }elseif ($row_counter == 5){
    $startingY=($row_5_start);
  }elseif ($row_counter == 6){
    $startingY=($row_6_start);
  }elseif ($row_counter == 7){
    $startingY=($row_7_start);
  }elseif ($row_counter == 8){
    $startingY=($row_8_start);
  }elseif ($row_counter == 9){
    $startingY=($row_9_start);
  }elseif ($row_counter == 10){
    $startingY=($row_10_start);
  }

  //set xy and print
  $pdf->SetXY($startingX,$startingY);
  $pdf->write(2,$name);
  //$pdf->write(2," ");
  //$pdf->write(2,$lastname);
  $pdf->write(2,"   ");
  $pdf->write(2,$project_number_print);
  $pdf->Ln();
  $startingY=($startingY+$text_height_offset);
  $pdf->SetXY($startingX,$startingY);
  $pdf->write(2,$address_one);
  $pdf->Ln();
  $startingY=($startingY+$text_height_offset);
  $pdf->SetXY($startingX,$startingY);
  if ($address_two) {
    $pdf->write(2,$address_two);
    $pdf->Ln();
    $startingY=($startingY+$text_height_offset);
    $pdf->SetXY($startingX,$startingY);
  }
  $pdf->write(2,$city);
  $pdf->write(2,", ");
  $pdf->write(2,$state);
  $pdf->write(2,"  ");
  $pdf->write(2,$zip);
  $pdf->Ln();

  //increment row and col counter and end loop
  //if at the end of a row, increment the row counter
  if ($col_counter == $label_columns){
  $row_counter=($row_counter+1);
  }
  //increment column counter
  $col_counter=($col_counter+1);


  }
  // ---------------------------------------------------------
  //after completion of loop
  //Close and output PDF document
  $pdf->Output("MNN Mailing Labels", "I");

}
