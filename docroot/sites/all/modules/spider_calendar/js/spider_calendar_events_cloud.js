function spider_calendar_show_titles_list(ev, text) {
  spider_calendar_get_cursor_xy(ev);
  document.getElementById('spiderCalendarTitlesList').innerHTML = '<table style="border-collapse:separate;" cellpadding="0" cellspacing="0" border="0" width="100%"><tr style="background-color:inherit;"><td id="spider_calendar_src1" style="background: url(\'' + Drupal.settings.spider_calendar.site_url + '/TitleListBg1.png\') no-repeat scroll 0 0 transparent; height: 65px; margin: 0; padding: 0;">&nbsp;</td></tr><tr style="background-color:inherit;"><td id="spider_calendar_src2" style="background: url(\'' + Drupal.settings.spider_calendar.site_url + '/TitleListBg2.png\') repeat-y scroll 0 0 transparent; margin: 0; padding: 0 20px 0 20px;">' + text + '</td></tr><tr style="background-color:inherit;"><td id="spider_calendar_src3" style="background: url(\'' + Drupal.settings.spider_calendar.site_url + '/TitleListBg3.png\') no-repeat scroll 0 0 transparent; height: 32px; margin: 0; padding: 0;">&nbsp;</td></tr>';
  document.getElementById('spiderCalendarTitlesList').style.left = (tempX - 33) + "px";
  document.getElementById('spiderCalendarTitlesList').style.top = (tempY + 3) + "px";
  document.getElementById('spiderCalendarTitlesList').style.display = "block";
}

var tempX = 0;
var tempY = 0;

function spider_calendar_get_cursor_xy(e) {
  e = e || window.event;
  if (e.pageX || e.pageY) {
    tempX = e.pageX - (document.documentElement.scrollLeft || document.body.scrollLeft);
    tempY = e.pageY - (document.documentElement.scrollTop || document.body.scrollTop);
  }
  else {
    tempX = e.clientX - document.documentElement.clientLeft;
    tempY = e.clientY - document.documentElement.clientTop;
  }
}

function spider_calendar_hide_titles_list() {
  if (document.getElementById('spiderCalendarTitlesList')) {
    document.getElementById('spiderCalendarTitlesList').style.display = "none";
  }
  return;
}

var oldFunctionOnLoad = null;

function spider_calendar_add_to_onload() {
  if (oldFunctionOnLoad && al2) {
    al2 = false;
    oldFunctionOnLoad();
  }
  var spiderCalendarTitlesListElement = document.createElement('div');
  var spiderCalendarTitlesListId = document.createAttribute('id');
  spiderCalendarTitlesListId.nodeValue = 'spiderCalendarTitlesList';
  spiderCalendarTitlesListElement.setAttributeNode(spiderCalendarTitlesListId);
  spiderCalendarTitlesListElement.setAttribute('style', 'position:fixed; width:331px; z-index:99;')
  document.body.appendChild(spiderCalendarTitlesListElement);
}
window.onload = spider_calendar_add_to_onload;
