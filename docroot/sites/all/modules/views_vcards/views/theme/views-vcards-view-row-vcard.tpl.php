<?php

/**
 * @file
 * Default view template to display a vCard item.
 *
 * The following variables are available:
 * $first_name
 * $middle_name
 * $last_name
 * $full_name
 * $title
 * $email
 * $email2
 * $email3
 * $home_address
 * $home_city
 * $home_state
 * $home_zip
 * $home_country
 * $home_phone
 * $home_cellphone
 * $home_website
 * $work_title
 * $work_company
 * $work_address
 * $work_city
 * $work_state
 * $work_zip
 * $work_country
 * $work_phone
 * $work_fax
 * $work_website
 */
?>
BEGIN:VCARD<?php echo "\n"; ?>
VERSION:4.0<?php echo "\n"; ?>
N:<?php echo $last_name; ?>;<?php echo $first_name; ?>;<?php echo $middle_name; ?>;<?php echo $title; ?>;<?php echo "\n"; ?>
FN:<?php echo $full_name; ?> <?php echo "\n"; ?>
ADR;INTL;PARCEL;WORK:;;<?php echo $work_address; ?>;<?php echo $work_city; ?>;<?php echo $work_state; ?>;<?php echo $work_zip; ?>;<?php echo $work_country; ?><?php echo "\n"; ?>
ADR;DOM;PARCEL;HOME:;;<?php echo $home_address; ?>;<?php echo $home_city; ?>;<?php echo $home_state; ?>;<?php echo $home_zip; ?>;<?php echo $home_country; ?><?php echo "\n"; ?>
EMAIL;INTERNET:<?php echo $email; ?><?php echo "\n"; ?>
EMAIL;INTERNET:<?php echo $email2; ?><?php echo "\n"; ?>
EMAIL;INTERNET:<?php echo $email3; ?><?php echo "\n"; ?>
ORG:<?php echo $work_company; ?><?php echo "\n"; ?>
TEL;WORK:<?php echo $work_phone; ?><?php echo "\n"; ?>
TEL;FAX;WORK:<?php echo $work_fax; ?><?php echo "\n"; ?>
TEL;CELL:<?php echo $home_cellphone; ?><?php echo "\n"; ?>
TEL;HOME:<?php echo $home_phone; ?><?php echo "\n"; ?>
TITLE:<?php echo $work_title; ?><?php echo "\n"; ?>
URL;WORK:<?php echo $work_website; ?><?php echo "\n"; ?>
URL:<?php echo $home_website; ?><?php echo "\n"; ?>
END:VCARD<?php echo "\n"; ?>
