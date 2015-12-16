<?php

////////////////////////////////////////////////////////////////////////////
//FOR A SEASON OF AN ACTIVE SERIES PROJECT
function mnn_programming_exports_project_show_labels_pdf($project) {
  $var = 'mnn_pnid_'.$project->nid."_airing_data";
  $show_labels = $_SESSION[$var];
  mnn_programming_exports_show_label_batch_pdf($show_labels);
}
////////////////////////////////////////////////////////////////////////////
//FOR A SEASON OF ALL ACTIVE SERIES PROJECTS
function mnn_programming_exports_show_labels_pdf() {
  $airing_data = $_SESSION['mnn_project_airing_data']; 
  mnn_programming_exports_show_label_batch_pdf($airing_data);
}

////////////////////////////////////////////////////////////////////////////
//FOR A SINGLE LABEL FOR A SHOW
function mnn_programming_exports_show_label_single($show) {
  //var_dump("hi mom");
  $attributes = array('type'=>'single', 'show'=>$show);
  $label_info = 
    mnn_programming_exports_get_show_label_info($attributes);
  mnn_programming_exports_single_label_pdf($label_info);
}
/////////////////////////////////////////////////////////////////////////////
//GET DATA FOR A SINGLE SHOW LABEL
function mnn_programming_exports_get_show_label_info(&$attributes) {
  $ch1 = MNN_CHANNEL_1_TID;
  $ch2 = MNN_CHANNEL_2_TID;
  $ch3 = MNN_CHANNEL_3_TID;
  $ch4 = MNN_CHANNEL_4_TID;

  $show = $attributes['show'];
  if ($show) {
    $show_nid = $show->nid;

    /* 
    $label_info['project_id'];
    $label_info['episode_number'];
    $label_info['project_title'];
    $label_info['air_date'].' ';
    $label_info['air_time'];
    $label_info['channel'];
    $label_info['runtime'];
    */    
  }


  $airing_select = "SELECT pnode.title as project_title,
                 show_ep_number.field_episode_number_value as episode_number,
                 pnode.nid as project_id,
                 DATE_FORMAT(airing_date.field_airing_date_value, '%c/%d/%Y') 
                   AS air_date,                           
                 DATE_FORMAT(CONCAT('1111-11-11 ', 
                                    TIME(airing_date.field_airing_date_value)),
                             '%h:%i:%s%p') as air_time,
                   CASE airing_channel.field_airing_channel_tid
                     WHEN $ch1 THEN 'CH.1'
                     WHEN $ch2 THEN 'CH.2'
                     WHEN $ch3 THEN 'CH.3'
                     WHEN $ch4 THEN 'CH.4'
                     ELSE 'CH.?'
                   END as channel,
                 run_td.name as runtime";

  $airingless_select = "SELECT pnode.title as project_title,
                 show_ep_number.field_episode_number_value as episode_number,
                 pnode.nid as project_id,
                 '' AS air_date,                           
                 '' AS air_time,
                 '' AS channel,
                 run_td.name as runtime";

  $sql_from = "
          FROM field_data_field_episode_number show_ep_number
          INNER JOIN og_membership og_proj 
            ON show_ep_number.entity_id= og_proj.etid
          INNER JOIN node pnode ON pnode.nid=og_proj.gid
          INNER JOIN field_data_field_runtime runtime_field
            ON runtime_field.revision_id=show_ep_number.revision_id
          INNER JOIN taxonomy_term_data run_td
            ON run_td.tid=runtime_field.field_runtime_tid";

  $airing_joins = "
          INNER JOIN field_data_field_airing_show_ref airing_show
            ON show_ep_number.entity_id = 
              airing_show.field_airing_show_ref_target_id
          INNER JOIN field_data_field_airing_date airing_date
            ON airing_show.revision_id=airing_date.revision_id
          INNER JOIN field_data_field_airing_channel airing_channel
            ON airing_show.revision_id=airing_channel.revision_id";

  if ($attributes['type'] == 'single') {
    $sql .= $airing_select.$sql_from.$airing_joins.
      " WHERE show_ep_number.entity_id = :show_nid 
        ORDER BY airing_date.field_airing_date_value ASC ";
    $results = db_query($sql, array(':show_nid' => $show_nid));

    $result = $results->fetchAssoc();
    if ($result) {
      return $result;
    }
    else {
      $sql = $airingless_select.$sql_from.
	" WHERE show_ep_number.entity_id = :show_nid ";
      $results = db_query($sql, array(':show_nid' => $show_nid));
      return $results->fetchAssoc();
    }

  }
  else {

  }

  return $results;               
}
/////////////////////////////////////////////////////////////////////////////
//PDF FUNCTIONS
function mnn_programming_exports_single_label_pdf($label_info) {
  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);

  // create new PDF document
  $pdf = new TCPDF(L, mm, array(18.00,50.00), true);
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("MNN");
  $pdf->SetTitle($label_info['project_title'].' #'.
    $label_info['episode_number']);
  $pdf->SetSubject($label_info['project_title'].' #'.
    $label_info['episode_number']);
  $pdf->SetKeywords("PDF");
  $pdf->SetAutoPageBreak(false);
  $pdf->SetAutoPageBreak(false);

  // remove default header/footer
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);
  $pdf->setFooterMargin(0);
  $pdf->setPageOrientation(L,false,0);

  //set margins
  $pdf->SetMargins(0,0,0);

  //set auto page breaks
  $pdf->SetAutoPageBreak(FALSE, PDF_MARGIN_BOTTOM);

  //set image scale factor
  $pdf->setImageScale(1);

  //initialize document
  $pdf->AliasNbPages();

  $pdf->AddPage();

  // set font
  $pdf->SetFont("dejavusans", "", 6);

  $style = array(
    "position" => "S",
    "border" => false,
    "padding" => 0,
    "fgcolor" => array(0,0,0),
    "bgcolor" => false, //array(255,255,255),
    "text" => false,
    "font" => "dejavusans",
    "fontsize" => 4,
    "stretchtext" => 0
  );

  //set data to be printed
