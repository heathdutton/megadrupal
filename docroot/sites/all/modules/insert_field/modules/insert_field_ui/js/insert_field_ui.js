Drupal.behaviors.insertFieldUI = {
  attach: function (context, settings) {

    // Initiate the tabs with no tab selected.
    jQuery('.insert-field-tabs').once('insert-field').tabs({
      selected: -1,
      collapsible: true
    });

    // Add a click handler to the Insert Button.
    jQuery('.insert-button').once('button-click').click(function(e) {

      // Do Not execute the button fuction.
      e.preventDefault();

      // Get the id of the parent.
      var tabs = jQuery(this).parents('.insert-field-tabs');
      var parent_id = jQuery(tabs).attr('id').replace('insert-', 'edit-');
      var parent = jQuery('#'+parent_id);

      // Get the field from the name attribute.
      var field = jQuery(this).attr('name');

      // Insert the Comment into the Parent Field.
      jQuery('textarea.text-full', parent).caret('<!-- '+field+' -->');

    });

  }
};
