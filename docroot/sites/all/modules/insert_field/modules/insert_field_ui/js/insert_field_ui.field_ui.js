Drupal.behaviors.insertFieldUIFieldUI = {
  attach: function (context, settings) {

    // Get the Taible.
    var table = jQuery('#field-overview, #field-display-overview');

    // Attach the Handlers to the Table once.
    jQuery(table).once('insert-field', function() {

      // Get the Insert Field Settings.
      var insert_field = settings.insert_field;

      // Create a Row ID Object.
      insert_field.row_id = new Object();

      // Loop through the Field Settings.
      for (var field in Drupal.settings.insert_field.fields) {

        // Save the field_machine_name indexed by row id.
        insert_field.row_id[field.replace('_', '-')] = field;

        // Save the row_id indexed by field_machine_mane.
        insert_field.fields[field].row_id = field.replace('_', '-');

      }

      // Get the ID of the table.
      var id = jQuery(table).attr('id');

      // Save the existing onDrop() method for later use.
      Drupal.tableDrag[id]._onDrop = Drupal.tableDrag[id].onDrop;

      // Override the existing onDrop() method.
      Drupal.tableDrag[id].onDrop = Drupal.insertFieldFieldUI.onDrop;

      // Save the existing validIndentInterval() for later use.
      Drupal.tableDrag[id].row.prototype._validIndentInterval = Drupal.tableDrag[id].row.prototype.validIndentInterval;

      // Override the existing validIndentInterval() method.
      Drupal.tableDrag[id].row.prototype.validIndentInterval = Drupal.insertFieldFieldUI.row.validIndentInterval;

    });

  }
};

Drupal.insertFieldFieldUI = {};

/*
 * Replaces the Field UI onDrop() method.
 * This is done to keep the 'region' up to date with the field children.
 *
 * @see: Drupal.fieldUIOverview.onDrop().
 */
Drupal.insertFieldFieldUI.onDrop = function() {
  var dragObject = this;
  var row = dragObject.rowObject.element;
  var rowHandler = jQuery(row).data('fieldUIRowHandler');

  // Get the Insert Field Settings.
  var settings = Drupal.settings.insert_field;

  // Get the Formatter Value.
  var formatter = jQuery('.field-formatter-type', row);

  // If the formatter cannot be found, pass back to the origin method.
  if (!formatter.length) {
    return this._onDrop();
  }

  var value = jQuery(formatter).get(0).value;

  // Start an empty refresh object for storing all of the refreshRows.
  // This allows use to perform a single AJAXRefreshRows().
  var refresh = {};

  // Start the refreshRows as an empty object.
  var refreshRows = {};

  // Start the Region as an empty string since the region we are moving towards
  // is not yet known.
  var region = '';

  // Ensure that the rowHandler is defined.
  if (typeof rowHandler != 'undefined') {

    // Get the Region we are moving towards.
    var regionRow = jQuery(row).prevAll('tr.region-message').get(0);
    var region = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');

    // Attempt to refresh the row.
    if (refreshRows = Drupal.insertFieldFieldUI.regionChange(region, rowHandler)) {

      // Add the refresh row to our mastor object.
      refresh = jQuery.extend(refreshRows, refresh);

    }

  }

  // If a Region was found, and the row is an insert field:
  if (region && typeof settings.row_id[row.id] != 'undefined') {

    // Loop through every parent field, searching for children to the current row.
    jQuery('select.field-parent').each(function() {

      // Get the value and compare it to the current row field id.
      if (jQuery(this).get(0).value == settings.row_id[row.id]) {

        // Get the Row.
        var childRow = jQuery(this).parents('tr');

        // Get the rowHandler for the current child row.
        var childRowHandler = jQuery(childRow).data('fieldUIRowHandler');

        // Ensure that the rowHandler does in fact exist.
        if (typeof childRowHandler != 'undefined') {

          // Attempt to refresh the child row(s).
          if (refreshRows = Drupal.insertFieldFieldUI.regionChange(region, childRowHandler)) {

            // Add the refresh row to our mastor object.
            refresh = jQuery.extend(refreshRows, refresh);

          }

        }



      }

    })

  }

  // Ajax-update the rows.
  Drupal.fieldUIOverview.AJAXRefreshRows(refreshRows);

}

/*
 * Region change method.
 *
 * @see: Drupal.fieldUIOverview.onDrop().
 */
