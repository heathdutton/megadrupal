function spider_calendar_set_theme() {
  themeID = document.getElementById('edit-default-themes').value;
  var if_set;
  switch (themeID) {
    case '1':
      if_set = spider_calendar_reset_theme_1();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Black light";
      }
      break;

    case '2':
      if_set = spider_calendar_reset_theme_2();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Blue Dark";
      }
      break;

    case '3':
      if_set = spider_calendar_reset_theme_3();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Blue Elegant";
      }
      break;

    case '4':
      if_set = spider_calendar_reset_theme_4();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Blue Green Mix";
      }
      break;

    case '5':
      if_set = spider_calendar_reset_theme_5();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Blue Light";
      }
      break;

    case '6':
      if_set = spider_calendar_reset_theme_6();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Blue Simple";
      }
      break;

    case '7':
      if_set = spider_calendar_reset_theme_7();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Brown Elegant";
      }
      break;

    case '8':
      if_set = spider_calendar_reset_theme_8();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Green Dark";
      }
      break;

    case '9':
      if_set = spider_calendar_reset_theme_9();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Green Elegant";
      }
      break;

    case '10':
      if_set = spider_calendar_reset_theme_10();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Green Light";
      }
      break;

    case '11':
      if_set = spider_calendar_reset_theme_11();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Red Elegant";
      }
      break;

    case '12':
      if_set = spider_calendar_reset_theme_12();
      if (if_set) {
        document.getElementById("edit-theme-title").value = "new Blue";
      }
      break;
  }
}

