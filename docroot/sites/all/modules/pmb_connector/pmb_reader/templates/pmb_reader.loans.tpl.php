<?php

/**
 * @file
 * PMB reader account loans template.
 */

$header = array(
  t('Title'),
  t('Bar code'),
  t('Media type'),
  t('Location'),
  t('Section'),
  t('Loan start date'),
  t('Loan return date'),
);
$rows = array();

foreach ($loans as $key => $aloan) {
  $notice_link = $aloan->notice_id ?
    l($aloan->expl_libelle, _pmb_p('catalog/record/') . $aloan->notice_id, array('html' => TRUE)) :
    $aloan->expl_libelle;

  // Is loan late?
  if ($key == 'late') {
    $return_date = "<span style='color:red;'>" . $aloan->loan_returndate . '<br />';
  }
  else {
    $return_date = $aloan->loan_returndate;
  }

  $rows[] = array(
    $notice_link,
    $aloan->expl_cb,
    $aloan->expl_support,
    l($aloan->expl_location_caption, _pmb_p('catalog/location/') . $aloan->expl_location_id),
    l($aloan->expl_section_caption, _pmb_p('catalog/section/') . $aloan->expl_section_id),
    $aloan->loan_startdate,
    $return_date,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No loan.')));
