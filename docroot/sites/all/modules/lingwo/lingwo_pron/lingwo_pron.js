
(function ($) {
  Drupal.behaviors.lingwo_pron = {
    attach: function (context, settings) {
      function triggerAjax(evt) {
        evt.preventDefault();
        var id = 'edit-lingwo-pron-refresh',
          ajax_settings = settings.ajax[id],
          event_name = ajax_settings['event'],
          selector = ajax_settings['selector'];
        $(selector).trigger(event_name);
        return false;
      }

      Drupal.lingwo_entry.setCallback('lingwo_pron', 'language', triggerAjax);

      // bind to remove checkboxes to trigger AHAH
      $('.lingwo-pron-remove', context).once('lingwo_pron', function () {
        // TODO: for some reason, the remove checkbox remains checked!
        $(this).removeAttr('checked');
        $(this).change(triggerAjax);
      });

      // bind to the upload button as well
      $('.lingwo-pron-upload', context).once('lingwo_pron', function () {
        $(this).click(triggerAjax);
      });
    }
  };
  //
  // Replace Drupal.ajaxError to give a sane message to users
  // TODO: can I do this just for our AJAX callback?
  // TODO: does this still make sense in Drupal 7?
  /*
  Drupal.ajaxError = function (xmlhttp, uri) {
    return xmlhttp.responseText;
  }
  */
})(jQuery);

