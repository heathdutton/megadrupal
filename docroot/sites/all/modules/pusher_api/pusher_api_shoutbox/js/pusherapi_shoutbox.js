(function($) {
  Drupal.behaviors.pusherApiShoutbox = function (context) {
    if (Drupal.settings.pusherapi.appkey != undefined && Drupal.settings.pusherapi.appkey != '') {
      var pusher = new Pusher(Drupal.settings.pusherapi.appkey);
      var channel = pusher.subscribe(Drupal.settings.pusherapi_shoutbox.channel);
      channel.bind(Drupal.settings.pusherapi_shoutbox.eventname, function(data) {
        var box = $('#pusherapi_shoutbox_channel');
        var container = document.createElement('div');
        var name = document.createElement('div');
        var message = document.createElement('div');
        var time = document.createElement('div');

        box.prepend(container);

        $(container).addClass('shoutbox-container');
        $(name).addClass('shoutbox-name');
        $(message).addClass('shoutbox-message');
        $(time).addClass('shoutbox-time');

        $(container).append(name);
        $(container).append(message);
        $(container).append(time);

        $(name).append(data.name);
        $(message).append(data.message);
        $(time).append(data.time);
      });

      var shoutbox = $('#pusherapi-shoutbox-form');
      if (shoutbox.length) {
        shoutbox.find('input[type="submit"]').click(function(e) {
          if (e.preventDefault) { e.preventDefault(); }
          var msg = shoutbox.find('textarea[name="message"]');
          var n = shoutbox.find('input[name="name"]');
          if (msg.val() != undefined && msg.val() != '') {
            $.ajax({
              url: Drupal.settings.basePath + 'pusherapi/shoutbox',
              type: 'POST',
              data: { message: msg.val(), name: n.val() },
              success: function() {
                msg.val('');
                return false;
              },
              error: function() {
                alert(Drupal.t('Something went wrong'));
                return false;
              }
            });
          }
        })
      }
    }
    else {
      alert(Drupal.t('There is no Pusher API appkey defined'));
    }
    return false;
  }
})(jQuery);