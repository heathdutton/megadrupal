/**
 * @file
 * File for admin pages of Uptolike module.
 */

(function ($, Drupal, undefined) {
  Drupal.behaviors.Uptolike = {
    attach: function (context, settings) {

      var UptolikeSettingsJson = JSON.stringify(settings.Uptolike);

      var onmessage = function (e) {
        if (e.data && e.data.action == 'ready') {
          var win = document.getElementById('uptolike').contentWindow;
          win.postMessage({
            action: 'initialize',
            json: UptolikeSettingsJson
          }, "*");
        }
        else {
          // Save data in hidden fields ans submit form.
          if (('json' in e.data) && ('code' in e.data)) {
            $('input[name="code"]').val(e.data.code);
            $('input[name="json"]').val(e.data.json);
            $('#ctools-export-ui-edit-item-form').submit();
          }
        }

        // Check secret key.
        if ($('input[name="uptolike_status"]').length) {
          if ((e.data.action == 'badCredentials') || (e.data.action == 'foreignAccess')) {
            $('input[name="uptolike_status"]').val(e.data.action);
            $('#uptolike-admin-statistic-form').submit();
          }
          else {
            $('input[name="uptolike_status"]').val('ready');
            $('#uptolike-admin-statistic-form').submit();
          }
        }

        // Apply custom height from context.
        if (typeof e.data.size !== 'undefined' && e.data.size !== null) {
          $('#uptolike').css('height', e.data.size);
        }

      };

      if (typeof window.addEventListener != 'undefined') {
        window.addEventListener('message', onmessage, false);
      } else if (typeof window.attachEvent != 'undefined') {
        window.attachEvent('onmessage', onmessage);
      }
      var getCode = function(){
        var win = document.getElementById('uptolike').contentWindow;
        win.postMessage({action: 'getCode'}, "*");
      };

      // This page will not work without javascript so no big deal here that
      // the button is disabled via javascript.
      $('#ctools-export-ui-edit-item-form input#edit-submit').click(function() {
        getCode();
        return false;
      });

      // Build secret key and change iframe scr.
      $('#uptolike-admin-statistic-form input#edit-submit').click(function() {
        var userEmail = $('#edit-uptolike-email').val()
        var userKey = $.trim($('#edit-uptolike-key').val());
        var statisticString = 'mail=' + userEmail + '&partner=' + settings.Uptolike.partner + '&projectId=' + settings.Uptolike.projectId;
        var cryptKey = CryptoJS.MD5(statisticString + userKey).toString();
        var iframeUrl = 'https://uptolike.com/api/statistics.html?' + statisticString + '&signature=' + cryptKey;
        $("#uptolike").attr("src", iframeUrl);
        getCode();
        return false;
      });
    }
  };
})(jQuery, Drupal);