function spider_calendar_reset_theme_1() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Black light";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("000000");
    document.getElementById("edit-border-radius").value = "8";
    document.getElementById("edit-border-width").value = "2";
    document.getElementById("edit-body-bgcolor").color.fromString("323232");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("FFFFFF");
    document.getElementById("edit-omd-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-events").color.fromString("000000");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("F0F0F0");
    document.getElementById("edit-event-title-color").color.fromString("000000");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("FFFFFF");
    document.getElementById("edit-cell-border-color").color.fromString("000000");
    document.getElementById("edit-sundays-tcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("323232");
    document.getElementById("edit-sundays-fcolor").value = "16";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "90";
    document.getElementById("edit-header-bgcolor").color.fromString("2A2829");
    document.getElementById("edit-year-font-size").value = "18";
    document.getElementById("edit-year-color").color.fromString("FFFFFF");
    document.getElementById("edit-year-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-month-fsize").value = "16";
    document.getElementById("edit-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "12";
    document.getElementById("edit-weekdays-color").color.fromString("FFFFFF");
    document.getElementById("edit-week-cell-height").value = "35";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("969696");
    document.getElementById("edit-sunday-bgcolor").color.fromString("969696");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = "40"
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("323232");
    document.getElementById("edit-arrow-color-popup").color.fromString("C7C7C7");
    document.getElementById("edit-popup-bgcolor").color.fromString("969696");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_2() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Blue Dark";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("000000");
    document.getElementById("edit-border-radius").value = "";
    document.getElementById("edit-border-width").value = "2";
    document.getElementById("edit-body-bgcolor").color.fromString("5BCAFF");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("E6E6E6");
    document.getElementById("edit-omd-bgcolor").color.fromString("C0C0C0");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("000000");
    document.getElementById("edit-cell-text-color-events").color.fromString("FFFFFF");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("00004F");
    document.getElementById("edit-event-title-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("FFFFFF");
    document.getElementById("edit-cell-border-color").color.fromString("000000");
    document.getElementById("edit-sundays-tcolor").color.fromString("000000");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("8ADAFF");
    document.getElementById("edit-sundays-fcolor").value = "18";
    document.getElementById("edit-days-fsize").value = "14";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "120";
    document.getElementById("edit-header-bgcolor").color.fromString("00004F");
    document.getElementById("edit-year-font-size").value = "40";
    document.getElementById("edit-year-color").color.fromString("D1D4F5");
    document.getElementById("edit-year-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("D1D4F5");
    document.getElementById("edit-cur-month-fsize").value = "20";
    document.getElementById("edit-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "14";
    document.getElementById("edit-weekdays-color").color.fromString("FFFFFF");
    document.getElementById("edit-week-cell-height").value = "30";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("000000");
    document.getElementById("edit-sunday-bgcolor").color.fromString("000000");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = "0"
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("00004F");
    document.getElementById("edit-arrow-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-popup-bgcolor").color.fromString("009EEB");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_3() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Blue Elegant";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("346699");
    document.getElementById("edit-border-radius").value = "4";
    document.getElementById("edit-border-width").value = "10";
    document.getElementById("edit-body-bgcolor").color.fromString("E3F9F9");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("FFFFFF");
    document.getElementById("edit-omd-bgcolor").color.fromString("CCCCCC");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("2410EE");
    document.getElementById("edit-cell-text-color-events").color.fromString("000000");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FFCC33");
    document.getElementById("edit-event-title-color").color.fromString("000000");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("346699");
    document.getElementById("edit-cell-border-color").color.fromString("6B6B6B");
    document.getElementById("edit-sundays-tcolor").color.fromString("2410EE");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("CDDDFF");
    document.getElementById("edit-sundays-fcolor").value = "18";
    document.getElementById("edit-days-fsize").value = "14";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "100";
    document.getElementById("edit-header-bgcolor").color.fromString("346699");
    document.getElementById("edit-year-font-size").value = "33";
    document.getElementById("edit-year-color").color.fromString("FFFFFF");
    document.getElementById("edit-year-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-month-fsize").value = "16";
    document.getElementById("edit-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "12";
    document.getElementById("edit-weekdays-color").color.fromString("FFFFFF");
    document.getElementById("edit-week-cell-height").value = "25";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("68676D");
    document.getElementById("edit-sunday-bgcolor").color.fromString("68676D");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("000000");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("346699");
    document.getElementById("edit-arrow-color-popup").color.fromString("E3B62D");
    document.getElementById("edit-popup-bgcolor").color.fromString("FFCC33");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_4() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Blue Green Mix";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("ABCEA8");
    document.getElementById("edit-border-radius").value = "2";
    document.getElementById("edit-border-width").value = "8";
    document.getElementById("edit-body-bgcolor").color.fromString("E3F9F9");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("B0B0B0");
    document.getElementById("edit-omd-bgcolor").color.fromString("E1DDE9");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("383838");
    document.getElementById("edit-cell-text-color-events").color.fromString("383838");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("C0EFC0");
    document.getElementById("edit-event-title-color").color.fromString("383838");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("58A42B");
    document.getElementById("edit-cell-border-color").color.fromString("B1B1B0");
    document.getElementById("edit-sundays-tcolor").color.fromString("FF7C5C");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-fcolor").value = "16";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "40";
    document.getElementById("edit-header-bgcolor").color.fromString("C0EFC0");
    document.getElementById("edit-year-font-size").value = "18";
    document.getElementById("edit-year-color").color.fromString("58A42B");
    document.getElementById("edit-year-arrow-color").color.fromString("58A42B");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("58A42B");
    document.getElementById("edit-cur-month-fsize").value = "18";
    document.getElementById("edit-month-arrow-color").color.fromString("58A42B");
    document.getElementById("edit-next-month-color").color.fromString("58A42B");
    document.getElementById("edit-next-month-fsize").value = "16";
    document.getElementById("edit-next-month-arrow-color").color.fromString("58A42B");
    document.getElementById("edit-prev-month-color").color.fromString("58A42B");
    document.getElementById("edit-prev-month-fsize").value = "16";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("58A42B");
    document.getElementById("edit-arrow-size").value = "10";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "25";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sunday-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-weekdays-fsize").value = "12";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("262626");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("58A42B");
    document.getElementById("edit-arrow-color-popup").color.fromString("AED9AE");
    document.getElementById("edit-popup-bgcolor").color.fromString("C0EFC0");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_5() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Blue Light";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("000000");
    document.getElementById("edit-border-radius").value = "0";
    document.getElementById("edit-border-width").value = "4";
    document.getElementById("edit-body-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-height").value = "80";
    document.getElementById("edit-omd-fcolor").color.fromString("525252");
    document.getElementById("edit-omd-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("000000");
    document.getElementById("edit-cell-text-color-events").color.fromString("FFFFFF");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FFA142");
    document.getElementById("edit-event-title-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("36A7E9");
    document.getElementById("edit-cell-border-color").color.fromString("000000");
    document.getElementById("edit-sundays-tcolor").color.fromString("36A7E9");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-fcolor").value = "14";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "80";
    document.getElementById("edit-header-bgcolor").color.fromString("36A7E9");
    document.getElementById("edit-year-font-size").value = "22";
    document.getElementById("edit-year-color").color.fromString("000000");
    document.getElementById("edit-year-arrow-color").color.fromString("000000");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("000000");
    document.getElementById("edit-cur-month-fsize").value = "14";
    document.getElementById("edit-month-arrow-color").color.fromString("000000");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "16";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "40";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sunday-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-date-fsize-popup").value = "16";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Bold";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("FFA142");
    document.getElementById("edit-arrow-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-popup-bgcolor").color.fromString("36A7E9");
    document.getElementById("edit-popup-width").value = "800";
    document.getElementById("edit-popup-height").value = "600";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_6() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Blue Simple";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("3DBCEB");
    document.getElementById("edit-border-radius").value = "6";
    document.getElementById("edit-border-width").value = "12";
    document.getElementById("edit-body-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("C7C7C7");
    document.getElementById("edit-omd-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("1374C3");
    document.getElementById("edit-cell-text-color-events").color.fromString("000000");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FCF7D9");
    document.getElementById("edit-event-title-color").color.fromString("000000");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("9A0000");
    document.getElementById("edit-cell-border-color").color.fromString("1374C3");
    document.getElementById("edit-sundays-tcolor").color.fromString("013A7D");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-fcolor").value = "16";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "93";
    document.getElementById("edit-header-bgcolor").color.fromString("FCF7D9");
    document.getElementById("edit-year-font-size").value = "33";
    document.getElementById("edit-year-color").color.fromString("9A0000");
    document.getElementById("edit-year-arrow-color").color.fromString("9A0000");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("9A0000");
    document.getElementById("edit-cur-month-fsize").value = "16";
    document.getElementById("edit-month-arrow-color").color.fromString("9A0000");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "10";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "20";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("013A7D");
    document.getElementById("edit-sunday-bgcolor").color.fromString("1374C3");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("000000");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("000000");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Bold";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("21B5FF");
    document.getElementById("edit-arrow-color-popup").color.fromString("E0E0E0");
    document.getElementById("edit-popup-bgcolor").color.fromString("FCF7D9");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_7() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Brown Elegant";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("FFC219");
    document.getElementById("edit-border-radius").value = "6";
    document.getElementById("edit-border-width").value = "10";
    document.getElementById("edit-body-bgcolor").color.fromString("7E5F43");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("FFFFFF");
    document.getElementById("edit-omd-bgcolor").color.fromString("523F30");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-events").color.fromString("404040");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FFC219");
    document.getElementById("edit-event-title-color").color.fromString("404040");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("FFFFFF");
    document.getElementById("edit-cell-border-color").color.fromString("000000");
    document.getElementById("edit-sundays-tcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("7E5F43");
    document.getElementById("edit-sundays-fcolor").value = "18";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "100";
    document.getElementById("edit-header-bgcolor").color.fromString("E7C892");
    document.getElementById("edit-year-font-size").value = "30";
    document.getElementById("edit-year-color").color.fromString("404040");
    document.getElementById("edit-year-arrow-color").color.fromString("404040");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("404040");
    document.getElementById("edit-cur-month-fsize").value = "20";
    document.getElementById("edit-month-arrow-color").color.fromString("404040");
    document.getElementById("edit-next-month-color").color.fromString("404040");
    document.getElementById("edit-next-month-fsize").value = "16";
    document.getElementById("edit-next-month-arrow-color").color.fromString("404040");
    document.getElementById("edit-prev-month-color").color.fromString("404040");
    document.getElementById("edit-prev-month-fsize").value = "16";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("404040");
    document.getElementById("edit-arrow-size").value = "12";
    document.getElementById("edit-weekdays-color").color.fromString("404040");
    document.getElementById("edit-week-cell-height").value = "30";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("FFC219");
    document.getElementById("edit-sunday-bgcolor").color.fromString("FFC219");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("FFC219");
    document.getElementById("edit-arrow-color-popup").color.fromString("B3875F");
    document.getElementById("edit-popup-bgcolor").color.fromString("7E5F43");
    document.getElementById("edit-popup-width").value = "800";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "2";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_8() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Green Dark";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("D78B29");
    document.getElementById("edit-border-radius").value = "6";
    document.getElementById("edit-border-width").value = "12";
    document.getElementById("edit-body-bgcolor").color.fromString("F0F0E6");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("A6A6A6");
    document.getElementById("edit-omd-bgcolor").color.fromString("DDDCC8");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("5C5C5C");
    document.getElementById("edit-cell-text-color-events").color.fromString("FFFFFF");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("D78B29");
    document.getElementById("edit-event-title-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("000000");
    document.getElementById("edit-cell-border-color").color.fromString("363636");
    document.getElementById("edit-sundays-tcolor").color.fromString("000000");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("F0F0E6");
    document.getElementById("edit-sundays-fcolor").value = "16";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "100";
    document.getElementById("edit-header-bgcolor").color.fromString("598923");
    document.getElementById("edit-year-font-size").value = "33";
    document.getElementById("edit-year-color").color.fromString("FFFFFF");
    document.getElementById("edit-year-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-month-fsize").value = "16";
    document.getElementById("edit-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "12";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "30";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("D78B29");
    document.getElementById("edit-sunday-bgcolor").color.fromString("D78B29");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Courier New";
    document.getElementById("edit-event-title-fstyle-popup").value = "Bold";
    document.getElementById("edit-date-color-popup").color.fromString("000000");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("DDDCC8");
    document.getElementById("edit-arrow-color-popup").color.fromString("D78B29");
    document.getElementById("edit-popup-bgcolor").color.fromString("FFB061");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_9() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Green Elegant";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("FDFDCC");
    document.getElementById("edit-border-radius").value = "2";
    document.getElementById("edit-border-width").value = "14";
    document.getElementById("edit-body-bgcolor").color.fromString("FDFDCC");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("8C8C8C");
    document.getElementById("edit-omd-bgcolor").color.fromString("FDFDE8");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("383838");
    document.getElementById("edit-cell-text-color-events").color.fromString("383838");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FE7C00");
    document.getElementById("edit-event-title-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("000000");
    document.getElementById("edit-cell-border-color").color.fromString("4D4D4D");
    document.getElementById("edit-sundays-tcolor").color.fromString("000000");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("BACBDC");
    document.getElementById("edit-sundays-fcolor").value = "16";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "90";
    document.getElementById("edit-header-bgcolor").color.fromString("009898");
    document.getElementById("edit-year-font-size").value = "30";
    document.getElementById("edit-year-color").color.fromString("FFFFFF");
    document.getElementById("edit-year-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-month-fsize").value = "16";
    document.getElementById("edit-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "12";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "30";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("9865FE");
    document.getElementById("edit-sunday-bgcolor").color.fromString("9865FE");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("FDFDE8");
    document.getElementById("edit-arrow-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-popup-bgcolor").color.fromString("FE7C00");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_10() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Green Light";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("000000");
    document.getElementById("edit-border-radius").value = "0";
    document.getElementById("edit-border-width").value = "2";
    document.getElementById("edit-body-bgcolor").color.fromString("FDFCDE");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("6E5959");
    document.getElementById("edit-omd-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("060D12");
    document.getElementById("edit-cell-text-color-events").color.fromString("000000");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FF6933");
    document.getElementById("edit-event-title-color").color.fromString("000000");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("4AFF9E");
    document.getElementById("edit-cell-border-color").color.fromString("000000");
    document.getElementById("edit-sundays-tcolor").color.fromString("FF0000");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("FDFCDE");
    document.getElementById("edit-sundays-fcolor").value = "18";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "90";
    document.getElementById("edit-header-bgcolor").color.fromString("A6BA7D");
    document.getElementById("edit-year-font-size").value = "28";
    document.getElementById("edit-year-color").color.fromString("000000");
    document.getElementById("edit-year-arrow-color").color.fromString("000000");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("080808");
    document.getElementById("edit-cur-month-fsize").value = "18";
    document.getElementById("edit-month-arrow-color").color.fromString("000000");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "18";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "50";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("E6E6DE");
    document.getElementById("edit-sunday-bgcolor").color.fromString("BD848A");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("000000");;
    document.getElementById("edit-event-title-fsize-popup").value = "18"
    document.getElementById("edit-event-title-family-popup").value = "Courier New";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("000000");
    document.getElementById("edit-date-fsize-popup").value = "16";
    document.getElementById("edit-date-family-popup").value = "Courier New";
    document.getElementById("edit-date-fstyle-popup").value = "Bold";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("CCCCCC");
    document.getElementById("edit-arrow-color-popup").color.fromString("E0E0C5");
    document.getElementById("edit-popup-bgcolor").color.fromString("FDFCDE");
    document.getElementById("edit-popup-width").value = "800";
    document.getElementById("edit-popup-height").value = "600";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_11() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Red Elegant";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "mo";
    document.getElementById("edit-border-color").color.fromString("E6E6E4");
    document.getElementById("edit-border-radius").value = "6";
    document.getElementById("edit-border-width").value = "18";
    document.getElementById("edit-body-bgcolor").color.fromString("CDCC96");
    document.getElementById("edit-cell-height").value = "70";
    document.getElementById("edit-omd-fcolor").color.fromString("525252");
    document.getElementById("edit-omd-bgcolor").color.fromString("E4E7CC");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("000000");
    document.getElementById("edit-cell-text-color-events").color.fromString("FFFFFF");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("9A0000");
    document.getElementById("edit-event-title-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("9A0000");
    document.getElementById("edit-cell-border-color").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-tcolor").color.fromString("000000");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("CDCC96");
    document.getElementById("edit-sundays-fcolor").value = "18";
    document.getElementById("edit-days-fsize").value = "";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "100";
    document.getElementById("edit-header-bgcolor").color.fromString("9A0000");
    document.getElementById("edit-year-font-size").value = "33";
    document.getElementById("edit-year-color").color.fromString("FFFFFF");
    document.getElementById("edit-year-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-month-fsize").value = "16";
    document.getElementById("edit-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "10";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "60";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("FFFED0");
    document.getElementById("edit-sunday-bgcolor").color.fromString("FFFED0");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("000000");;
    document.getElementById("edit-event-title-fsize-popup").value = "18"
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("000000");
    document.getElementById("edit-date-fsize-popup").value = "";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Normal";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("9A0000");
    document.getElementById("edit-arrow-color-popup").color.fromString("DEDDB5");
    document.getElementById("edit-popup-bgcolor").color.fromString("FFFED0");
    document.getElementById("edit-popup-width").value = "600";
    document.getElementById("edit-popup-height").value = "500";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

function spider_calendar_reset_theme_12() {
  if (confirm(Drupal.t('Do you really whant to reset theme?'))) {
    document.getElementById("edit-theme-title").value = "Blue Light";
    document.getElementById("edit-theme-width").value = "650";
    document.getElementById("edit-week-start-day").value = "su";
    document.getElementById("edit-border-color").color.fromString("000000");
    document.getElementById("edit-border-radius").value = "0";
    document.getElementById("edit-border-width").value = "4";
    document.getElementById("edit-body-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-height").value = "80";
    document.getElementById("edit-omd-fcolor").color.fromString("525252");
    document.getElementById("edit-omd-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-cell-text-color-wevents").color.fromString("000000");
    document.getElementById("edit-cell-text-color-events").color.fromString("FFFFFF");
    document.getElementById("edit-cell-bgcolor-events").color.fromString("FFA142");
    document.getElementById("edit-event-title-color").color.fromString("FFFFFF");
    document.getElementById("edit-cur-day-cell-border-color").color.fromString("36A7E9");
    document.getElementById("edit-cell-border-color").color.fromString("000000");
    document.getElementById("edit-sundays-tcolor").color.fromString("36A7E9");
    document.getElementById("edit-sundays-cell-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sundays-fcolor").value = "14";
    document.getElementById("edit-days-fsize").value = "12";
    document.getElementById("edit-time-in-cell-1").checked = true;
    document.getElementById("edit-time-in-cell-0").checked = false;
    document.getElementById("edit-header-heidht").value = "80";
    document.getElementById("edit-header-bgcolor").color.fromString("36A7E9");
    document.getElementById("edit-year-font-size").value = "22";
    document.getElementById("edit-year-color").color.fromString("000000");
    document.getElementById("edit-year-arrow-color").color.fromString("000000");
    document.getElementById("edit-month-type").value = "1";
    document.getElementById("edit-cur-month-color").color.fromString("000000");
    document.getElementById("edit-cur-month-fsize").value = "14";
    document.getElementById("edit-month-arrow-color").color.fromString("000000");
    document.getElementById("edit-next-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-next-month-fsize").value = "";
    document.getElementById("edit-next-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-color").color.fromString("FFFFFF");
    document.getElementById("edit-prev-month-fsize").value = "";
    document.getElementById("edit-prev-month-arrow-color").color.fromString("FFFFFF");
    document.getElementById("edit-arrow-size").value = "16";
    document.getElementById("edit-weekdays-color").color.fromString("000000");
    document.getElementById("edit-week-cell-height").value = "40";
    document.getElementById("edit-weekdays-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-sunday-bgcolor").color.fromString("FFFFFF");
    document.getElementById("edit-weekdays-fsize").value = "14";
    document.getElementById("edit-date-format-popup").value="w/d/m/y";
    document.getElementById("edit-event-title-color-popup").color.fromString("FFFFFF");;
    document.getElementById("edit-event-title-fsize-popup").value = ""
    document.getElementById("edit-event-title-family-popup").value = "Arial";
    document.getElementById("edit-event-title-fstyle-popup").value = "Normal";
    document.getElementById("edit-date-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-date-fsize-popup").value = "16";
    document.getElementById("edit-date-family-popup").value = "Arial";
    document.getElementById("edit-date-fstyle-popup").value = "Bold";
    document.getElementById("edit-arrow-bgcolor-popup").color.fromString("FFA142");
    document.getElementById("edit-arrow-color-popup").color.fromString("FFFFFF");
    document.getElementById("edit-popup-bgcolor").color.fromString("36A7E9");
    document.getElementById("edit-popup-width").value = "800";
    document.getElementById("edit-popup-height").value = "600";
    document.getElementById("edit-displaied-events").value = "1";
    document.getElementById("edit-repeat-rate-1").checked = true;
    document.getElementById("edit-repeat-rate-0").checked = false;
    return true;
  }
  else {
    return false;
  }
}

