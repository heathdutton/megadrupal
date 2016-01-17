(function ($) {
  $(document).ready(function() {
    $('div.opac_items_availability').each(function() {
      var nid = $(this).find(".node").text();

      var url = "/opac_items/availability/" + nid;

      var avail = $(this);
      $.ajax({
        type: 'GET',
        url: url,
        success: function(data, textStatus, jqXHR) {
          $(avail).html(data);
        },
        error: function(jqXHR, textStatus, errorThrown) {
          $(avail).html(Drupal.t('Unable to retrieve items availability', {}, {'context': 'libraries'}));
        }
      });
    });

    $('div.opac_items_information').each(function() {
      var nid = $(this).children(".node").text();
      var limititems = $(this).children(".limititems").text();
      var url = "/opac_items/information/" + nid + "/" + limititems;

      var info = $(this);
      $.ajax({
        type: 'GET',
        url: url,
        success: function(data, textStatus, jqXHR) {
          $(info).html(data);
          if (Drupal.settings.opac_items.hide_no_items && data != 'hide_no_items') {
            $('#items_informations_' + nid).show();
          }
        },
        error: function(jqXHR, textStatus, errorThrown) {
          $(info).html(Drupal.t('Unable to retrieve items information', {}, {'context': 'libraries'}));
        }
      });
    });
  });
})(jQuery);
