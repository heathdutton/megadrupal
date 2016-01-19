/**
 * @file
 * Attaches the behaviors for the SN QUICK Add module.
 */

(function($) {

  Drupal.behaviors.fieldUIFieldOverview = {
    attach: function(context, settings) {
      $('#quick-field-add-new-field-form', context).once('quick-field-add-new-field-form', function() {
        Drupal.fieldUIFieldOverview.attachUpdateSelects(this, settings);
      });
    }
  };

  Drupal.fieldUIFieldOverview = {
    /**
     * Implements dependent select dropdowns on the 'Manage fields' screen.
     */
    attachUpdateSelects: function(table, settings) {
      var widgetTypes = settings.fieldWidgetTypes;
      var fields = settings.fields;

      // Store the default text of widget selects.
      var initialValue = '';
      $('.widget-type-select', table).each(function() {
        initialValue = this.options[0].text;
      });

      // 'Field type' select updates its 'Widget' select.
      $('.field-type-select', table).each(function() {
        this.targetSelect = $('.widget-type-select', $(this).closest('.field-type-select'));

        $(this).bind('change keyup', function() {
          var selectedFieldType = this.options[this.selectedIndex].value;
          var options = (selectedFieldType in widgetTypes ? widgetTypes[selectedFieldType] : []);
          this.targetSelect.fieldUIPopulateOptions(options, '', initialValue);
        });
        // Trigger change on initial pageload to get the right widget options
        // when field type comes pre-selected (on failed validation).
        $(this).trigger('change', false);
      });
    }
  };

  /**
   * Populates options in a select input.
   */
  jQuery.fn.fieldUIPopulateOptions = function(options, selected, initialValue) {
    var html = '';
    if (options.length == 0) {
      options = [initialValue];
      disabled = true;
      jQuery.each(options, function(value, text) {
        // Figure out which value should be selected. The 'selected' param
        // takes precedence.
        var is_selected = ((typeof selected != 'undefined' && value == selected) || (typeof selected == 'undefined' && text == previousSelectedText));
        html += '<option value="' + value + '"' + (is_selected ? ' selected="selected"' : '') + '>' + text + '</option>';
      });
      $('#edit-widget-type').html(html).attr('disabled', disabled ? 'disabled' : false);
      $('#dropdown_second_replace').hide();
    }
    else {
      options = $.extend({'': initialValue}, options);
      //Array.prototype.push.call(options, initialValue);
      jQuery.each(options, function(value, text) {
        // Figure out which value should be selected. The 'selected' param
        // takes precedence.
        var is_selected = ((typeof selected != 'undefined' && value == selected) || (typeof selected == 'undefined' && text == previousSelectedText));
        html += '<option value="' + value + '"' + (is_selected ? ' selected="selected"' : '') + '>' + text + '</option>';
      });
      $('#edit-widget-type').html(html).attr('disabled', false);
      $('#dropdown_second_replace').show();
    }
  };
})(jQuery);

(function($, Drupal)
{
  // Our function name is prototyped as part of the Drupal.ajax namespace, adding to the commands:
  Drupal.ajax.prototype.commands.resetTypeWidgetFields = function(ajax, response, status)
  {
    $('#edit-type').val('');
    $('#edit-widget-type').val('');
    $('#dropdown_second_replace').html('<div class= "messages ' + response.messageType + '">' + response.selectedValue + '</div>');
    $('#quick-field-add-new-field-form').each(function(){
      this.reset();
    });
    $('#console').html('');
      return false;
  };
}(jQuery, Drupal));
(function($, Drupal)
{
	// Our function name is prototyped as part of the Drupal.ajax namespace, adding to the commands:
	Drupal.ajax.prototype.commands.resetFormOnError = function(ajax, response, status)
	{
    // The value we passed in our Ajax callback function will be available inside the
    // response object. Since we defined it as selectedValue in our callback, it will be
    // available in response.selectedValue. Usually we would not use an alert() function
    // as we could use ajax_command_alert() to do it without having to define a custom
    // ajax command, but for the purpose of demonstration, we will use an alert() function
    // here:
    $('#edit-type').val('');
    $('#edit-widget-type').val('');
    $('#dropdown_second_replace').html('<div class= "messages ' + response.messageType + '">' + response.selectedValue + '</div>');
    $('#quick-field-add-new-field-form').each(function(){
      this.reset();
    });
    $('#console').html('');
      return false;
  };
}(jQuery, Drupal));
