/**
 * @file
 * Enables the autocomplete dependent field population functionality.
 */

(function ($) {

  /**
   * Attach handlers to form element of type autocomplete_dependee to populate
   * it's depedendent form elements.
   */
  Drupal.behaviors.autocompleteDependentPopulation = {
    attach: function (context, settings) {
      // This variable contains the keyup fields name.
      var autocompleteDependeeFields = '';

      // This array contains the callback paths supplied as #path arguments.
      var urlsArr = [];

      // This variable contains keycode of previously keyup element.
      var prevKeycode = '';

      // This array contains the settings values.
      var settingsArray = settings['autocomplete_dependent_population'];

      // This variable contains Drupal base path.
      var DrupalBasePath = settings.basePath;

      // Concatenate the autocomplete dependee form elements and populate the
      // path to callback functions that will return the result in JSON format.
      for (var index = 0; index < settingsArray.length; index++) {
        autocompleteDependeeFields += "input[name='" + settingsArray[index]['keyupField'] + "'],";
        urlsArr[settingsArray[index]['keyupField']] = settingsArray[index]['url'];
      }

      autocompleteDependeeFields = autocompleteDependeeFields.slice(0, -1);

      // This variable contains the selected value.
      var lastSelectId = '';

      // This variable contains the response from the callback path in
      // JSON format.
      var resObj = '';

      $(autocompleteDependeeFields, context).once().keyup(function (event) {
        // This variable contains the encoded value of the keyup field.
        var input = Drupal.encodePath($.trim($(this).val()));

        // This variable contains how many elements returned from the callback
        // path in JSON format.
        var count  = 0;

        // The name of the autocomplete dependee form element.
        var fieldName = $(this).attr('name');

        // The page callback which will provide the result.
        var callBackUrl = urlsArr[fieldName];

        // The complete callback path with arguments as the keyup field name
        // and the keyup value.
        var url = DrupalBasePath + callBackUrl + '/' + fieldName + '/' + input;

        // This array contains the list of values of autocomplete dependent
        // form element.
        var autocompleteValues = [];

        if (event.keyCode === 37 || event.keyCode === 39 ||
          event.keyCode === 17 || (prevKeycode === 17 && event.keyCode === 65)) {
          // Do not invoke callback path for the keycodes.
        }
        else if (input !== '' && event.keyCode !== 37 && event.keyCode !== 39) {
          // Make a ajax call to fetch the response matching with keyup value.
          $.ajax({
            type: "GET",
            url : url,
            dataType: "json",
            async : false,
            success: function(response) {
              // Populate the response which are the values of dependent form
              // elements in JSON formats into an array.
              if ($.isEmptyObject(response) === false) {
                resObj = response['data'];
                $.each(response['data'], function(index , value) {
                  autocompleteValues[count] = index;
                  count++;
                });
              }
            },
            error: function (xmlhttp) {
              alert(Drupal.ajaxError(xmlhttp, url));
            }
          });

          // This section actually populate the autocomplete dependent form
          // elements when a value from autocomplete suggestion list is
          // selected.
          $(autocompleteDependeeFields).autocomplete({
            source: autocompleteValues,
            minLength: 1,
            select : function (event, ui){
              lastSelectId = ui.item.value;
              // Set the dependendent form elements.
              Drupal.autocompleteSetUnsetValue(resObj[lastSelectId], 'Yes');
            }
          });
        }
        else {
          if($.isEmptyObject(resObj[lastSelectId]) === false){
            // Unset the dependendent form elements when backspace or delete
            // key is pressed.
            Drupal.autocompleteSetUnsetValue(resObj[lastSelectId], 'No');
          }
        }
        // Assign the keycode of current keyup element.
        prevKeycode = event.keyCode;
      });

      // Need to initialize the autocomplete function otherwise no suggestion
      // list will be generated on the first keyup event.
      $(autocompleteDependeeFields).autocomplete({
        source: []
      });
    }
  };

  /**
   * Set or unset dependent form elements.
   *
   * @param resObjectSelected
   *   An object containing values for dependent form elements.
   * @param type
   *   A string containing whether the dependent form elements need to be
   *   set/unset.
   */
  Drupal.autocompleteSetUnsetValue = function (resObjectSelected, type) {
    var checkedValue, textValue;
    $.each(resObjectSelected, function(field_type, field_info) {
      $.each(field_info, function(field_name, field_value) {
        // Check whether the function is called to set or unset form elements.
        if (type === 'Yes') {
          checkedValue = true;
          textValue = field_value;
        }
        else if (type === 'No') {
          checkedValue = false;
          textValue = '';
        }
        // The following section checks the type of dependent form elements and
        // set/unset values in the dependent form elements.
        if (field_type === 'radio') {
          $('input:radio[name=' + field_name + '][value=' + field_value + ']').attr('checked', checkedValue);
        }
        else if (field_type === 'checkbox') {
          var temp_field_value = field_value.split(',');
          $.each(temp_field_value, function(chk_key, chk_value) {
            $("#" + $('input[name="' + field_name + '[' + $.trim(chk_value) + ']"]').attr('id')).attr('checked', checkedValue);
          });
        }
        else {
          $("#" + $('*[name="' + field_name + '"]').attr('id')).val(textValue);
        }
      });
    });
  };
})(jQuery);
