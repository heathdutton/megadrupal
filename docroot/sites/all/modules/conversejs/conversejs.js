/**
 * Integrates converse.js with Drupal
 */

(function($) {
  Drupal.behaviors.conversejs = {
    'attach': function(context){
      //require(['converse'], function (converse){
      var settings = Drupal.settings.conversejs;

      // Disable multiple initialization process (eg.: on AJAX request).
      if (Drupal.settings.conversejs.initialized !== undefined) {
        return;
      }

      //For details about various configuration parameters, please refer to converse.js documentations.
      var conf = {
        'prebind':                    settings.prebind,
        'bosh_service_url':           settings.bosh_service_url,
        'allow_contact_requests':     settings.allow_contact_requests,
        'allow_muc':                  settings.allow_muc,
        'animate':                    settings.animate,
        'auto_list_rooms':            settings.auto_list_rooms,
        'auto_reconnect':             settings.auto_reconnect,
        'auto_subscribe':             settings.auto_subscribe,
        'allow_otr':                  settings.allow_otr,
        'use_otr_by_default':         settings.use_otr_by_default,
        'cache_otr_key':              settings.cache_otr_key,
        'debug':                      settings.debug,
        'expose_rid_and_sid':         settings.expose_rid_and_sid,
        'hide_muc_server':            settings.hide_muc_server,
        'i18n':                       locales[settings.i18n_locale],
        'show_controlbox_by_default': settings.show_controlbox_by_default,
        'show_call_button':           settings.show_call_button,
        'show_only_online_users':     settings.show_only_online_users,
        'use_vcards':                 settings.use_vcards,
        'xhr_custom_status':          settings.xhr_custom_status,
        'xhr_user_search':            settings.xhr_user_search
      };

      //full name should be set only if is used.
      if (settings.fullname) {
        conf.fullname = settings.fullname;
      }

      //if custom_status is enabled, set its url
      if (conf.xhr_custom_status) {
        conf.xhr_custom_status_url = settings.xhr_custom_status_url;
      }

      //if user_search is enabled, set its url
      if (conf.xhr_user_search) {
        conf.xhr_user_search_url = settings.xhr_user_search_url;
      }

      if (conf.prebind) {
        //Get pre-bind credentials from server via ajax and then initialize converse.js
        $.getJSON(settings.prebind_path, function(data) {
          if (data.error) {
            conf.prebind = false;
            throw new Error(data.error);
          }
          else {
            conf.jid = data.jid;
            conf.sid = data.sid;
            conf.rid = data.rid;
          }

          converse.initialize(conf);
          Drupal.settings.conversejs.initialized = true;
        });
      }
      else {
        //Initialize converse.js with defined configuration.
        converse.initialize(conf);
        Drupal.settings.conversejs.initialized = true;
      }
      //});
    }
  };
})(jQuery);
