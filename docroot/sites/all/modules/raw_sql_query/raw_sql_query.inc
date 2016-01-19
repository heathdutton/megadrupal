<?php

class RsqExportExcel {
  public $objPHPExcel;

  public function __construct() {
    libraries_load('PHPExcel');
    $this->objPHPExcel = new PHPExcel();
    $this->objPHPExcel->setActiveSheetIndex(0);
  }

  public function prepareFile($fields, $rows) {
    global $user;
      if (!empty($rows)) {
      $rowCount = 1;
      $num = 0;
      foreach ($fields as $field) {
        $letter = self::get_name_from_number($num);
        $this->objPHPExcel->getActiveSheet()->SetCellValue($letter.$rowCount, $field);
        $num++;
      }

      // body
      foreach ($rows as $row) {
        $rowCount++;
        $num = 0;
        foreach($fields as $field) {
          $letter = self::get_name_from_number($num);
          $this->objPHPExcel->getActiveSheet()->SetCellValue($letter.$rowCount, $row->{$field});
          $num++;
        }

      }
    }
  }


  public function downloadFile($tablename) {
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="' . $tablename . '-' . date('d-m-Y') . '.xlsx"');
    header('Cache-Control: max-age=0');

    $objWriter = new PHPExcel_Writer_Excel2007($this->objPHPExcel);
    $objWriter->save('php://output');
    drupal_exit();
  }

  /**
   * [getNameFromNumber description]
   * http://stackoverflow.com/questions/3302857/algorithm-to-get-the-excel-like-column-name-of-a-number
   * Based on zero indexed numbers, meaning 0 == A, 1 == B, etc
   * @param  [type] $num [description]
   * @return [type]      [description]
   */

  static function get_name_from_number($num) {
      $numeric = $num % 26;
      $letter = chr(65 + $numeric);
      $num2 = intval($num / 26);
      if ($num2 > 0) {
          return get_name_from_number($num2 - 1) . $letter;
      } else {
          return $letter;
      }
  }

}
