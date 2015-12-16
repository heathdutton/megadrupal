<?php

  $max_width = theme_get_setting('layout_max_width', $theme);
  $gutter_horizontal = theme_get_setting('gutter_horizontal', $theme) / 2;
  $gutter_vertical = theme_get_setting('gutter_vertical', $theme) / 2;

  $CSS .= <<<EOT
.container {
  max-width: {$max_width}px;
  padding-left: {$gutter_horizontal}px;
  padding-right: {$gutter_horizontal}px;
}
.row {
  margin-left: -{$gutter_horizontal}px;
  margin-right: -{$gutter_horizontal}px;
}
.col-xs-1, .col-sm-1, .col-md-1, .col-lg-1, .col-xs-2, .col-sm-2, .col-md-2, .col-lg-2, .col-xs-3, .col-sm-3, .col-md-3, .col-lg-3, .col-xs-4, .col-sm-4, .col-md-4, .col-lg-4, .col-xs-5, .col-sm-5, .col-md-5, .col-lg-5, .col-xs-6, .col-sm-6, .col-md-6, .col-lg-6, .col-xs-7, .col-sm-7, .col-md-7, .col-lg-7, .col-xs-8, .col-sm-8, .col-md-8, .col-lg-8, .col-xs-9, .col-sm-9, .col-md-9, .col-lg-9, .col-xs-10, .col-sm-10, .col-md-10, .col-lg-10, .col-xs-11, .col-sm-11, .col-md-11, .col-lg-11, .col-xs-12, .col-sm-12, .col-md-12, .col-lg-12 {
  padding-left: {$gutter_horizontal}px;
  padding-right: {$gutter_horizontal}px;
  margin-bottom: {$gutter_vertical}px; /* changed from padding to margin because buttons were looking bad */
}

EOT;

  $gutter_horizontal = theme_get_setting('gutter_horizontal_mobile', $theme) / 2;
  $gutter_vertical = theme_get_setting('gutter_vertical_mobile', $theme) / 2;

  $CSS .= <<<EOT
@media screen and (max-width: 1200px) {
   .container {
      padding-left: {$gutter_horizontal}px;
      padding-right: {$gutter_horizontal}px;
      margin-bottom: {$gutter_vertical}px;
    }
    .row {
      margin-left: -{$gutter_horizontal}px;
      margin-right: -{$gutter_horizontal}px;
    }
    .col-xs-1, .col-sm-1, .col-md-1, .col-lg-1, .col-xs-2, .col-sm-2, .col-md-2, .col-lg-2, .col-xs-3, .col-sm-3, .col-md-3, .col-lg-3, .col-xs-4, .col-sm-4, .col-md-4, .col-lg-4, .col-xs-5, .col-sm-5, .col-md-5, .col-lg-5, .col-xs-6, .col-sm-6, .col-md-6, .col-lg-6, .col-xs-7, .col-sm-7, .col-md-7, .col-lg-7, .col-xs-8, .col-sm-8, .col-md-8, .col-lg-8, .col-xs-9, .col-sm-9, .col-md-9, .col-lg-9, .col-xs-10, .col-sm-10, .col-md-10, .col-lg-10, .col-xs-11, .col-sm-11, .col-md-11, .col-lg-11, .col-xs-12, .col-sm-12, .col-md-12, .col-lg-12 {
      padding-left: {$gutter_horizontal}px;
      padding-right: {$gutter_horizontal}px;
    }
}

EOT;
