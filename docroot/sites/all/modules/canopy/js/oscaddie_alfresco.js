// <?php
/**
 * @file
 * The file which handles the results of the Alfresco Server ping.
 *
 * Pings alfresco server for status.
 */
// ?>

(function ($) {
  Drupal.behaviors.oscaddie_alfrescoAlfrescoStatus = {
    attach: function (context, settings) {
      // Send a ping to Alfresco server for status
      //jQuery.ajax({
      //  type: 'GET',
      //  dataType: 'json',
      //  url: '//' + window.location.host + '/oscaddie_alfresco/ajax/alfresco-status',
      //  success: function (data, status, jqXHR) {
      //    if (data == true) {
      //      $('#portal-alfresco table span.status').html('<font color="green">Online</font>');
      //    }
      //    else {
      //      $('#portal-alfresco table span.status').html('<font color="red">Offline</font>');
      //    }
      //  },
      //  error: function (jqXHR, status, errorThrown) {
      //    $('#portal-alfresco table span.status').html('<font color="red">Offline</font>');
      //  }
      //});
    }
  };
})(jQuery);
