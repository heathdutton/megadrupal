(function ($) {

  Drupal.behaviors.evanced_events_importer_toggle = {
  attach: function(context) {

    $().ready(function() {

      // One of the add/remove buttons was just clicked on.
      $('div.toggle_buttons input').click(function() {

        var mapped_list_values = '';
        var recipient_list_id = '';
        var donor_list_id = '';

        // Retrieve the ID of the button that was clicked on.
        var button_id = $(this).attr('id');

        // Determine the type of action the user
        // wants to perform based on the button ID.
        var action = button_id.match(/(add|remove)/);

        // Set the row number of the button as a variable.
        var row_num = button_id.replace(/[^0-9]+/g, '');

        // Since almost all of the div IDs in the current row start the
        // same way, set the beginning of the ID string as a variable.
        var mapped_fields_id = '#mapped_fields select#edit-evanced-events-importer-field' + row_num;

        // Determine if the user wants to add
        // an option to the righthand column.
        if (action[0] == 'add') {
          recipient_list_id = mapped_fields_id + '-xml-elements';
          donor_list_id = '#xml_tags select#edit-exml-list';
        }
        // Determine if the user wants to remove
        // an option from the righthand column.
        else if (action[0] == 'remove') {
          recipient_list_id = '#xml_tags select#edit-exml-list';
          donor_list_id = mapped_fields_id + '-xml-elements';
        }

        // Loop through the selected options in the donor column.
        $(donor_list_id + ' :selected').each(function(i, selected) {
          // Create a new option and populate it with the donor data.
          // NOTE: There are more efficient ways to do this in
          // jQuery. However, this is the only method that doesn't
          // cause Drupal to throw an error upon submission.
          var option = document.createElement("option");
          option.value = $(selected).val();
          option.text = $(selected).text();
          // Add the new option to the recipient column.
          $(recipient_list_id).get(0)[$(recipient_list_id + ' option').length] = option;
        });

        // Remove the selected options from the donor column.
        $(donor_list_id + ' :selected').remove();

        // Retrieve all the option values in the righthand column
        // of the active row and set them as a comma-delimited string.
        mapped_list_values = $(mapped_fields_id + '-xml-elements').children().map(function() {return $(this).val();}).get();

        // Update a hidden field with the string of mapped elements.
        $('#edit-evanced-events-importer-field' + row_num + '-xml-elements-list').val(mapped_list_values);

      });

    });

  }

  }

})(jQuery);