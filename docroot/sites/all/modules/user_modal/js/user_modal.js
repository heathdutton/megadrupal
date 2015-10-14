(function ($) {

/**
 * JS related to the tabs in the user modal.
 */
Drupal.behaviors.userModal = {
  attach: function (context) {
  
    $('#tabs:not(.tabs-processed)', context)
      .addClass('tabs-processed')  
      // Turn div to tabs, and select the tab as was send by the server.
      // The server already translated the names of the selected tabs to their 
      // numeric value (i.e. regsiter = 0, login = 1, password = 2).
      .tabs({"selected": Drupal.settings.userModal.selectedTab})
      // Bind click on tabs, so we can send back to server which tab is 
      // selected.
      .bind('tabsselect', function(event, ui) {
        // Translate the tab numeric value to its name.
        var tabName;
        if (ui.index == 0) {
          tabName = 'register';
        }
        else if (ui.index == 1) {
          tabName = 'login';  
        }
        else {
          tabName = 'password';
        }
        // Set the selection of the tab.
        $('.form-item-selected-tab select')
          .val(tabName)
          // Trigger change event so implementing modules can react.
          .trigger('change');
     });
  }
};

})(jQuery);


