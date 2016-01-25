(function ($) {
  function executeFunctionByName(functionName, context /*, args */) {
    var args = Array.prototype.slice.call(arguments).splice(2);
    var namespaces = functionName.split(".");
    var func = namespaces.pop();
    for (var i = 0; i < namespaces.length; i++) {
      context = context[namespaces[i]];
    }
    return context[func].apply(this, args);
  }

  Drupal.GoPushAPI = {
    running: {},
    ping_killswitch: {},
    start: function (data) {
      var id = data.path + ' :: ' + data.center;
      if (!Drupal.GoPushAPI.running[id]) {
        Drupal.GoPushAPI.running[id] = true;
        if (window['WebSocket']) {
          var conn = new WebSocket('ws' + (data.secure ? 's' : '') + '://' + data.path + '/listen?center=' + data.center);
          conn.onclose = function () {
            Drupal.GoPushAPI.running[id] = false;
            if (data.ondisconnect) {
              executeFunctionByName(data.ondisconnect, window);
            }
          };
          conn.onopen = function () {
            if (data.onconnect) {
              executeFunctionByName(data.onconnect, window);
            }
          };
          conn.onmessage = function (evt) {
            // alert(evt.data);
            if (data.onmessagereceive) {
              executeFunctionByName(data.onmessagereceive, window, evt.data);
            }
          };
        }
        else {
          var loop;
          loop = function () {
            $.ajax({
              url: 'http' + (data.secure ? 's' : '') + '://' + data.path + '/ping?center=' + data.center,
              dataType: 'jsonp',
              success: function (d) {
                executeFunctionByName(data.onmessagereceive, window, d);
                if (!Drupal.GoPushAPI.ping_killswitch[id]) {
                  setTimeout(loop, 1000);
                }
                else {
                  Drupal.GoPushAPI.running[id] = false;
                }
              },
              error: function () {
                Drupal.GoPushAPI.running[id] = false;
              }
            });

            loop();
          }
        }
        return true;
      }
      return false;
    }
  };

  Drupal.behaviors.GoPushAPI = {
    attach: function () {
      for (var i in Drupal.settings.gopushapi_clients) {
        Drupal.GoPushAPI.start(Drupal.settings.gopushapi_clients[i]);
      }
    }
  };
})(jQuery);
