jQuery(document).ready(function ($) {
  // Contains '%commerce_order' to be replaced by the order ID
  var refresh_status_pattern_uri = Drupal.settings.commerce_bpost.refresh_status_pattern_url;

  $('td .refresh').each(function () {
    var cell = $(this);
    $(this).text(Drupal.t('Refresh...'));
    var classes = $(this).attr('class').split(' ');
    var order_id = undefined;
    var i = 0;
    while (i < classes.length && order_id === undefined) {
      if (classes[i].indexOf('order-') === 0) {
        order_id = classes[i].substr('order-'.length);
      }
      i++;
    }
    if (order_id === undefined) {
      return false;
    }
    var uri = refresh_status_pattern_uri.replace('%commerce_order', order_id);
    $.get(uri, function (data) {
      processStatusResp(cell, data);
    }, 'json');
  });

  function processStatusResp(cell, data) {
    if (typeof data.status_name === 'undefined' || typeof data.status_i18n === 'undefined') {
      return false;
    }
    $(cell).text(data.status_i18n);
  }
});
