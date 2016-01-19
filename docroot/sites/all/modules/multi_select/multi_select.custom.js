/**
* @file
* Javascript, modifications of DOM.
*
* Manipulates links to include scrollreveal data
*/

(function ($) {
  Drupal.behaviors.multi_select = {
    attach: function (context, settings) {
      var triggers = Drupal.settings.multi_select.triggers_fieldset;
      $.each(triggers, function(key, trigger) {
        $(trigger.element).multiSelect({
          selectableHeader: trigger.selectableheader,
          selectionHeader: trigger.selectedheader,
          selectableFooter: trigger.selectablefooter,
          selectionFooter: trigger.selectedfooter
        });
      });
    }
  }  

}(jQuery));