//breaking house number into two so project id can be bolded
  $house_number=$label_info['project_id'].'.'.$label_info['episode_number'];
  $projectid =$label_info['project_id'];
  $project_episodenumber =".".$label_info['episode_number'];
  $project_name=$label_info['project_title'];

  if (strlen($project_name) > 36) {
    $project_name = substr($project_name, 0, 34)."...";
  }
  else if(strlen($project_name) < 16) {
    $project_name .= "\n";
  }
  
  if ($label_info['air_date']) {
    $footer_text = $label_info['air_date'].' '.$label_info['air_time'].
      '  '.$label_info['channel'].' '.
      str_replace('mins', 'min',$label_info['runtime']);;
  }
  else {
    $footer_text = $label_info['runtime'];
  }

  // CODE 128 A

  $pdf->SetFont("dejavusans", "B", 10);
  $pdf->write(2,$projectid);
  $pdf->SetFont("dejavusans", "", 10);
  $pdf->write(2,$project_episodenumber);
  $pdf->Ln();
  $pdf->SetFont("dejavusans", "", 10);
  $pdf->write(2,$project_name);
  $pdf->Ln();
  $pdf->SetFont("dejavusans", "", 7);
  $pdf->write(2,$footer_text);
  $pdf->Ln();

  //Close and output PDF document
  $needle = "/\s/";
  $rplcmnt = "_";
  $pdf->Output("tape_label_".
               preg_replace($needle, $rplcmnt, $label_info['project_title']).
               '_'.
               preg_replace($needle, $rplcmnt, $label_info['episode_number']).
               ".pdf"
             );
}
function mnn_programming_exports_show_label_batch_pdf($show_labels) {
  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);

  //label style/layout configuration section
  $label_rows = 10;
  $label_columns = 3;

  //starting x point for all columns
  $col_1_start =11;
  $col_2_start =81;
  $col_3_start =150;

  //starting y point for all rows
  $row_1_start = 17;
  $row_2_start = 42;
  $row_3_start = 68;
  $row_4_start = 93;
  $row_5_start = 120;
  $row_6_start = 144;
  $row_7_start = 169;
  $row_8_start = 194;
  $row_9_start = 220;
  $row_10_start = 245;

  //offsets for height of lines
  $text_height_offset = 4;
  $barcode_text_height_offset= 3;
  $barcode_height_offset = 11;
  // since barcodes don't line up where they should
  //we need to use an offset to add to the x for the rest of that cell
  $barcode_x_offset = 5;

  // create new PDF document
  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, LETTER, true);

  // set document information
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("MNN");
  $pdf->SetTitle("MNN_Show_Labels");
  $pdf->SetSubject("MNN Show Labels");
  $pdf->SetKeywords("PDF");

  //set auto page breaks
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
  $pdf->SetFont("dejavusans", "", 6);

  //initialize col and row counters
  $row_counter =1;
  $col_counter =1;

  foreach($show_labels as $key => $label_item){
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
    }elseif ($col_counter == 3){
      $startingX=$col_3_start;
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
    $pdf->SetFont("dejavusans", "B", 10);
    $pdf->write(2,$label_item['project_id']);
    $pdf->SetFont("dejavusans", "", 10);
    $pdf->write(2,".".$label_item['episode_number']);
    $pdf->Ln();

    $startingY=($startingY+4);
    $pdf->SetXY($startingX,$startingY);
    $pdf->write(2,$label_item['project_title_line1']);
    $pdf->Ln();

    $startingY=($startingY+4);
    $pdf->SetXY($startingX,$startingY);
    $pdf->write(2,$label_item['project_title_line2']);
    $pdf->Ln();

    $startingY=($startingY+4);
    $pdf->SetXY($startingX,$startingY);
    $pdf->SetFont("dejavusans", "", 7);
    $pdf->write(2,$label_item['footer']);
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
  $pdf->Output("MNN Show Labels", "I");
}
