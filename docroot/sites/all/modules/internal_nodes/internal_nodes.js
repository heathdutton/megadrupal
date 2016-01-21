(function ($) {

  Drupal.behaviors.internal_nodesFieldsetSummaries = {
    attach: function (context) {
      $('fieldset#edit-internal-nodes', context).drupalSetSummary(function (context) {

        // Retrieve the value of the selected radio button
        var action = $("input[name=internal_nodes_action]:checked").val();

        if (action==0) {
          return Drupal.t('Content type default')
        }
        else if (action==200) {
          return Drupal.t('Allow')
        }
        else if (action==403) {
          return Drupal.t('Access denied - 403')
        }
        else if (action==404) {
          return Drupal.t('File not found - 404')
        }
        else if (action==301) {
          return Drupal.t('Redirect - 301')
        }
      });
    }
  };

})(jQuery);
