jQuery(document).ready(function() {
  jQuery('.ahref').click(function(event) {
    spider_calendar_update_ajax_data(event);
  });
})

function spider_calendar_update_ajax_data(event) {
  var a1Tag = event.target;
  var rel = jQuery(a1Tag).attr('rel');
  var nodeid = Drupal.settings.spider_calendar.nodeid;
  var date = parseInt(Drupal.settings.spider_calendar.date);
  var month = parseInt(Drupal.settings.spider_calendar.month);
  if (month < 1) {
    date = --date;
    month = 12;
  }
  if (month > 12) {
    date = ++date;
    month = 1;
  }
  if (rel == '1') {
    date = date - 1;
    Drupal.settings.spider_calendar.date = date;
    Drupal.settings.spider_calendar.month = month;
  }
  if (rel == '2') {
    date = date + 1;
    Drupal.settings.spider_calendar.date = date;
    Drupal.settings.spider_calendar.month = month;
  }
  if (rel == '3') {
    month = month - 1;
    Drupal.settings.spider_calendar.date = date;
    Drupal.settings.spider_calendar.month = month;
  }
  if (rel == '4') {
    month = month + 1;
    Drupal.settings.spider_calendar.date = date;
    Drupal.settings.spider_calendar.month = month;
  }
  if (nodeid) {
    jQuery.post(
      Drupal.settings.spider_calendar.url + nodeid,
      {
        nid: nodeid,
        date_year: date,
        date_month: month
      },
      function (data) {
        //jQuery('#wrapper').html(data);
        jQuery('#bigcalendar' + nodeid).html(jQuery(data).find('#bigcalendar' + nodeid).html());
        jQuery('.ahref').click(function(event) {
          spider_calendar_update_ajax_data(event);
        });
      }
    );
  }
  if (event) {
    event.preventDefault();
  }
  return false;
}
