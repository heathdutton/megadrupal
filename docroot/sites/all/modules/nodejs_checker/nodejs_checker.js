/**
 * @File
 *
 * Javascript client handler for nodejs checker.
 */

(function($) {
  var interval = false;
  var client = false;
  var server = false;
  var transport = false;

  Drupal.behaviors.nodejs_checker = {
    'attach': function(context, setting) {
      if (client == false && context == document) {
         interval = setInterval(Drupal.behaviors.nodejs_checker.check_server, 5000);
      }
    },

    'setClient' : function(state) {
      client = state;
      if (client == true) {
        server = true;
        Drupal.behaviors.nodejs_checker.killInterval();
      }
      else {
        interval = setInterval(Drupal.behaviors.nodejs_checker.check_server, 5000)
      }
    },

    'setServer' : function(state) {
      server = state;
      Drupal.behaviors.nodejs_checker.switchState(state);
    },

    'killInterval': function() {
      clearInterval(interval);
      interval = false;
    },

    'setTransport': function(trans) {
      transport = trans;
      if (transport == false) {
        $("#nodejs_checker_transport").removeClass();
        $("#nodejs_checker_transport").addClass("nodejs_checker_hidden");
      }
      else {
        $("#nodejs_checker_transport").removeClass("nodejs_checker_hidden");
        $("#nodejs_checker_transport").addClass(transport);
        $("#nodejs_checker_transport").html(Drupal.t("Connected via <span class='nodejs_checker_transport_name'>@transport</span>", { '@transport': transport }));
      }
    },

    'switchState': function(state) {
      if (state == false) {
        $('#nodejs_checker_server_state_disconnected').removeClass("nodejs_checker_hidden");
        $('#nodejs_checker_server_state_connected').addClass("nodejs_checker_hidden");
      }
      else {
        $('#nodejs_checker_server_state_disconnected').addClass("nodejs_checker_hidden");
        $('#nodejs_checker_server_state_connected').removeClass("nodejs_checker_hidden");
      }
    },

    'check_server': function() {
      $.ajax({
        url: "/nodejs_checker",
        success: function(data) {
          Drupal.behaviors.nodejs_checker.setServer(data);
          if (client == true) {
            Drupal.behaviors.nodejs_checker.killInterval();
          }
        }
      });
    },

  };

  Drupal.Nodejs.connectionSetupHandlers.nodejs_checker = {
    'connect' : function() {
      $("#nodejs_checker_client_connected").removeClass("nodejs_checker_hidden");
      $("#nodejs_checker_client_disconnected").addClass("nodejs_checker_hidden");
      Drupal.behaviors.nodejs_checker.setClient(true);
      Drupal.behaviors.nodejs_checker.setTransport(Drupal.Nodejs.socket.socket.transport.name);
    },
    'disconnect' : function() {
      $("#nodejs_checker_client_connected").addClass("nodejs_checker_hidden");
      $("#nodejs_checker_client_disconnected").removeClass("nodejs_checker_hidden");
      Drupal.behaviors.nodejs_checker.setClient(false);
      Drupal.behaviors.nodejs_checker.setTransport(false);
    },
  };
})(jQuery);
