(function($) {
  /**
   * Initialization
   */
  Drupal.behaviors.jquery_ui_multiselect_widget = {
    /**
     * Run Drupal module JS initialization.
     *
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      if (!settings.jquery_ui_multiselect_widget) {
        // No settings found. Cancel!
        return;
      }

      // Global context!
      var filter = "select";
      if (settings.jquery_ui_multiselect_widget.multiple) {
        // Multiple only
        filter = filter + '[multiple=multiple]';
      }
      var elements = $(filter, context);
      if (jQuery.trim(settings.jquery_ui_multiselect_widget.subselector) !== '') {
        // Subselector
        elements = elements.filter(settings.jquery_ui_multiselect_widget.subselector);
      }
      // Convert int 1 to boolean so that the header works correctly.
      if (settings.jquery_ui_multiselect_widget.header === 1) {
        settings.jquery_ui_multiselect_widget.header = true;
      }
      elements.each(function() {
        $(this).once('ui-multiselect', function() {
          var isMultiselect = $(this).is('[multiple]');
          var multiselect = $(this).multiselect({
            // Get default options from drupal to make them easier accessible.
            selectedList: settings.jquery_ui_multiselect_widget.selectedlist,
            selectedText: function(numChecked, numTotal, checkedItems) {
              // Override text to make it translateable.
              return Drupal.t('@numChecked of @numTotal checked', {'@numChecked': numChecked, '@numTotal': numTotal});
            },
            multiple: isMultiselect,
            autoOpen: settings.jquery_ui_multiselect_widget.autoOpen,
            header: settings.jquery_ui_multiselect_widget.header,
            height: settings.jquery_ui_multiselect_widget.height,
            classes: settings.jquery_ui_multiselect_widget.classes,
            checkAllText: Drupal.t('Check all'),
            uncheckAllText: Drupal.t('Uncheck all'),
            noneSelectedText: Drupal.t('Select option(s)'),
          });
          if (settings.jquery_ui_multiselect_widget.filter) {
            // Allow filters
            multiselect.multiselectfilter({
              label: Drupal.t('Filter'),
              placeholder: Drupal.t('Enter keywords'),
              width: settings.jquery_ui_multiselect_widget.filter_width,
              autoReset: settings.jquery_ui_multiselect_widget.filter_auto_reset
            });
          }
        });
      });
    }
  };
})(jQuery);