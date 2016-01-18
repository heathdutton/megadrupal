/**
 * @file
 * This is jQuery code to refresh the block content, play adhan and more.
 */

(function ($) {

Drupal.behaviors.adhan = {
  attach: function (context, settings) {
    if (!Drupal.settings.adhan['queueCompass']) {
      setTimeout(adhan_refreshCompass, Drupal.settings.adhan['refreshCompass'] * 1000);
      Drupal.settings.adhan['queueCompass'] = true;
    }

    var time = new Date().getTime() / 1000;
    for (var i = 1; i < 9; i++) {
      if (i == 8) {
        break;
      }
      if (time < Drupal.settings.adhan.times[i]) {
        break;
      }
    }
    if (i != Drupal.settings.adhan.current) {
      if (i == 8) {
        adhan_refreshTable();
      }
      else {
        adhan_notify(i);
      }
    }

    var next = (Drupal.settings.adhan.times[i] - time) * 1000;
    setTimeout(Drupal.behaviors.adhan, next);
  }
};

adhan_refreshCompass = function () {
  $.ajax({
    url: Drupal.settings.adhan.basePath + "calc-compass",
    dataType: "json",
    success: function (response) {
      if (response !== false) {
        $('#block-adhan-adhan-compass').attr('src', response.url);
      }
    },
  });

  setTimeout(adhan_refreshCompass, Drupal.settings.adhan['refreshCompass'] * 1000);
};

adhan_refreshTable = function () {
  $.ajax({
    url: Drupal.settings.adhan.basePath + "calc-times",
    dataType: "json",
    success: function (response) {
      if (response !== false) {
        Drupal.settings.adhan.times = response.times;
        $('#block-adhan-adhan .time').each(function (i) {
          $(this).html(response.table[i]);
          if (i == 1) {
            $(this).parent().addClass('next');
          }
          else {
            $(this).parent().removeClass('current');
            $(this).parent().removeClass('next');
          }
        });
      }
    },
  });

  Drupal.settings.adhan.current = 1;
};

adhan_notify = function (index) {
  $('#block-adhan-adhan .prayer').each(function (i) {
    if (i == index - 2 && i != 1) {
      $(this).addClass('current');
    }
    else {
      $(this).removeClass('current');
    }
    if (i == index - 1) {
      $(this).addClass('next');
    }
    else {
      $(this).removeClass('next');
    }
  });

  var notify = Drupal.settings.adhan.notify;
  if ((index == 2 || index > 3 && index < 8) && notify > 0) {
    $.ajax({
      url: Drupal.settings.adhan.basePath + "notification-" + (index - 2),
      dataType: "json",
      success: function (response) {
        var dialog = $('#adhan-dialog');
        var title = response.title;
        var content = response.content;

        if (dialog.dialog('isOpen')) {
          dialog.dialog('destroy');
        }
        dialog.html(content);
        dialog.show();
        dialog.dialog({
          title: title,
          modal: true,
          closeText: '',
          buttons: {
            'Ok': function () {
              $(this).dialog('destroy');
              $(this).html('');
            }
          }
        });

        // jQuery appears to contain a bug, fix it with this
        dialog.css('height', 'auto');
        dialog.css('width', 'auto');
        $('.ui-dialog-buttonpane').css('width', '100%');
      },
    });
  }

  Drupal.settings.adhan.current = index;
};

})(jQuery);