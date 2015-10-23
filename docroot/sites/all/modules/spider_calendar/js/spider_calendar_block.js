jQuery(document).ready(function() {
  jQuery("a[class^='ahref_block_']").click(function(event) {
    spider_calendar_update_ajax_data_block(event);
  });
})

function spider_calendar_update_ajax_data_block(event) {
  var a1Tag = event.target;
  var rel = jQuery(a1Tag).attr('rel');
  var cName = a1Tag.className;
  var block_id = cName.substring(cName.lastIndexOf("_") + 1);
  block_url = Drupal.settings.spider_calendar["url_block" + block_id];
  date_block = parseInt(Drupal.settings.spider_calendar["date_block" + block_id]);
  month_block = parseInt(Drupal.settings.spider_calendar["month_block" + block_id]);
  if (month_block < 1) {
    date_block = --date_block;
    month_block = 12;
  }
  if (month_block > 12) {
    date_block = ++date_block;
    month_block = 1;
  }
  if (rel == '1') {
    date_block = date_block - 1;
    Drupal.settings.spider_calendar["date_block" + block_id] = date_block;
    Drupal.settings.spider_calendar["month_block" + block_id] = month_block;
  }
  if (rel == '2') {
    date_block = date_block + 1;
    Drupal.settings.spider_calendar["date_block" + block_id] = date_block;
    Drupal.settings.spider_calendar["month_block" + block_id] = month_block;
  }
  if (rel == '3') {
    month_block = month_block - 1;
    Drupal.settings.spider_calendar["month_block" + block_id] = month_block;
    Drupal.settings.spider_calendar["date_block" + block_id] = date_block;
  }
  if (rel == '4') {
    month_block = month_block + 1;
    Drupal.settings.spider_calendar["month_block" + block_id] = month_block;
    Drupal.settings.spider_calendar["date_block" + block_id] = date_block;
  }
  if (block_url) {
    jQuery.post(
      block_url,
      {
        date_block_year: date_block,
        date_block_month: month_block
      },
      function (data) {
        jQuery('#block-spider-calendar-spider-calendar' + block_id).html(jQuery(data).find('#block-spider-calendar-spider-calendar' + block_id).html());
        jQuery('.ahref_block_' + block_id).click(function(event) {
            spider_calendar_update_ajax_data_block(event);
        });
      }
    );
  }
  if (event) {
    event.preventDefault();
  }
  return false;
}