Drupal.insertFieldFieldUI.regionChange = function(region, rowHandler) {

  // Check to ensure that the region is actually changing.
  if (region != rowHandler.region) {

    // Let the row handler deal with the region change.
    refreshRows = rowHandler.regionChange(region);

    // Update the row region.
    rowHandler.region = region;

    // Return the refreshRows object.
    return refreshRows;

  }

  // Since nothing was done, return an empty object.
  return {};

}

Drupal.insertFieldFieldUI.row = {};

/*
 * Save the existing validIndentInterval() method for later use.
 */
Drupal.insertFieldFieldUI.row._validIndentInterval = Drupal.tableDrag.prototype.row.prototype.validIndentInterval;

/*
 * Override the exisitng validIndentInterval() method.
 * Since some fields can be inserted (under certain condtions), and some cannot
 * be inserted at all, we must instruct the rowHandler if the indentation is
 * valid or not.
 *
 * @see: Drupal.tableDrag.prototype.row.prototype.validIndentInterval.
 */
Drupal.insertFieldFieldUI.row.validIndentInterval = function (prevRow, nextRow) {

  // Get the Current Row.
  var row = jQuery(this.element);

  // Get the Insert Field Settings.
  var settings = Drupal.settings.insert_field;

  // Execute the Existing validIndentInterval() method.
  var interval = Drupal.insertFieldFieldUI.row._validIndentInterval(prevRow, nextRow);

  // If the Interval Max is 0, return original result.
  if (!interval.max) {
    return interval;
  }

  // Get the ID of the current row.
  var id = jQuery(row).attr('id');

  // Get the Literal Next Row, even if it's being dragged with the current row.
  var next = jQuery(row).next();

  // Get the Field Data for all Rows.
  var row_data = Drupal.insertFieldFieldUI.row.getFieldData(row);
  var prev_data = Drupal.insertFieldFieldUI.row.getFieldData(prevRow);
  var next_data = Drupal.insertFieldFieldUI.row.getFieldData(next);

  // If the Curren Row is not a Field:
  if (!row_data.is_field) {

    // Prevent the Row from being a Child of a Field.
    if (prev_data.is_parent || prev_data.has_parent) {
      interval.max = interval.max - 1;
      return interval;
    }
    else {
      return interval;
    }

  }

  // If the Previous Field has a parent and is a parent itself,
  // then prevent fields from being inserted two levels deep.
  if (prev_data.has_parent && prev_data.is_parent) {
    interval.max = interval.max - 1;
    return interval;
  }

  // By default, the Next Row is not a child of the Current Row.
  var next_is_child = false;

  // Only rows that are parents can have children.
  if (row_data.is_parent) {

    // If the Next Row has a parent, then it might be a child.
    if (next_data.has_parent) {

      // If the Next Row's parent, matches the current row, it is a child.
      if (row_data.field_name == next_data.row_parent) {
        next_is_child = true;
      }

    }
  }

  // Rows that already have children, should not be inserted into fields.
  if (next_is_child && (prev_data.has_parent || prev_data.is_parent)) {
    interval.max = interval.max - 1;
    return interval;
  }

  // Return the interval.
  return interval;

};

/*
 * Retreive Data about a Field.
 */
Drupal.insertFieldFieldUI.row.getFieldData = function(row) {

  // Get the Insert Field Settings.
  var settings = Drupal.settings.insert_field;

  // Get the ID of the current row.
  var id = jQuery(row).attr('id');

  // Initialzie the Data object with defaults.
  var data = {
    is_field: false,
    is_parent: false,
    field_name: '',
    row_parent: '',
    has_parent: false
  };

  // Determine if the curren field is a field.
  data.is_field = settings.row_id.hasOwnProperty(id);

  // If the row is a field, it might be a parant, and
  // it has a field name.
  if (data.is_field) {

    // Row is a parent if it's in the list of parents.
    data.is_parent = settings.parents.hasOwnProperty(settings.row_id[id]);

    // Get the Field Name.
    data.field_name = settings.row_id[id];

  }

  // If the row has a Field Parent select box, it might have a parent.
  if (jQuery('select.field-parent', row).length) {

    // Get the value of the parent field.
    data.row_parent = jQuery('select.field-parent', row).get(0).value;

  }

  // If the row is a Field and it has a Parent set,
  // determine if it's has a field parent.
  if (data.is_field && data.row_parent) {

    // Field has a parent if the value of the select box, is in the list of parents.
    data.has_parent = settings.parents.hasOwnProperty(data.row_parent);

  }

  // Return the modified data object.
  return data;

}
