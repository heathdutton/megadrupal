
/**
 * @file
 * Enhancements for select list configuration options.
 */

(function ($) {

  Drupal.behaviors.dquarksSelectLoadOptions = {};
  Drupal.behaviors.dquarksSelectLoadOptions.attach = function(context) {
    settings = Drupal.settings;

    $('#edit-extra-options-source', context).change(function() {
      var url = settings.dquarks.selectOptionsUrl + '/' + this.value;
      $.ajax({
        url: url,
        success: Drupal.dquarks.selectOptionsLoad,
        dataType: 'json'
      });
    });
  }

  Drupal.dquarks = Drupal.dquarks || {};

  Drupal.dquarks.selectOptionsOriginal = false;
  Drupal.dquarks.selectOptionsLoad = function(result) {
    if (Drupal.optionsElement) {
      if (result.options) {
        // Save the current select options the first time a new list is chosen.
        if (Drupal.dquarks.selectOptionsOriginal === false) {
          Drupal.dquarks.selectOptionsOriginal = $(Drupal.optionElements[result.elementId].manualOptionsElement).val();
        }
        $(Drupal.optionElements[result.elementId].manualOptionsElement).val(result.options);
        Drupal.optionElements[result.elementId].disable();
        Drupal.optionElements[result.elementId].updateWidgetElements();
      }
      else {
        Drupal.optionElements[result.elementId].enable();
        if (Drupal.dquarks.selectOptionsOriginal) {
          $(Drupal.optionElements[result.elementId].manualOptionsElement).val(Drupal.dquarks.selectOptionsOriginal);
          Drupal.optionElements[result.elementId].updateWidgetElements();
          Drupal.dquarks.selectOptionsOriginal = false;
        }
      }
    }
    else {
      if (result.options) {
        $('#' + result.elementId).val(result.options).attr('readonly', 'readonly');
      }
      else {
        $('#' + result.elementId).attr('readonly', '');
      }
    }
  }

})(jQuery);
